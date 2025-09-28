#include <gtest/gtest.h>

#include <SWCommon/Serialization/BinarySerializer.hpp>
#include <SWEditor/Graphics/SlangShaderCompiler.hpp>

#include "Fakes/FakeContentAddressManager.hpp"
#include "Stubs/SlangFeatureSetStub.hpp"
#include "shaders/Shaders.hpp"

using namespace PyroshockStudios;
using namespace PyroshockStudios::RHI;
using namespace PyroshockStudios::Editor;
using namespace PyroshockStudios::Types;


// Helper to simulate command-line input
class TestSlangReflection : public ::testing::Test {
protected:
    static inline SlangVulkan13FeatureSetStub gSlangFeatureSet;
    static inline FakeContentAddressManager gShaderCAM;
    static inline FakeContentAddressManager gReflectionCAM;

    static void SetUpTestSuite() {
    }
    static void TearDownTestSuite() {
    }

    static inline SlangShaderCompiler gCompiler = SlangShaderCompiler({
        .featureSet = &gSlangFeatureSet,
        .shaderProgramCAM = &gShaderCAM,
        .shaderReflectionCAM = &gReflectionCAM,
    });

    static ShaderReflection BuildReflection(const eastl::string& code, const ShaderCompilationInfo& info) {
        gShaderCAM.Reset();
        gReflectionCAM.Reset();
        CompiledShaderResult program = gCompiler.CompileShaderFromSource(code, info);

        if (!program.address)
            throw std::runtime_error("invalid address");

        ShaderReflection reflection;
        auto stream = gReflectionCAM.CreateContentReadStream(program.address);

        if (!stream)
            throw std::runtime_error("content stream failed");

        BinarySerializer serializer = BinarySerializer(stream, nullptr);
        serializer >> reflection;
        gReflectionCAM.ReleaseContentStream(stream);

        return reflection;
    }
};


