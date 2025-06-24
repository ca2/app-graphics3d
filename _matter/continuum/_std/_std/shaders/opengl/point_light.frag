#version 420

layout (location = 0) in vec2 fragOffset;
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

uniform vec4 position;
uniform vec4 color;
uniform float radius;



void main() {
  float dis = sqrt(dot(fragOffset, fragOffset));
  if (dis >= 1.0) {
    discard;
  }
  vec4 oColor = vec4(color.xyz, 1.0);
  if (oColor.a < 0.01)
        discard; // Kill the fragment — no color, no depth
  outColor=oColor;
}