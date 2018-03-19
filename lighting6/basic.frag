#version 330 core
//Definisco una struct per creare il materiale di un oggetto, partendo da come reagisce alla luce.
//Ho inserito l'elemento sampler2D per inserire una texture e realizzare un effetto di luce su di essa chiamato diffuse map.
struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

//Definisco una struct per creare un luce direzionale
struct DirLight{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//Definisco una struct per creare una pointlight
struct PointLight{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 direction;
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
	
	float cutOff;
	float outerCutOff;
};

#define NR_POINT_LIGHTS 4

in vec3 Normal;
in vec3 FragPos;
in vec2 TextCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){	
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	
	for(int i = 0; i < NR_POINT_LIGHTS; i++){
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	
	FragColor = vec4(result, 1.0);		
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	//Operazioni necessaria per definire una luce ambientale, senza una sorgente ben definita.
	vec3 ambient = light.ambient * texture(material.diffuse, TextCoords).rgb;
	
	//Operazioni necessarie per definire una luce diffusa da una sorgente, che illumina un particolare punto del nostro cubo.
	//Viene fatto calcolando il vettore direzione della luce e l'angolo che si forma tra questo vettore e la normale alla superficie del cubo.
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TextCoords).rgb;
	
	//Operazzioni necessarie per definire la luce riflessa da un oggetto, con un certo valore di brillantezza.
	//Viene fatto calcolando la posizione della camera, la direzione tra la camera ed il punto in cui la luce tocca l'oggetto ed il vettore riflessione della luce.
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TextCoords).rgb;
	
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	//Operazioni necessaria per definire una luce ambientale, senza una sorgente ben definita.
	vec3 ambient = light.ambient * texture(material.diffuse, TextCoords).rgb;
	
	//Operazioni necessarie per definire una luce diffusa da una sorgente, che illumina un particolare punto del nostro cubo.
	//Viene fatto calcolando il vettore direzione della luce e l'angolo che si forma tra questo vettore e la normale alla superficie del cubo.
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TextCoords).rgb;
	
	//Operazzioni necessarie per definire la luce riflessa da un oggetto, con un certo valore di brillantezza.
	//Viene fatto calcolando la posizione della camera, la direzione tra la camera ed il punto in cui la luce tocca l'oggetto ed il vettore riflessione della luce.
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TextCoords).rgb;
	
	//Operazioni necessarie per simulare l'attenuazione della luminosità di una sorgente luminosa, 
	//mediante il calcolo della distanza dell'oggetto dalla fonte luminosa ed il valore dell'attenuazione, derivante da essa.
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	//Operazioni necessaria per definire una luce ambientale, senza una sorgente ben definita.
	vec3 ambient = light.ambient * texture(material.diffuse, TextCoords).rgb;
	
	//Operazioni necessarie per definire una luce diffusa da una sorgente, che illumina un particolare punto del nostro cubo.
	//Viene fatto calcolando il vettore direzione della luce e l'angolo che si forma tra questo vettore e la normale alla superficie del cubo.
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TextCoords).rgb;
	
	//Operazzioni necessarie per definire la luce riflessa da un oggetto, con un certo valore di brillantezza.
	//Viene fatto calcolando la posizione della camera, la direzione tra la camera ed il punto in cui la luce tocca l'oggetto ed il vettore riflessione della luce.
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TextCoords).rgb;
	
	//Operazioni necessarie per simulare una spotlight con un cono più esterno con una luce più soffusa.
	//Theta indica l'angolo tra la normale dalla luce all'oggetto ed il vettore che delimita il cono.
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		
	//Operazioni necessarie per simulare l'attenuazione della luminosità di una sorgente luminosa, 
	//mediante il calcolo della distanza dell'oggetto dalla fonte luminosa ed il valore dell'attenuazione, derivante da essa.
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));
	
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	
	return (ambient + diffuse + specular);
	
}