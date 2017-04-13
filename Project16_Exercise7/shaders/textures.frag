#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	// Linearly interpolate between both textures (second texture is only slightly combined)
	// Exercise 7 : 
	vec2 _TexCoord;
	_TexCoord.x = (-1.0)*TexCoord.x;
	_TexCoord.y = TexCoord.y;
	//color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, _TexCoord), 0.2);
	// Correction:
	color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.2);
}
