//skybox_ibl.frag
#version 450 core
layout(set = 1, binding = 0) uniform samplerCube skybox;

layout(location = 0) in vec3 vDirection;
layout(location = 0) out vec4 outColor;

// Push constants or UBO for camera + post params could be used; keep here as UBO-like values
// For simplicity we read cameraPosition from the GlobalUbo.viewPos (xyz) if you prefer.
// But keep dedicated uniform if you want:
layout(push_constant) uniform PushConsts 
{
    
    vec3 multiplier;

} pushConsts;



void main() {
    vec3 dir = normalize(vDirection);
    dir *= pushConsts.multiplier;
    outColor = texture(skybox, dir);
}