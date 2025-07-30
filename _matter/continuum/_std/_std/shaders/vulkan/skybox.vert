#version 450

layout(location = 0) in vec3 aPos; // Skybox positions
layout(location = 1) in vec3 aNormal;   // NEW: normal attribute
layout(location = 2) in vec2 uv;   // NEW: uv attribute

layout(location = 0) out vec3 TexCoords; // Pass to fragment shader

struct PointLight {
	vec4 position;
	vec4 color;
	};


layout(set = 0, binding = 0) uniform GlobalUbo {
  mat4 projection;
  mat4 view;
  mat4 invView;
  vec4 ambientLightColor; // w is intensity
  PointLight pointLights[10];
  int numLights;
} ubo;

void main() {

    //TexCoords = vec3(aPos.x, -aPos.y, aPos.z); // Flip Y

    TexCoords = vec3(aPos.x, aPos.y, -aPos.z); // Flip Y

    // Remove translation from the view matrix to keep the skybox centered around the camera
    mat4 viewNoTranslation = mat4(mat3(ubo.view)); 
    vec4 pos = ubo.projection * viewNoTranslation * vec4(aPos, 1.0);
    
    gl_Position = pos.xyww; // Force depth to 1.0 by copying w to z (z/w = 1)
}
