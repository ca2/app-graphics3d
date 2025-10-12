#version 450

layout(location = 0) in vec3 aPos;

layout(location = 0) out vec3 modelCoordinates;

// Push constant block (no set, no binding)
layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 view;
    mat4 projection;
} pc;

void main()
{
    gl_Position = pc.projection * pc.view * pc.model * vec4(aPos, 1.0);
    modelCoordinates = aPos;
}
