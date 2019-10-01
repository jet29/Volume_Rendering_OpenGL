#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;
in vec3 vPos;

uniform sampler3D texture1;

// Fragment Color
out vec4 color;

void main()
{
    float texCordX = (vPos.x + 1)/2;
    float texCordY = (vPos.y + 1)/2;
    float texCordZ = 127.0f/256.0f;

    vec3 texPos = vec3(texCordX, texCordY, texCordZ);

    color = texture3D(texture1,texPos);
    //color = vec4(1,0,0,0);
}