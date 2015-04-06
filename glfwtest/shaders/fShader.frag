#version 330 core
in vec3 vertexColor;

out vec4 exportColor;

void main()
{
   exportColor = vec4(vertexColor,1.0f);
}
