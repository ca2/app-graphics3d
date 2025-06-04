// Input from vertex shader
struct PS_INPUT {
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


// Output
float4 main(PS_INPUT input) : SV_TARGET
{
    float3 diffuseLight = ambientLightColor.rgb * ambientLightColor.a;
    float3 specularLight = float3(0.0, 0.0, 0.0);
    float3 surfaceNormal = normalize(input.fragNormalWorld);

    // Extract camera position from invView matrix (row-major)
    float3 cameraPosWorld = float3(invView._41, invView._42, invView._41);
    float3 viewDirection = normalize(cameraPosWorld - input.fragPosWorld);

    for (int i = 0; i < numLights; ++i)
    {
        PointLight light = pointLights[i];
        float3 directionToLight = light.position.xyz - input.fragPosWorld;
        float attenuation = 1.0 / dot(directionToLight, directionToLight);
        directionToLight = normalize(directionToLight);

        float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0.0f);
        float3 intensity = light.color.rgb * light.color.a * attenuation;

        diffuseLight += intensity * cosAngIncidence;

        float3 halfAngle = normalize(directionToLight + viewDirection);
        float blinnTerm = dot(surfaceNormal, halfAngle);
        blinnTerm = saturate(blinnTerm);
        blinnTerm = pow(blinnTerm, 500.0f);

        specularLight += intensity * blinnTerm;
    }

    float4 oColor = float4(diffuseLight * input.fragColor + specularLight * input.fragColor, 1.0f);
    clip(oColor.a < 0.01f ? -1.0f : 1.0f); // discard fragment if alpha < 0.01

    return oColor;
}