// NOTE: assuming scalar layout here
TEST_F(TestSlangReflection, BasicShaderInputReflection) {
    ShaderReflection reflection = BuildReflection(Shaders::TestShader_slang, { .stage = ShaderStage::Fragment });
    // Now inspect reflected data

    printf("%s\n", reflection.ToString().c_str());

    // Check Push Constants
    ASSERT_TRUE(reflection.push.has_value());
    const auto& pc = *reflection.push;
    EXPECT_EQ(pc.name, "Push");
    EXPECT_EQ(pc.size, 20);
    ASSERT_TRUE(pc.members.has_value());
    ASSERT_EQ(pc.members->size(), 2);

    const auto& tintField = (*pc.members)[0];
    EXPECT_EQ(tintField.name, "tint");
    EXPECT_EQ(tintField.offset, 0);
    EXPECT_EQ(tintField.size, 16);
    EXPECT_EQ(tintField.type, ShaderScalarType::Float32);
    EXPECT_EQ(tintField.columnCount, 4);
    EXPECT_EQ(tintField.elementCount, 4); // vec4
    EXPECT_TRUE(tintField.IsVector());

    const auto& frameIndexField = (*pc.members)[1];
    EXPECT_EQ(frameIndexField.name, "frameIndex");
    EXPECT_EQ(frameIndexField.offset, 16);
    EXPECT_EQ(frameIndexField.size, 4);
    EXPECT_EQ(frameIndexField.type, ShaderScalarType::Int32);
    EXPECT_TRUE(frameIndexField.IsScalar());

    // Find the SceneData input:
    const ShaderInputInfo* sceneDataInput = nullptr;
    for (const auto& input : reflection.inputs) {
        if (input.name == "SceneData") {
            sceneDataInput = &input;
            break;
        }
    }

    ASSERT_NE(sceneDataInput, nullptr);
    EXPECT_EQ(sceneDataInput->type, ShaderInputType::UniformBuffer);
    ASSERT_TRUE(sceneDataInput->members.has_value());

    const auto& members = *sceneDataInput->members;
    ASSERT_EQ(members.size(), 3);

    // Check for struct

    // viewProjection
    const auto& viewProj = members[0];
    EXPECT_EQ(viewProj.name, "viewProjection");
    EXPECT_EQ(viewProj.type, ShaderScalarType::Float32);
    EXPECT_EQ(viewProj.offset, 0);
    EXPECT_EQ(viewProj.size, 64);
    EXPECT_EQ(viewProj.stride, 16);
    EXPECT_EQ(viewProj.rowCount, 4);
    EXPECT_EQ(viewProj.columnCount, 4);
    EXPECT_TRUE(viewProj.IsMatrix());

    // lights
    const auto& lights = members[1];
    EXPECT_EQ(lights.name, "lights");
    EXPECT_EQ(lights.offset, 64);
    EXPECT_EQ(lights.size, 64);
    EXPECT_EQ(lights.elementCount, 4);
    EXPECT_EQ(lights.stride, 16);
    EXPECT_TRUE(lights.IsArray());
    EXPECT_TRUE(lights.ArrayUndelyingType().has_value()); // array of structs
    ASSERT_EQ(lights.members->size(), 1);

    auto lightStruct = *lights.ArrayUndelyingType();

    EXPECT_TRUE(lightStruct.IsStruct());
    EXPECT_EQ(lightStruct.size, 16);
    EXPECT_TRUE(lightStruct.members.has_value());
    ASSERT_EQ(lightStruct.members->size(), 2);

    EXPECT_EQ((*lightStruct.members)[0].name, "position");
    EXPECT_EQ((*lightStruct.members)[0].type, ShaderScalarType::Float32);
    EXPECT_EQ((*lightStruct.members)[0].elementCount, 3); // float3

    EXPECT_EQ((*lightStruct.members)[1].name, "intensity");
    EXPECT_EQ((*lightStruct.members)[1].type, ShaderScalarType::Float32);
    EXPECT_TRUE((*lightStruct.members)[1].IsScalar());

    // lightCount
    const auto& lightCount = members[2];
    EXPECT_EQ(lightCount.name, "lightCount");
    EXPECT_EQ(lightCount.offset, 128);
    EXPECT_EQ(lightCount.size, 4);
    EXPECT_EQ(lightCount.type, ShaderScalarType::Int32);
    EXPECT_TRUE(lightCount.IsScalar());



    // Texture2D<float4> gTexture
    // RWTexture2D<float4> gRWTexture
    // SamplerState gSampler
    const ShaderInputInfo* gTexture = nullptr;
    const ShaderInputInfo* gRWTexture = nullptr;
    const ShaderInputInfo* gSampler = nullptr;
    for (const auto& input : reflection.inputs) {
        if (input.name == "gTexture") {
            gTexture = &input;
        } else if (input.name == "gRWTexture") {
            gRWTexture = &input;
        } else if (input.name == "gSampler") {
            gSampler = &input;
        }
    }

    EXPECT_NE(gTexture, nullptr);
    EXPECT_NE(gRWTexture, nullptr);
    EXPECT_NE(gSampler, nullptr);

    EXPECT_EQ(gTexture->type, ShaderInputType::Texture2D);
    EXPECT_EQ(gRWTexture->type, ShaderInputType::RWTexture2D);
    EXPECT_EQ(gSampler->type, ShaderInputType::Sampler);

    // check if the vk::binding works
    EXPECT_EQ(gTexture->binding, 0);
    EXPECT_EQ(gRWTexture->binding, 1);
    EXPECT_EQ(gSampler->binding, 2);
    EXPECT_EQ(gTexture->space, 0);
    EXPECT_EQ(gRWTexture->space, 0);
    EXPECT_EQ(gSampler->space, 1);
}


TEST_F(TestSlangReflection, NestedArray) {
    ShaderReflection reflection = BuildReflection(Shaders::TestShaderNestedArray_slang, { .stage = ShaderStage::Fragment });

    printf("%s\n", reflection.ToString().c_str());

    // Check Push Constants
    ASSERT_TRUE(reflection.push.has_value());
    const auto& pc = *reflection.push;
    EXPECT_EQ(pc.name, "Push");
    EXPECT_EQ(pc.size, 68);
    ASSERT_TRUE(pc.members.has_value());
    ASSERT_EQ(pc.members->size(), 2);

    const auto& colorField = (*pc.members)[0];
    EXPECT_EQ(colorField.name, "colors");
    EXPECT_EQ(colorField.offset, 0);
    EXPECT_EQ(colorField.size, 64);
    EXPECT_EQ(colorField.stride, 32);
    EXPECT_EQ(colorField.type, ShaderScalarType::Array);
    EXPECT_EQ(colorField.elementCount, 2); // array of arrays
    EXPECT_TRUE(colorField.IsArray());
    EXPECT_TRUE(colorField.ArrayUndelyingType().has_value());

    EXPECT_TRUE(colorField.ArrayUndelyingType()->IsArray());
    EXPECT_EQ(colorField.ArrayUndelyingType()->elementCount, 4); // array of vec2
    EXPECT_EQ(colorField.ArrayUndelyingType()->size, 32);
    EXPECT_EQ(colorField.ArrayUndelyingType()->type, ShaderScalarType::Array);

    EXPECT_TRUE(colorField.ArrayUndelyingType()->ArrayUndelyingType().has_value());
    EXPECT_TRUE(colorField.ArrayUndelyingType()->ArrayUndelyingType()->IsVector()); // vec2
    EXPECT_EQ(colorField.ArrayUndelyingType()->ArrayUndelyingType()->type, ShaderScalarType::Float32);
    EXPECT_EQ(colorField.ArrayUndelyingType()->ArrayUndelyingType()->elementCount, 2);

    const auto& frameIndexField = (*pc.members)[1];
    EXPECT_EQ(frameIndexField.name, "frameIndex");
    EXPECT_EQ(frameIndexField.offset, 64);
    EXPECT_EQ(frameIndexField.size, 4);
    EXPECT_EQ(frameIndexField.type, ShaderScalarType::Int32);
    EXPECT_TRUE(frameIndexField.IsScalar());
}

