#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){	
	//Operazioni necessaria per definire una luce ambientale, senza una sorgente ben definita.
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
		
	//Operazioni necessarie per definire una luce diffusa da una sorgente, che illumina un particolare punto del nostro cubo.
	//Viene fatto calcolando il vettore direzione della luce e l'angolo che si forma tra questo vettore e la normale alla superficie del cubo.
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	//Operazzioni necessarie per definire la luce riflessa da un oggetto, con un certo valore di brillantezza.
	//Viene fatto calcolando la posizione della camera, la direzione tra la camera ed il punto in cui la luce tocca l'oggetto ed il vettore riflessione della luce.
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}