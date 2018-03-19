#version 330 core
out vec4 FragColor;

in vec2 TextCoords;

uniform sampler2D texture_diffuse1;

void main(){
    FragColor = texture(texture_diffuse1, TextCoords);
}
