#version 450 core

layout(location = 0) in vec3 modelCoordinates;
layout(location = 0) out vec4 FragColor;

// cubemap sampler (adjust set/binding to match your descriptor layout)
layout(set = 0, binding = 0) uniform samplerCube environmentCubemap;

// option A (recommended for a single float): push constant
layout(push_constant) uniform PushConstants {
    float roughness;
};

// option B (alternative): small uniform buffer (uncomment and use instead of push constant)
// layout(set = 0, binding = 1) uniform Params { float roughness; };

const float PI = 3.14159265359;
const uint SAMPLE_COUNT = 1024u;
const float FACE_RESOLUTION = 512.0;

// this mirrors the number in binary around the decimal point
// aka return: a0 / 2 + a1 / 4 + a2 / 8 + ...
// source: http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html#sec-SourceCode
float radicalInverseVanDerCorput(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

vec2 hammersley(uint i, uint N)
{
    return vec2(float(i) / float(N), radicalInverseVanDerCorput(i));
}

vec3 importanceSampleGGX(vec2 unitSquareSample, vec3 N, float roughness) {
    float alpha = roughness * roughness;

    float phi = 2.0 * PI * unitSquareSample.x;
    float cosTheta = sqrt((1.0 - unitSquareSample.y) / (1.0 + (alpha * alpha - 1.0) * unitSquareSample.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sampleVector = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVector);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float numerator = a2;
    float denominator = (NdotH2 * (a2 - 1.0) + 1.0);
    denominator = PI * denominator * denominator;

    return numerator / denominator;
}

float getSampleMipLevel(vec3 V, vec3 N, vec3 H, float roughness) {
    float distribution = distributionGGX(N, H, roughness);
    float NdotH = max(dot(N, H), 0.0);
    float HdotV = max(dot(H, V), 0.0);
    float pdf = distribution * NdotH / (4.0 * HdotV) + 0.0001;

    float saTexel  = 4.0 * PI / (6.0 * FACE_RESOLUTION * FACE_RESOLUTION);
    float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

    return roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
}

void main() {
    vec3 N = normalize(modelCoordinates);
    vec3 V = N; // view direction aligned with normal (Epic Games approximation)

    float totalWeight = 0.0;
    vec3 outputColor = vec3(0.0);

    for(uint i = 0u; i < SAMPLE_COUNT; ++i) {
        vec2 unitSquareSample = hammersley(i, SAMPLE_COUNT);
        vec3 H = importanceSampleGGX(unitSquareSample, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0) {
            float mipLevel = getSampleMipLevel(V, N, H, roughness);
            outputColor += textureLod(environmentCubemap, L, mipLevel).rgb * NdotL;
            totalWeight += NdotL;
        }
    }

    outputColor /= max(totalWeight, 1e-6);
    FragColor = vec4(outputColor, 1.0);
}
