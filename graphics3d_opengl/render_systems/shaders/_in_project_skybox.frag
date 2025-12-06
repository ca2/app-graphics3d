//skybox.frag
#version 330 core
 
in vec3 TexCoords;
out vec4 FragColor;
 
uniform samplerCube skybox;
 
void main()
{
    vec4 color;
    // Simple lookup in the cube map
  //  if(TexCoords.x > 0.5)
    //{

//        color= vec4(0.2,0.5,0.35,0.5);

    //}
    //else
    //{
        color = texture(skybox, TexCoords);

    //}
    //color = float4(1, 0, 1, 1); // Pink test color
    FragColor = vec4(color);
}