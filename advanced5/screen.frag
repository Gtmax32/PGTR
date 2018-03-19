#version 330 core
in vec2 TextCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main(){
	//FragColor = texture(screenTexture, TextCoords);
	
	//Riga di codice per ottenere l'inverso del colore della texture
	//FragColor = vec4( vec3(1.0 - texture(screenTexture, TextCoords)), 1.0);
	
	//Righe di codice per ottenere la texture in scala di grigi
	//FragColor = texture(screenTexture, TextCoords);
	//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	//float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
	//FragColor = vec4(average, average, average, 1.0);
	
	//Righe di codice per ottenere effetti più complessi, mediante l'utilizzo di matrici e di kernel
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
	
	//Kernel per ottenere un effetto "fumetto"
    /*float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );*/
	
	//Kernel per ottenere un effetto "blur"
	/*float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16  
	);*/
    
	//Kernel per ottenere un effetto che evidenzi i contorni degli oggetti
    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );
	
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TextCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);
}