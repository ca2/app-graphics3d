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
    float3 _padding; // Padding to align to 16-byte boundary
};

struct VSInput {
    float3 position : POSITION;
    float3 normalNotUsed : NORMAL;
    float2 uvNotUsed : TEXCOORD0;
};

struct VSOutput {
    float4 pos      : SV_POSITION;
    float3 texcoord : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    output.texcoord = input.position;

    // Remove translation from view matrix
    float3x3 viewRot = (float3x3)view;
    float4x4 viewNoTranslation = float4x4(
        float4(viewRot[0], 0.0),
        float4(viewRot[1], 0.0),
        float4(viewRot[2], 0.0),
        float4(0.0, 0.0, 0.0, 1.0)
    );

    float4 worldPos = float4(input.position, 1.0);
    float4 clipPos = mul(projection, mul(viewNoTranslation, worldPos));

    // Force depth to 1.0 (z = w)
    output.pos = float4(clipPos.xy, clipPos.w, clipPos.w);

    return output;
}