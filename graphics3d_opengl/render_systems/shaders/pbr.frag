#version 330 core

#define PI 3.1415926535897932384626433832795
#define GREYSCALE_WEIGHT_VECTOR vec3(0.2126, 0.7152, 0.0722)

layout (location = 0) out vec4 FragColor; // regular output
layout (location = 1) out vec4 BloomColor; // output to be used by bloom shader

// vertex attributes
in vec3 worldCoordinates;
in vec2 textureCoordinates;
in vec3 tangent;
in vec3 bitangent;
in vec3 normal;


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
	 vec3 cameraPosition;
    PointLight pointLights[10];
    int numLights; // Needs to be padded to 16 bytes in std140 layout
    // Add padding to align to 16 bytes
    int padding1;
    int padding2;
    int padding3;
};


//struct PushConstants {
  uniform bool useTextureAlbedo;
  uniform bool useTextureMetallicRoughness;
  uniform bool useTextureNormal;
  uniform bool useTextureAmbientOcclusion;
  uniform bool useTextureEmissive;

  uniform vec3 albedo;
  uniform float metallic;
  uniform float roughness;
  uniform float ambientOcclusion;
  uniform vec3 emissive;

  uniform sampler2D textureAlbedo;
  uniform sampler2D textureMetallicRoughness;
  uniform sampler2D textureNormal;
  uniform sampler2D textureAmbientOcclusion;
  uniform sampler2D textureEmissive;
//};

//uniform PushConstants pushConsts;


// lights
//uniform vec3 lightPositions[4];
//uniform vec3 lightColors[4];

// PBR
// IBL precomputed maps
const float PREFILTERED_ENV_MAP_LOD = 4.0; // how many mipmap levels

uniform samplerCube diffuseIrradianceMap;
uniform samplerCube prefilteredEnvMap;
uniform sampler2D brdfConvolutionMap;

uniform float x_multiplier;
uniform float y_multiplier;
uniform float z_multiplier;

// Post parameters
uniform float bloomBrightnessCutoff;

// Fresnel function (Fresnel-Schlick approximation)
//
// F_schlick = f0 + (1 - f0)(1 - (h * v))^5
//
vec3 fresnelSchlick(float cosTheta, vec3 f0) { 
	return f0 + (1.0 - f0) * pow(max(1 - cosTheta, 0.0), 5.0);
}

