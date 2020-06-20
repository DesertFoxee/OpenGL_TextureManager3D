#version 430

layout(location =0 ) in vec3 position;
layout(location =1) in vec2 text_coord;


uniform mat4 mv_mat ;
uniform mat4 proj_mat;

out vec2 text_coord_in;


void main(void){ 
	gl_Position = proj_mat * mv_mat * vec4(position , 1.0); 

	text_coord_in = text_coord;
}