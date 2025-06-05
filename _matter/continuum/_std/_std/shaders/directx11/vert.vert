// Input structure from vertex buffer
struct VS_INPUT {
    float3 position : POSITION;
    float3 color    : COLOR;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD0;
};

// Output structure to pixel shader
struct VS_OUTPUT {
    float4 pos             : SV_POSITION;
    float3 fragColor       : COLOR0;
    float3 fragPosWorld    : TEXCOORD1;
    float3 fragNormalWorld : TEXCOORD2;
};

// Match GLSL std140 layout (float4-aligned)
struct PointLight {
    float4 position;
    float4 color;
};

// Uniform buffer: GlobalUbo (binding slot b0)
cbuffer GlobalUbo : register(b0)
{
    float4x4 projection;
    float4x4 view;
    float4x4 invView;
    float4 ambientLightColor;
    PointLight pointLights[10];
    int numLights;
    int padding1;
    int padding2;
    int padding3;
};

// Separate uniforms (can also be placed in their own cbuffer if needed)
cbuffer ObjectMatrices : register(b1)
{
    float4x4 modelMatrix;
    float4x4 normalMatrix; // Should be precomputed as the transpose(inverse(modelMatrix))
};

// Main vertex shader
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 positionWorld = mul(float4(input.position, 1.0f), modelMatrix);      // model * vec
    output.pos = mul(mul(positionWorld, view), projection);                    // projection * view * model * vec
    output.fragNormalWorld = normalize(mul((float3x3)normalMatrix, input.normal));
    output.fragPosWorld = positionWorld.xyz;
    output.fragColor = input.color;

    return output;
}