TEST_F(TestSlangReflection, SRV) {
    ShaderReflection reflection = BuildReflection(Shaders::TestShaderSRVTest_slang, { .stage = ShaderStage::Fragment });

    printf("%s\n", reflection.ToString().c_str());

    EXPECT_EQ(reflection.inputs.size(), 1);
    auto srv = reflection.inputs[0];


    EXPECT_EQ(srv.name, "gLight");
    EXPECT_EQ(srv.type, ShaderInputType::StructuredBuffer);
    EXPECT_EQ(srv.size, 20);
    ASSERT_TRUE(srv.members.has_value());
    ASSERT_EQ(srv.members->size(), 2);

    const auto& colorField = (*srv.members)[0];
    EXPECT_EQ(colorField.name, "color");
    EXPECT_EQ(colorField.offset, 0);
    EXPECT_EQ(colorField.size, 16);
    EXPECT_EQ(colorField.elementCount, 4);
    EXPECT_EQ(colorField.type, ShaderScalarType::Float32);
    EXPECT_TRUE(colorField.IsVector());

    const auto& intensityField = (*srv.members)[1];
    EXPECT_EQ(intensityField.name, "intensity");
    EXPECT_EQ(intensityField.offset, 16);
    EXPECT_EQ(intensityField.size, 4);
    EXPECT_EQ(intensityField.type, ShaderScalarType::Float32);
    EXPECT_TRUE(intensityField.IsScalar());
}



TEST_F(TestSlangReflection, ParameterBlock) {
    ShaderReflection reflection = BuildReflection(Shaders::TestShaderParameterBlockTest_slang, { .stage = ShaderStage::Fragment });

    printf("%s\n", reflection.ToString().c_str());
    EXPECT_EQ(reflection.inputs.size(), 2);

    auto srvCamera = reflection.inputs[0];

    EXPECT_EQ(srvCamera.name, "gCamera");
    EXPECT_EQ(srvCamera.type, ShaderInputType::StructuredBuffer);
    EXPECT_EQ(srvCamera.size, 64);
    ASSERT_TRUE(srvCamera.members.has_value());
    ASSERT_EQ(srvCamera.members->size(), 1);
    {
        const auto& mvpField = (*srvCamera.members)[0];
        EXPECT_EQ(mvpField.name, "mvp");
        EXPECT_EQ(mvpField.offset, 0);
        EXPECT_EQ(mvpField.size, 64);
        EXPECT_EQ(mvpField.rowCount, 4);
        EXPECT_EQ(mvpField.columnCount, 4);
        EXPECT_EQ(mvpField.stride, 16);
        EXPECT_EQ(mvpField.type, ShaderScalarType::Float32);
        EXPECT_TRUE(mvpField.IsMatrix());
    }

    auto srvLight = reflection.inputs[1];

    EXPECT_EQ(srvLight.name, "gLight");
    EXPECT_EQ(srvLight.type, ShaderInputType::StructuredBuffer);
    EXPECT_EQ(srvLight.size, 20);
    ASSERT_TRUE(srvLight.members.has_value());
    ASSERT_EQ(srvLight.members->size(), 2);
    {
        const auto& colorField = (*srvLight.members)[0];
        EXPECT_EQ(colorField.name, "color");
        EXPECT_EQ(colorField.offset, 0);
        EXPECT_EQ(colorField.size, 16);
        EXPECT_EQ(colorField.elementCount, 4);
        EXPECT_EQ(colorField.type, ShaderScalarType::Float32);
        EXPECT_TRUE(colorField.IsVector());

        const auto& intensityField = (*srvLight.members)[1];
        EXPECT_EQ(intensityField.name, "intensity");
        EXPECT_EQ(intensityField.offset, 16);
        EXPECT_EQ(intensityField.size, 4);
        EXPECT_EQ(intensityField.type, ShaderScalarType::Float32);
        EXPECT_TRUE(intensityField.IsScalar());
    }
}