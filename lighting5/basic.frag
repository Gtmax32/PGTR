#version 330 core
//Definisco una struct per creare il materiale di un oggetto, partendo da come reagisce alla luce.
//Ho inserito l'elemento sampler2D per inserire una texture e realizzare un effetto di luce su di essa chiamato diffuse map.
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	//Introduco la variabile direction per simulare una luce onnipresente, come quella del sole
	vec3 direction;
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	//Introduco queste tre variabili per simulare l'attenuazione di una luce con l'allontanarsi da essa.
	float constant;
	float linear;
	float quadratic;
	
	//Introduco questa variabile per rappresentare l'angolo del cono di luce generato da una spotlight
	float cutOff;
	//Introduco questa variabile per rappresentare l'angolo del cono più esterno di una spotlight
	float outerCutOff;
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
	//vec3 lightDir = normalize(-light.direction); Con questa riga di codice, posso ottenere una luce uniforme in tutta la scena, come la luce del sole.
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TextCoords).rgb;
	
	//Operazzioni necessarie per definire la luce riflessa da un oggetto, con un certo valore di brillantezza.
	//Viene fatto calcolando la posizione della camera, la direzione tra la camera ed il punto in cui la luce tocca l'oggetto ed il vettore riflessione della luce.
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TextCoords).rgb;
	
	//Operazioni necessarie per simulare una spotlight con un cono più esterno con una luce più soffusa.
	//Theta indica l'angolo tra la normale dalla luce all'oggetto ed il vettore che delimita il cono.
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;
	
	//Operazioni necessarie per simulare l'attenuazione della luminosità di una sorgente luminosa, 
	//mediante il calcolo della distanza dell'oggetto dalla fonte luminosa ed il valore dell'attenuazione, derivante da essa.
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));
	
	//ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
	
	/*Eseguo questo controllo per verificare se l'oggetto è all'interno del cono. Uso il > perchè i valori che confronto sono coseni degli angoli, che invertono la relazione.
	if (theta > light.cutOff){
		{...}
	} else {
		FragColor = vec4(vec3(light.ambient * texture(material.diffuse, TextCoords).rgb),1.0);
	}*/
		
}