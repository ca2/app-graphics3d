#version 450

layout(location = 0) in vec2 textureCoordinates;
layout(location = 0) out vec2 FragColor;

const float PI = 3.14159265359;
const uint SAMPLE_COUNT = 1024u;

// mirrors the number in binary around the decimal point
float radicalInverseVanDerCorput(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

// Hammersley sequence (low-discrepancy)
vec2 hammersley(uint i, uint N)
{
    return vec2(float(i) / float(N), radicalInverseVanDerCorput(i));
}

vec3 importanceSampleGGX(vec2 unitSquareSample, vec3 N, float roughness)
{
    float alpha = roughness * roughness;

    float phi = 2.0 * PI * unitSquareSample.x;
    float cosTheta = sqrt((1.0 - unitSquareSample.y) / (1.0 + (alpha * alpha - 1.0) * unitSquareSample.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sampleVector = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVector);
}

// Geometry term (Schlick-GGX)
float geometrySchlickGGX(vec3 n, vec3 v, float k)
{
    float nDotV = max(dot(n, v), 0.0);
    float numerator = nDotV;
    float denomenator = nDotV * (1.0 - k) + k;
    return numerator / denomenator;
}

float geometrySmith(vec3 n, vec3 v, vec3 l, float roughness)
{
    float k = (roughness * roughness) / 2.0;
    return geometrySchlickGGX(n, v, k) * geometrySchlickGGX(n, l, k);
}

void main()
{
    float NdotV = textureCoordinates.x;
    float roughness = textureCoordinates.y;

    vec3 N = vec3(0.0, 0.0, 1.0);
    vec3 V;
    V.x = sqrt(max(0.0, 1.0 - NdotV * NdotV));
    V.y = 0.0;
    V.z = NdotV;

    float F0Scale = 0.0;
    float F0Bias  = 0.0;

    for (uint i = 0u; i < SAMPLE_COUNT; ++i) {
        vec2 unitSquareSample = hammersley(i, SAMPLE_COUNT);
        vec3 H = importanceSampleGGX(unitSquareSample, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if (NdotL > 0.0) {
            float G = geometrySmith(N, V, L, roughness);
            float GVis = (G * VdotH) / (NdotH * NdotV);

            float partialFresnel = pow(1.0 - VdotH, 5.0);

            F0Scale += GVis * (1.0 - partialFresnel);
            F0Bias  += GVis * partialFresnel;
        }
    }

    F0Scale /= float(SAMPLE_COUNT);
    F0Bias  /= float(SAMPLE_COUNT);

    FragColor = vec2(F0Scale, F0Bias);
}
