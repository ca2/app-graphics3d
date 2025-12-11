// This file was automatically generated from a command line like:
// application_build_helper --inline-text "scene_frag.frag"
#pragma once


const char g_psz_scene_frag_frag[] = R"frag_text(#version 330 core


in vec3 fragNormal;
in vec4 fragColor; 
in vec2 fragUV; 
in vec3 fragViewVec; 
in vec3 fragWorldPos; 
in vec4 fragTangent;

out vec4 outFragColor;

struct PointLight {
	vec4 position;
	vec4 color;
	};


// Must match the same binding point as in your C++ side using glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboBuffer)
layout(std140) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
	 vec4 viewPos;
    PointLight pointLights[10];
    int numLights; // Needs to be padded to 16 bytes in std140 layout
    // Add padding to align to 16 bytes
    int padding1;
    int padding2;
    int padding3;
};


struct Material {
  //bool useTextureAlbedo;
  //bool useTextureMetallicRoughness;
  bool useTextureNormal;
  //bool useTextureAmbientOcclusion;
  //bool useTextureEmissive;

  //vec3 albedo;
  //float metallic;
  //float roughness;
  //float ambientOcclusion;
  //vec3 emissive;

  sampler2D textureAlbedo;
  //sampler2D textureMetallicRoughness;
  sampler2D textureNormal;
  //sampler2D textureAmbientOcclusion;
  //sampler2D textureEmissive;
};

uniform Material material;

uniform vec3 cameraPosition;


void main() {

    vec4 texColor = texture(material.textureAlbedo, fragUV) * fragColor;

    //if (ALPHA_MASK && texColor.a < ALPHA_MASK_CUTOFF) {
      //  discard;
   // }

    // Normal mapping
////////////////

    // Normal mapping (conditional)
    vec3 N = normalize(fragNormal);
    if (material.useTextureNormal) {
        vec3 T = normalize(fragTangent.xyz);
        vec3 B = cross(N, T) * fragTangent.w;
        mat3 TBN = mat3(T, B, N);
        vec3 sampledNormal = texture(material.textureNormal, fragUV).xyz * 2.0 - 1.0;
        N = normalize(TBN * sampledNormal);
    }

/////////////////

    vec3 V = normalize(fragViewVec);

    vec3 ambient = ambientLightColor.rgb * texColor.rgb * ambientLightColor.a;
    vec3 lighting = ambient;

    for (int i = 0; i < numLights; ++i) {
        vec3 L = normalize(pointLights[i].position.xyz - fragWorldPos);
        vec3 R = reflect(-L, N);
        vec3 lightCol = pointLights[i].color.rgb * pointLights[i].color.a;

        float NdotL = max(dot(N, L), 0.0);
        float RdotV = pow(max(dot(R, V), 0.0), 32.0);

        vec3 diffuse  = texColor.rgb * NdotL * lightCol;
        vec3 specular = RdotV * lightCol;

        lighting += diffuse + specular;
    }

    outFragColor = vec4(lighting, texColor.a);

})frag_text";




