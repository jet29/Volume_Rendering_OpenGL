#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;
in vec3 vPos;

uniform sampler2D texture1;

// Fragment Color
out vec4 color;

void main()
{
    float texCordX = (vPos.x + 0.5f)/2;
    float texCordY = (vPos.y + 0.5f)/2;


    vec2 texPos = vec2(texCordX, texCordY);

    color = texture(texture1,texPos);
    //color = vec4(1,0,0,0);
}