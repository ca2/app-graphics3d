struct PointLight {
    float4 position;
    float4 color;
};

cbuffer GlobalUbo : register(b0) {
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

struct VertexInput {
    float3 position : POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD0;
};

struct VertexOutput {
    float4 position : SV_POSITION;
    float3 texCoords : TEXCOORD0;
};

VertexOutput main(VertexInput input) {
    VertexOutput output;
    
    // Pass position directly as texture coordinates for skybox
    output.texCoords = float3(input.position.x, input.position.y, -input.position.z);
    //output.texCoords = input.position;
    
    // Since matrices are transposed on C++ side, we need to transpose them back
    // or use the opposite multiplication order
    //float4x4 projectionT = transpose(projection);
    //float4x4 viewT = transpose(view);
    float4x4 projectionT =transpose(projection);
    float4x4 viewT = transpose(view);
    
    // Extract only the rotation part from the transposed view matrix
    float4x4 viewRotation = float4x4(
        viewT[0].x, viewT[0].y, viewT[0].z, 0,
        viewT[1].x, viewT[1].y, viewT[1].z, 0,
        viewT[2].x, viewT[2].y, viewT[2].z, 0,
        0, 0, 0, 1
    );
    
    // Now multiply in the same order as OpenGL: projection * viewRotation * position
    float4 pos = mul(projectionT, mul(viewRotation, float4(input.position, 1.0)));
    
    // Keep the z/w ratio 1:1 for skybox (equivalent to gl_Position.xyww)
    output.position = pos.xyww;
    
    return output;
}