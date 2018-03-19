#version 330 core
in vec2 TextCoords;

out vec4 FragColor;

uniform sampler2D texture1;

void main(){
	//vec4 textColor = texture(texture1, TextCoords);
	
	//if(textColor.a < 0.1)
	//	discard;
		
	//FragColor = textColor;
	//Dato che abbiamo attivato il blending via codice, non c'è più bisogno di scartare i fragment da qui.
	//Viene fatto automaticamente al momento del render.
	FragColor = texture(texture1, TextCoords);
}