#version 330 core
in vec4 objectColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D _Texture1;

void main()
{
	FragColor = texture(_Texture1, TexCoord) * objectColor;
}