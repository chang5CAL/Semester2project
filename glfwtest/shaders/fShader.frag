#version 330 core
in vec3 vertexColor;
in vec2 texExCoord;

out vec4 exportColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
   exportColor = mix((texture(ourTexture1,texExCoord)*vec4(vertexColor,1.0f)),
			texture(ourTexture2,texExCoord),0.2);
}
