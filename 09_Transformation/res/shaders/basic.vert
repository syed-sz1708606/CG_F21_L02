#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 transform; 

void main()
{
	gl_Position = transform * vec4(pos.x, pos.y, pos.z, 1.0);
	TexCoord = texCoord;

	//method 1 to flip texture 
	TexCoord = vec2(TexCoord.x,1.0 -TexCoord.y);
}