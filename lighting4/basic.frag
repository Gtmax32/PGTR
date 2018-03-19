#version 330 core
//Definisco una struct per creare il materiale di un oggetto, partendo da come reagisce alla luce.
//Ho inserito l'elemento sampler2D per inserire una texture e realizzare un effetto di luce su di essa chiamato diffuse map.
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main(){	
	//Operazioni necessaria per definire una luce ambientale, senza una sorgente ben definita.
	vec3 ambient = light.ambient * texture(material.diffuse, TextCoords).rgb;
		
	//Operazioni necessarie per definire una luce diffusa da una sorgente, che illumina un particolare punto del nostro cubo.
	//Viene fatto calcolando il vettore direzione della luce e l'angolo che si forma tra questo vettore e la normale alla superficie del cubo.
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TextCoords).rgb;
	
	//Operazzioni necessarie per definire la luce riflessa da un oggetto, con un certo valore di brillantezza.
	//Viene fatto calcolando la posizione della camera, la direzione tra la camera ed il punto in cui la luce tocca l'oggetto ed il vettore riflessione della luce.
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TextCoords).rgb;
	
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}