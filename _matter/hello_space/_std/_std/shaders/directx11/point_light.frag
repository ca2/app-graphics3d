cbuffer GlobalUbo : register(b0)
{
    float4x4 projection;
    float4x4 view;
    float4x4 invView;
    float4 ambientLightColor;

    struct PointLight {
        float4 position;
        float4 color;
    };

    PointLight pointLights[10];
    int numLights;
    int padding1;
    int padding2;
    int padding3;
};

// Matching the same buffer used in the vertex shader
cbuffer LightParams : register(b1)
{
    float4 position;
    float4 color;
    float radius;
    float padding4;
    float padding5;
    float padding6;
}

// Input from vertex shader
struct PSIn
{
    float4 pos : SV_POSITION;
    float2 fragOffset : TEXCOORD0;
};

// Output
float4 main(PSIn input) : SV_TARGET
{
    float dis = sqrt(dot(input.fragOffset, input.fragOffset));
    if (dis >= 1.0f)
        discard;

    float4 oColor = float4(color.rgb, 1.0f);
    if (oColor.a < 0.01f)
        discard;

    return oColor;
}
