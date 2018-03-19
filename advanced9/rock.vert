#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoords;
layout (location = 2) in mat4 instanceMatrix;

out vec2 TextCoords;

uniform mat4 projection;
uniform mat4 view;

void main(){
	TextCoords = aTextCoords;
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0); 
}