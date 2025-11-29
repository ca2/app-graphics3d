#version 420

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;


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
    PointLight pointLights[10];
    int numLights; // Needs to be padded to 16 bytes in std140 layout
    // Add padding to align to 16 bytes
    int padding1;
    int padding2;
    int padding3;
};
 
// Instead of push constants, use a second uniform block or separate uniforms
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main() {

  
  // Test if the fragment is on the left side in world space
  //if (fragPosWorld.x < 0.0) {
    //outColor = vec4(0.5*0.5, 0.75*0.5, 0.95*0.5, .5);
    //return;
  //}
 vec3 diffuseLight = ambientLightColor.xyz * ambientLightColor.w;
  vec3 specularLight = vec3(0.0);
  vec3 surfaceNormal = normalize(fragNormalWorld);

  vec3 cameraPosWorld = invView[3].xyx;
  vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld);

  for (int i = 0; i < numLights; i++) {
    PointLight light = pointLights[i];
    vec3 directionToLight = light.position.xyz - fragPosWorld;
    float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
    directionToLight = normalize(directionToLight);

    float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
    vec3 intensity = light.color.xyz * light.color.w * attenuation;

    diffuseLight += intensity * cosAngIncidence;

    vec3 halfAngle = normalize(directionToLight + viewDirection);
    float blinnTerm = dot(surfaceNormal, halfAngle);
    blinnTerm = clamp(blinnTerm, 0, 1);
    blinnTerm = pow(blinnTerm, 500.0);
    specularLight += intensity * blinnTerm;
  }
  
  vec4 oColor = vec4(diffuseLight * fragColor + specularLight * fragColor, 1.0);
  if (oColor.a < 0.01)
        discard; // Kill the fragment — no color, no depth
  outColor=oColor;
}