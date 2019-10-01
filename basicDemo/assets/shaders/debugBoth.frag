#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;
in vec3 vPos;

uniform sampler2D texture2;
uniform sampler3D texture1;

// Fragment Color
out vec4 color;

void main()
{
    bool show3D = false;

    float texCordX = (vPos.x + 1)/2;
    float texCordY = (vPos.y + 1)/2;
    float texCordZ = 127.0f/256.0f;

    vec3 texPos = vec3(texCordX, texCordY, texCordZ);

    vec2 texPos2 = vec2(texCordX, texCordY);


    if(show3D){
        color = texture3D(texture1,texPos);
    }
    else{
        color = texture2D(texture2,texPos2);
    }

    //color = vec4(1,0,0,0);
}