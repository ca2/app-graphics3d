//skybox.frag
#version 330 core
 
in vec3 TexCoords;
out vec4 FragColor;
 
uniform samplerCube skybox;
 
void main()
{
    // Simple lookup in the cube map
    vec4 color = texture(skybox, TexCoords);
    FragColor = vec4(color);
}