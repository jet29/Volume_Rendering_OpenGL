#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vColor;
in vec3 vPos;

// Fragment Color
out vec4 color;

void main()
{
    color = vec4(vPos,0);
}