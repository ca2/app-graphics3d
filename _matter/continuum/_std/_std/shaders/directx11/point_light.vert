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

// Individual uniforms (in DirectX 12, typically bundled in a second constant buffer)
cbuffer LightParams : register(b1)
{
    float4 position;
    float4 color;
    float radius;
    float padding4;
    float padding5;
    float padding6;
}

// Output structure to the pixel shader
struct VSOut
{
    float4 pos : SV_POSITION;
    float2 fragOffset : TEXCOORD0;
};

// Vertex shader entry point
VSOut main(uint vertexID : SV_VertexID)
{
    VSOut output;

    // OFFSETS[6] hardcoded as in OpenGL
    static const float2 OFFSETS[6] = {
        float2(-1.0f, -1.0f),
        float2(-1.0f,  1.0f),
        float2( 1.0f, -1.0f),
        float2( 1.0f, -1.0f),
        float2(-1.0f,  1.0f),
        float2( 1.0f,  1.0f)
    };

    float2 fragOffset = OFFSETS[vertexID];
    output.fragOffset = fragOffset;

    // Extract right and up vectors from the view matrix
    float3 cameraRightWorld = float3(view._11, view._21, view._31); // column 0
    float3 cameraUpWorld    = float3(view._12, view._22, view._32); // column 1

    float3 positionWorld =
        position.xyz +
        radius * fragOffset.x * cameraRightWorld +
        radius * fragOffset.y * cameraUpWorld;

    float4 posClip = mul(float4(positionWorld, 1.0f), mul(view, projection));
    output.pos = posClip;

    return output;
}
