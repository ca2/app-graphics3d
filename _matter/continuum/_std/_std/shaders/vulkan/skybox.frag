#version 450

layout(location = 0) in vec3 TexCoords;
layout(location = 0) out vec4 FragColor;

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

layout(set = 1, binding = 0) uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, TexCoords);
}
