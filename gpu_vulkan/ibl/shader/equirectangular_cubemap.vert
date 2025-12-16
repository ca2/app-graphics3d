#version 450

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUv;

layout(location = 0) out vec3 modelCoordinates;

// Push constant block (no set, no binding)
layout(push_constant) uniform PushConstants {
    mat4 mvp;
} pc;

void main()
{
    gl_Position = pc.mvp * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    modelCoordinates = aPos;
}
