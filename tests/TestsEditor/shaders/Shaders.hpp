#pragma once
namespace Shaders {
    // clang-format off
    constexpr const char* TestShader_slang = R"(
[[vk::binding(0, 1)]] struct Light {
    float3 position;
    float intensity;
};

[[vk::binding(1, 1)]] cbuffer SceneData {
    float4x4 viewProjection;
    Light lights[4];
    int lightCount;
};

[[push_constant]] cbuffer Push {
    float4 tint;
    int frameIndex;
};

[[vk::binding(0, 0)]] Texture2D<float4> gTexture : register(t0);
[[vk::binding(1, 0)]] RWTexture2D<float4> gRWTexture : register(u0);
[[vk::binding(2, 1)]] SamplerState gSampler : register(s0);

float4 main(float2 uv : TEXCOORD) : SV_Target {
    float4 color = gTexture.Sample(gSampler, uv);
    return color * tint;
}
)";

    constexpr const char* TestShaderNestedArray_slang = R"(
[[push_constant]] cbuffer Push {
    float2 colors[2][4];
    int frameIndex;
};

float4 main(float2 uv : TEXCOORD) : SV_Target {
    return float4(1,1,1,1) * colors[0][0].x * colors[0][1].x;
}
)";
    constexpr const char* TestShaderSRVTest_slang = R"(
struct Light {
    float4 color;
    float intensity;
};

[[vk::binding(0, 0)]] StructuredBuffer<Light> gLight : register(t0);

float4 main(float2 uv : TEXCOORD) : SV_Target {
    return gLight.Load(0).color * gLight.Load(0).intensity;
}
)";

    constexpr const char* TestShaderParameterBlockTest_slang = R"(
struct Camera {
    float4x4 mvp;
};

struct Light {
    float4 color;
    float intensity;
};

struct TestResources {
    [[vk::binding(0, 0)]] StructuredBuffer<Camera> gCamera : register(t0);
    [[vk::binding(1, 0)]] StructuredBuffer<Light>  gLight  : register(t1);
};

ParameterBlock<TestResources> resources;

float4 main(float2 uv : TEXCOORD) : SV_Target {
    Light l = resources.gLight.Load(0);
    return l.color * l.intensity;
}
)";

    // clang-format on
}