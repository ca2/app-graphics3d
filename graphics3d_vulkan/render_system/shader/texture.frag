#version 450

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;


struct PointLight
{
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
} globalUbo;



layout(push_constant) uniform Push {
  mat4 modelMatrix;
  mat4 normalMatrix;
} push;


layout(set = 1, binding = 0) uniform sampler2D textureSampler;


void main()
{
    if (gl_FrontFacing)
    {

    outColor = vec4(1.0, 1.0, 1.0, 1.0);
    
        
    }
    else
    {
        
    outColor = texture(textureSampler, fragTexCoord);
    }
}