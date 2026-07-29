#version 450

layout(location = 0) in vec3 TexCoords;
layout(location = 0) out vec4 FragColor;

struct PointLight {
	vec4 position;
	vec4 color;
	};

// ---------- Global UBO (set 0 binding 0) ----------
layout(std140, set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
    vec3 cameraPosition;
    // pointLights array
    PointLight pointLights[10];
    int numLights;
    int padding1;
    int padding2;
    int padding3;
} globalUbo;

layout(set = 1, binding = 0) uniform samplerCube skybox;

void main() {

    vec3 tc = TexCoords;

    //tc.y = -tc.y;
    tc.z = -tc.z;

    FragColor = texture(skybox, tc);
}
