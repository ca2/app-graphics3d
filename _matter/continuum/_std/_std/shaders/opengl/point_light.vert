#version 420

const vec2 OFFSETS[6] = vec2[](
  vec2(-1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, -1.0),
  vec2(1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, 1.0)
);

layout (location = 0) out vec2 fragOffset;


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

uniform vec4 position;
uniform vec4 color;
uniform float radius;


const float LIGHT_RADIUS = 0.05;

void main() {
 fragOffset = OFFSETS[gl_VertexID];
  vec3 cameraRightWorld = {view[0][0], view[1][0], view[2][0]};
  vec3 cameraUpWorld = {view[0][1], view[1][1], view[2][1]};

  vec3 positionWorld = position.xyz
    + radius * fragOffset.x * cameraRightWorld
    + radius * fragOffset.y * cameraUpWorld;

  vec4 posClip = projection * view * vec4(positionWorld, 1.0);
  gl_Position = posClip;
}