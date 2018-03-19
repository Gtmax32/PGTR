#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;

void GenerateLine(int index){
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}

/*void build_house(vec4 position){
	fColor = gs_in[0].color;
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); //1: bottom-left
	EmitVertex();
	gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); //2: bottom-right
	EmitVertex();
	gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); //3: top-left
	EmitVertex();
	gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); //4: top-right
	EmitVertex();
	gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); //5: top
	fColor = vec3(1.0);
	EmitVertex();
	EndPrimitive();
}

vec3 GetNormal(){
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	
	return normalize(cross(a,b));
}

vec4 explode(vec4 position, vec3 normal){
	float magnitude = 2.0;
	vec3 direction = normal * (sin(time) + 1.0) * magnitude;
	return position + vec4(direction, 1.0);
}*/

void main(){
	//Codice utilizzato per creare delle "case" disegnando delle linee, partendo dalla posizione dei punti
	//build_house(gl_in[0].gl_Position);
	
	//Codice utilizzato per esplodere un modello
	/*vec3 normal = GetNormal();
	
	gl_Position = explode(gl_in[0].gl_Position, normal);
	TextCoords = gs_in[0].textCoords;
	EmitVertex();
	
	gl_Position = explode(gl_in[1].gl_Position, normal);
	TextCoords = gs_in[1].textCoords;
	EmitVertex();
	
	gl_Position = explode(gl_in[2].gl_Position, normal);
	TextCoords = gs_in[2].textCoords;
	EmitVertex();
	EndPrimitive();*/
	
	//Codice usato per disegnare le normali in un modello
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}