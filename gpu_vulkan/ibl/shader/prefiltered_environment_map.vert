#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUv;


layout (location = 0) out vec3 modelCoordinates;

out gl_PerVertex {
	vec4 gl_Position;
};


// Push constant block (no set, no binding)
layout(push_constant) uniform PushConstants 
{
	mat4 mvp;
	float roughness;
	int numSamples;
} pc;


void main() 
{
    gl_Position = pc.mvp * vec4(inPos.x, inPos.y, inPos.z, 1.0);
    modelCoordinates = inPos;
}