// Fresnel schlick roughness
//
// Same as above except with a roughness term
vec3 fresnelSchlickRoughness(float cosTheta, vec3 f0, float roughness)
{
	return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// Normal distribution function (Trowbridge-Reitz GGX)
//
//                alpha ^ 2
//     ---------------------------------
//      PI((n * h)^2(alpha^2 - 1) + 1)^2
//
float ndfTrowbridgeReitzGGX(vec3 n, vec3 h, float roughness) {
	
	float alpha = roughness * roughness; // recommended by disney/epic papers
	float alphaSquared = alpha * alpha;

	float nDotH = max(dot(n, h), 0.0);
	float nDotHSquared = nDotH * nDotH;
	float innerTerms = nDotHSquared * (alphaSquared - 1.0) + 1.0;

	float numerator = alphaSquared;
	float denomenator = PI * innerTerms * innerTerms;
	denomenator = max(denomenator, 0.0001); // avoid div by zero

	return numerator / denomenator;
}

// Geometry function
//
//         n * v
//   -------------------
//   (n * v)(1 - k) + k
//
float geometrySchlickGGX(vec3 n, vec3 v, float k) {

	float nDotV = max(dot(n, v), 0.0);

	float numerator = nDotV;
	float denomenator = nDotV * (1.0 - k) + k;

	return numerator / denomenator;
}

// smiths method for taking into account view direction and light direction
float geometrySmith(vec3 n, vec3 v, vec3 l, float roughness) {

    // remapping for direct lighting (doesn't work for IBL)
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;

	return geometrySchlickGGX(n, v, k) * geometrySchlickGGX(n, l, k);
}

// Tangent space to world
vec3 calculateNormal(vec3 tangentNormal) {
	vec3 norm = normalize(tangentNormal * 2.0 - 1.0);
	mat3 TBN  = mat3(tangent, bitangent, normal);
	return normalize(TBN * norm); // tangent --> world
}

// ACES Filmic Tone Mapping (Narkowicz 2015)
vec3 ACESFilm(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

// gamma correction
vec3 toSRGB(vec3 x) {
    return pow(x, vec3(1.0 / 2.2));
}
vec3 linearToSrgb(vec3 c) { return pow(c, vec3(1.0 / 2.2)); }
vec3 srgbToLinear(vec3 c) { return pow(c, vec3(2.2)); }

void main() {
	// retrieve all the material properties
   vec2 uv = textureCoordinates;

   uv.y = 1.0 - uv.y; 

	// albedo
	vec3 albedo = albedo;
	if (useTextureAlbedo) {
		albedo = texture(textureAlbedo, uv).rgb;
	}

	// metallic/roughness
	float metallic = metallic;
	float roughness = roughness;
	if (useTextureMetallicRoughness) {
		vec3 metallicRoughness = texture(textureMetallicRoughness, uv).rgb;
		metallic = metallicRoughness.b;
		roughness = metallicRoughness.g;
	}

	// normal
	vec3 n = normal; // interpolated vertex normal
	if (useTextureNormal) {
		n = calculateNormal(texture(textureNormal, uv).rgb);
	}

	// ambient occlusion
	float ao = ambientOcclusion;
	if (useTextureAmbientOcclusion) {
		ao = texture(textureAmbientOcclusion, uv).r;
	}

	// emissive
	vec3 emissive = emissive;
	if (useTextureEmissive) {
		emissive = texture(textureEmissive, uv).rgb;
	}

	vec3 v = normalize(cameraPosition - worldCoordinates); // view vector pointing at camera
   //vec3 v = normalize(worldCoordinates - cameraPosition); // view vector pointing at camera

   //vec3 N = normalize(n);


	float NdotV = max(dot(n, v), 0.0);


	vec3 r = reflect(-v, n); // reflection

   r.x *= x_multiplier;
   r.y *= y_multiplier;
   r.z *= z_multiplier;



	// f0 is the "surface reflection at zero incidence"
	// for PBR-metallic we assume dialectrics all have 0.04
	// for metals the value comes from the albedo map
	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, metallic);

	vec3 Lo = vec3(0.0); // total radiance out

	// Direct lighting
	// Sum up the radiance contributions of each light source.
	// This loop is essentially the integral of the rendering equation.
	for (int i = 0; i < 4; i++) {
		vec3 l = normalize(pointLights[i].position.xyz  - worldCoordinates); // light vector
		vec3 h = normalize(v + l);

		float distance = length(pointLights[i].position.xyz  - worldCoordinates);
		float attenuation = 1.0 / (distance * distance); // inverse square law
		vec3 radiance = pointLights[i].color.rgb * attenuation; // aka Li

		// calculate Cook-Torrance specular BRDF term
		//
		//                DFG
		//        --------------------
		//         4(w_0 * n)(w_i * n)
		//
		//

		// Normal Distribution term (D)
		float dTerm = ndfTrowbridgeReitzGGX(n, h, roughness);

		// Fresnel term (F)
		// Determines the ratio of light reflected vs. absorbed
		vec3 fTerm = fresnelSchlick(max(dot(h, v), 0.0), f0);

		// Geometry term (G)
		float gTerm = geometrySmith(n, v, l, roughness);


		vec3 numerator = dTerm * fTerm * gTerm;
		float denominator = 4.0 * max(dot(v, n), 0.0) * max(dot(l, n), 0.0);
		
		// recall fTerm is the proportion of reflected light, so the result here is the specular
		vec3 specular = numerator / max(denominator, 0.001);

		vec3 kSpecular = fTerm;
		vec3 kDiffuse = vec3(1.0) - kSpecular;
		kDiffuse *= 1.0 - metallic; // metallic materials should have no diffuse component

		// now calculate full Cook-Torrance with both diffuse + specular
		//
		// f_r = kd * f_lambert + ks * f_cook-torrance
		//
		// where f_lambert = c / pi

		vec3 diffuse = kDiffuse * albedo / PI;
		vec3 cookTorranceBrdf = diffuse + specular;
		float nDotL = max(dot(n, l), 0.0);

		// Finally, the rendering equation!
		Lo += cookTorranceBrdf * radiance * nDotL;
	}

	// Indirect lighting (IBL)
	vec3 kSpecular = fresnelSchlickRoughness(NdotV, f0, roughness); // aka F
   vec3 kDiffuse = 1.0 - kSpecular;
	kDiffuse *= 1.0 - metallic; // metallic materials should have no diffuse component

	// diffuse
   vec3 irradiance = texture(diffuseIrradianceMap, n).rgb;
   vec3 diffuse = irradiance * albedo;

	// specular
	vec3 prefilteredEnvMapColor = textureLod(prefilteredEnvMap, r, roughness * PREFILTERED_ENV_MAP_LOD).rgb;
	vec2 brdf = texture(brdfConvolutionMap, vec2(NdotV, roughness)).rg;
	vec3 specular = prefilteredEnvMapColor * (kSpecular * brdf.x + brdf.y);
//vec3 specular = prefilteredEnvMapColor;
//vec3 specular = albedo;

	vec3 ambient = (kDiffuse * diffuse + specular) * ao; // indirect lighting
   //vec3 ambient = (kDiffuse * diffuse + specular); // indirect lighting
   //vec3 ambient = specular; // indirect lighting

	// Combine emissive + indirect + direct
	vec3 color = emissive + ambient + Lo;
   //vec3 color = ambient;

	// Outputs

	// main color output
   //FragColor = vec4(albedo, 1.0);
    const float EXPOSURE = 0.3;
    vec3 outColor = ACESFilm(color * EXPOSURE);
    outColor = toSRGB(outColor);
	FragColor = vec4(outColor, 1.0);

   //FragColor = vec4(texture(prefilteredEnvMap, r).rgb, 1.0);
//vec3 N = normalize(n);  // ignore normal map
//FragColor = vec4(n * 0.5 + 0.5, 1.0);
//fragColor = vec4(normalize(N) * 0.5 + 0.5, 1.0);

//if (n.y > 0.0)
//    FragColor = vec4(0.2, 1.0, 0.2, 1.0); // greenish for positive Y
//else
//    FragColor = vec4(1.0, 0.2, 0.6, 1.0); // magenta-ish for negative Y


//vec3 colA = texture(prefilteredEnvMap, r).rgb;
//vec3 colB = texture(prefilteredEnvMap, vec3(r.x * x_multiplier, r.y * y_multiplier, r.z * z_multiplier)).rgb;
//// show difference or one of them
//FragColor = vec4(colB, 1.0); // then try colB

	// bloom color output
	// use greyscale conversion here because not all colors are equally "bright"
    //float greyscaleBrightness = dot(FragColor.rgb, GREYSCALE_WEIGHT_VECTOR);
	//BloomColor = greyscaleBrightness > bloomBrightnessCutoff ? vec4(emissive, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);

// Fix bloom
float greyscaleBrightness = dot(FragColor.rgb, GREYSCALE_WEIGHT_VECTOR);
BloomColor = greyscaleBrightness > bloomBrightnessCutoff ? FragColor : vec4(0.0);
}



