#version 430
in vec2 text_coord_in;
out vec4 color;

// glActiveTexture(0) => texture 2D

layout(binding = 0) uniform sampler2D samp;

void main(void){ 
	color = texture(samp , text_coord_in) ;

}