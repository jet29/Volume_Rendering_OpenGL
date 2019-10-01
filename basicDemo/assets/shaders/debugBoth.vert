#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;
// Atributte 1 of the vertex
layout (location = 1) in vec3 vertexColor;

// Vertex data out data
out vec3 vColor;
out vec3 vPos;

void main()
{
    vColor = vertexColor;   
    gl_Position = vec4(vertexPosition, 1.0f);
    vPos = vertexPosition;
}