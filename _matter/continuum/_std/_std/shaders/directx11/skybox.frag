    struct PointLight {
        float4 position;
        float4 color;
    };



cbuffer GlobalUbo : register(b0)
{
    float4x4 projection;
    float4x4 view;
    float4x4 invView;
    float4 ambientLightColor; // w is intensity

    PointLight pointLights[10];
    int numLights;
    float3 _padding; // Padding for alignment
};

// Cube map sampler
TextureCube skybox : register(t0);   // Texture bound at register t0
SamplerState samplerSkybox : register(s0); // Sampler bound at s0

// Input from vertex shader
struct PSInput
{
    float4 pos : SV_POSITION;
    float3 texcoord : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    return skybox.Sample(samplerSkybox, input.texcoord);
}
