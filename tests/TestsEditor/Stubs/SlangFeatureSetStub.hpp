#pragma once

#include <SWRHI/Shader/IShaderFeatureSet.hpp>

using namespace PyroshockStudios;
using namespace PyroshockStudios::Types;
using namespace PyroshockStudios::RHI;

struct SlangVulkan13FeatureSetStub : public IShaderFeatureSet {
    SlangVulkan13FeatureSetStub() = default;
    ShaderCompileTarget GetTarget() const override {
        return ShaderCompileTarget::Spirv;
    }
    const char* GetProfileName(ShaderStage shaderStage) const override {
        return "spirv_1_4";
    }
    const char* GetFileExtension() const override {
        return "spv";
    }
    const ShaderFeatureInfo& Features() const override {
        static auto features = ShaderFeatureInfo{
            .bDescriptorIndexing = true,
            .bBufferDeviceAddress = true,
            .bScalarLayout = true,
            .bDrawIndirectCount = true,
        };
        return features;
    }
    const eastl::span<eastl::pair<const char*, const char*>>& GlobalPreprocessorDefines() const override {
        static eastl::vector<eastl::pair<const char*, const char*>> preprocesor = {};
        static auto span = eastl::span(preprocesor.data(), preprocesor.size());
        return span;
    }
};