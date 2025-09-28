#pragma once

#include <SWEditor/Graphics/IShaderCompiler.hpp>
#include <gmock/gmock.h>

using namespace PyroshockStudios;
using namespace PyroshockStudios::Types;
using namespace PyroshockStudios::RHI;

class MockShaderCompiler : public IShaderCompiler {
public:
    MOCK_METHOD(CompiledShaderResult, CompileShaderFromFile, (const eastl::string&, const ShaderCompilationInfo&), (override));
    MOCK_METHOD(CompiledShaderResult, CompileShaderFromSource, (const eastl::string&, const ShaderCompilationInfo&, const eastl::string&), (override));
};