#include <gtest/gtest.h>

#include "Mocks/MockShaderCompiler.hpp"
#include <SWEditor/Graphics/ShaderContentAddressLoader.hpp>

using namespace PyroshockStudios;
using namespace PyroshockStudios::RHI;
using namespace PyroshockStudios::Editor;
using namespace PyroshockStudios::Types;

// TODO: why are these tests so slow? they are just as slow as the slang tests!!!!

TEST(TestShaderContentAddressCompiler, CompileCall) {
    using namespace ::testing;
    MockShaderCompiler compiler{};

    const eastl::string srcPath = "F:/shaders";
    const eastl::vector<eastl::string> shaders = {
        "test.slang:-stage=vsh",
        "test.slang:-stage=fsh",
    };

    EXPECT_CALL(compiler, CompileShaderFromFile(
                              (srcPath + "/test.slang"),
                              ShaderCompilationInfo{
                                  .stage = ShaderStage::Vertex,
                                  .name = shaders[0],
                              }))
        .Times(Exactly(1));

    ON_CALL(compiler, CompileShaderFromFile(
                          (srcPath + "/test.slang"),
                          ShaderCompilationInfo{
                              .stage = ShaderStage::Vertex,
                              .name = shaders[0],
                          }))
        .WillByDefault(Return(CompiledShaderResult{ "ba9571c0-1b8a-414a-846c-1e6378e7762f"_guid }));
    EXPECT_CALL(compiler, CompileShaderFromFile(
                              (srcPath + "/test.slang"),
                              ShaderCompilationInfo{
                                  .stage = ShaderStage::Fragment,
                                  .name = shaders[1],
                              }))
        .Times(Exactly(1));
    ON_CALL(compiler, CompileShaderFromFile(
                          (srcPath + "/test.slang"),
                          ShaderCompilationInfo{
                              .stage = ShaderStage::Fragment,
                              .name = shaders[1],
                          }))
        .WillByDefault(Return(CompiledShaderResult{ "5e8e0588-1301-4a93-95e0-08e38991a40b"_guid }));

    ShaderContentAddressLoader loader = ShaderContentAddressLoader(srcPath, shaders);
    auto table = loader.Load(&compiler);
    EXPECT_EQ(table.GetContentAddress(shaders[0]), "ba9571c0-1b8a-414a-846c-1e6378e7762f"_guid);
    EXPECT_EQ(table.GetContentAddress(shaders[1]), "5e8e0588-1301-4a93-95e0-08e38991a40b"_guid);
}


TEST(TestShaderContentAddressCompiler, ParseArguments) {
    using namespace ::testing;
    MockShaderCompiler compiler{};

    const eastl::string srcPath = "F:/shaders";
    const eastl::vector<eastl::string> shaders = {
        R"(test.slang:-stage=vsh -name="My Epic Shader" -defines="NO_MESH 'VERTEX_COLOR 0xFF00FFFF' DEFERRED_RENDERING 'MAX_LIGHTS 128' ")",
        R"(util/sprite.slang:-stage=fsh -name="Coloured Sprite" -entry="quadSpriteMain" ")",
    };

    EXPECT_CALL(compiler, CompileShaderFromFile(
                              (srcPath + "/test.slang"),
                              ShaderCompilationInfo{
                                  .stage = ShaderStage::Vertex,
                                  .defines = { { "NO_MESH", "" }, { "VERTEX_COLOR", "0xFF00FFFF" }, { "DEFERRED_RENDERING", "" }, { "MAX_LIGHTS", "128" } },
                                  .name = "My Epic Shader",
                              }))
        .Times(Exactly(1));
    EXPECT_CALL(compiler, CompileShaderFromFile(
                              (srcPath + "/util/sprite.slang"),
                              ShaderCompilationInfo{
                                  .stage = ShaderStage::Fragment,
                                  .entryPoint = "quadSpriteMain",
                                  .name = "Coloured Sprite",
                              }))
        .Times(Exactly(1));

    ShaderContentAddressLoader loader = ShaderContentAddressLoader(srcPath, shaders);
    auto table = loader.Load(&compiler);
}