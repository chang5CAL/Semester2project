#version 330 core
in vec3 vertexColor;
in vec2 texExCoord;

out vec4 exportColor;

uniform sampler2D ourTexture;

void main()
{
   exportColor = texture(ourTexture,texExCoord) * vec4(vertexColor,1.0f);
}
