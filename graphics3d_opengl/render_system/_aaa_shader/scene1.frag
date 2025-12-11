//
// scene.frag OpenGL
//

#version 330 core


in vec3 fragmentWorldCoordinate; 
in vec3 fragmentNormal;
in vec2 fragmentTextureCoordinate; 
in vec4 fragmentColor; 
in vec3 fragmentTangent;
in vec3 fragmentBitangent;

out vec4 outputColor;

struct PointLight {
	vec4 position;
	vec4 color;
	};


layout(std140) uniform GlobalUbo 
{
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
	vec3 cameraPosition;
    PointLight pointLights[10];
    int numLights;
};


//struct PushConsts 
//{

  //bool useTextureAlbedo;
  //bool useTextureNormal;

  //vec3 albedo;


//      mat4 modelMatrix;
//    mat4 normalMatrix; // inverse-transpose of model

    // booleans promoted to ints (std140 rules); use 0/1 in C++ when updating

uniform     int useTextureAlbedo;
uniform     int useTextureNormal;
    //int useTextureMetallicRoughness;
    //int useTextureAmbientOcclusion;
    //int useTextureEmissive;
uniform     int useAlphaMask;

uniform     vec3 albedo;
uniform     float metallic;
uniform     float roughness;
uniform     float ambientOcclusion;
uniform     vec3 emissive;
uniform     float alphaMaskCutoff;

    //vec3 cameraPosition;
uniform     float bloomBrightnessCutoff;
uniform     vec3 multiplier;



uniform   sampler2D textureAlbedo;
  uniform  sampler2D textureNormal;

//};

//uniform PushConsts pushConsts;

void main() 
{

    vec4 texColor;
    if(useTextureAlbedo != 0)
    {
    
        texColor = texture(textureAlbedo, fragmentTextureCoordinate) * fragmentColor;

    }
    else
    {

        texColor = vec4(albedo, 1.0) * fragmentColor;

    }

    if (useAlphaMask != 0 && texColor.a < alphaMaskCutoff)
    {
        discard;
    }

    
    // Normal mapping (conditional)
    vec3 N = normalize(fragmentNormal);
    
    if (useTextureNormal != 0)
    {
        mat3 TBN = mat3(fragmentTangent, fragmentBitangent, fragmentNormal);
        vec3 sampledNormal = texture(textureNormal, fragmentTextureCoordinate).xyz * 2.0 - 1.0;
        N = normalize(TBN * sampledNormal);
    }

    //vec3 V = normalize(fragViewVec);

    //vec3 V = normalize(inViewVec);

    vec3 seq3CameraPosition = cameraPosition;

    vec3 V = normalize(seq3CameraPosition - fragmentWorldCoordinate);


    vec3 ambient = ambientLightColor.rgb * texColor.rgb * ambientLightColor.a;
    vec3 lighting = ambient;

    for (int i = 0; i < numLights; ++i) {
        vec3 L = normalize(pointLights[i].position.xyz - fragmentWorldCoordinate);
        vec3 R = reflect(-L, N);
        vec3 lightCol = pointLights[i].color.rgb * pointLights[i].color.a;

        float NdotL = max(dot(N, L), 0.0);
        float RdotV = pow(max(dot(R, V), 0.0), 32.0);

        vec3 diffuse  = texColor.rgb * NdotL * lightCol;
        vec3 specular = RdotV * lightCol;

        lighting += diffuse + specular;
    }

    outputColor = vec4(lighting, texColor.a);

}