/* Codice ricavato dal tutorial sulle Trasformations della categoria Getting Started,
 * partendo dal codice ottenuto dai tutorial precedenti.
 * Pagina web di riferimento: https://learnopengl.com/Getting-started/Transformations
 */

#include <iostream>
// THIS IS OPTIONAL AND NOT REQUIRED, ONLY USE THIS IF YOU DON'T WANT GLAD TO INCLUDE windows.h
// GLAD will include windows.h for APIENTRY if it was not previously defined.
// Make sure you have the correct definition for APIENTRY for platforms which define _WIN32 but don't use __stdcall
#ifdef _WIN32
    #define APIENTRY __stdcall
#endif

#include <glad/glad.h>

// GLFW
#include <glfw/glfw3.h>

// confirm that GLAD didn't include windows.h
#ifdef _WINDOWS_
    #error windows.h was included!
#endif

#include <utils/shader_v1.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// includo la libreria per il caricamento delle immagini
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

// Dimensioni della finestra dell'applicazione
GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;

// Registra gli eventi che modificano le dimensioni della finestra
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(){
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	
	GLuint indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
	};
	
	//INIZIALIZZO GLFW
	
	if (! glfwInit()){
		std::cout << "Errore nell'inizializzazione di GLFW!\n" << std::endl;
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	//CREO LA FINESTRA
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"Prima Finestra",nullptr,nullptr);
	
	if (!window){
		std::cout <<"Errore nella creazione della finestra!" << std::endl;
		glfwTerminate();
		
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	//SETTO LA FUNZIONE DI CALLBACK CHE SI OCCUPA DI GESTIRE LE INTERAZIONI DELL'UTENTE
	
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	
	// GLAD cerca di caricare il contesto impostato da GLFW
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Errore nell'inizializzazione del contesto OpenGL!" << std::endl;
        return -1;
    }
	
	//SETTO IL VIEWPORT	
	glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
	
	//UTILIZZO LA CLASSE SHADER CREATA PER COMPILARE IL VS ED IL FS, E LINKARLI NEL PS
	Shader shader("../basic.vert", "../basic.frag");
	
	//CREO ED INIZIALIZZO IL VBO (vertex buffer object) e VAO (vertex array object)
	
	GLuint VBO, VAO, EBO;
	//1. Genero il VAO e gli assegno un ID
	glGenVertexArrays(1, &VAO);
	//2. Genero il VBO e gli assegno un ID
	glGenBuffers(1, &VBO);
	//3. Genero il EBO e gli assegno un ID
	glGenBuffers(1, &EBO);
	//4. Collego il VAO
	glBindVertexArray(VAO);
	//5. Collego il VBO, indicandogli il tipo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//6. Copio i dati da una variabile al VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);
	//7. Collego il EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//8. Copio gli indici degli elementi nell'EBO	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	
	//9. Indico gli attributi del VBO ed in particolare la loro struttura. Si rifanno alle variabili layout presenti nel VS: per n variabili layout nel VS, ci saranno n dichiarazioni di attributi.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
//	10. Scollego il VBO
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	11. Scollego il VAO
//	glBindVertexArray(0);
	
	//VISUALIZZO IL RETTANGOLO IN MODALITA' WIREFRAME
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannel;
	unsigned char *data = stbi_load("../../container.jpg", &width, &height, &nrChannel, 0);
	
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Failed to load texture Container!" <<std::endl;
	}
	stbi_image_free(data);
	
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	data = stbi_load("../../awesomeface.png", &width, &height, &nrChannel, 0);
	
	if(data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		std::cout << "Failed to load texture Smile!" <<std::endl;
	}
	stbi_image_free(data);
	
	shader.Use();
	GLuint texture1Loc = glGetUniformLocation(shader.Program,"texture1"),
		   texture2Loc = glGetUniformLocation(shader.Program,"texture2");
	glUniform1i(texture1Loc ,0);
	glUniform1i(texture2Loc ,1);
	
//	glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
//	glm::mat4 trans;
//	//PRIORITA' DELLE TRASFORMAZIONI: SCALA -> ROTAZIONE -> TRASLAZIONE
//	//COSTRUISCO UNA MATRICE CHE EFFETTUI PRIMA LA SCALA DEL RETTANGOLO E POI LA SUA ROTAZIONE. 
//	//PARTENDO DALLA MATRICE UNITARIA (trans), EFFETTUO PRIMA LA ROTAZIONE ED IN SEGUITO LA SCALA, IN MODO CHE QUEST'ULTIMA VENGA APPLICATA PER PRIMA.
//	//LA FUNZIONE rotate PRENDE IN INPUT LA MATRICE UNITARIA, L'ANGOLO DI ROTAZIONE IN RADIANTI E L'ASSE DI ROTAZIONE (in questo caso Z perchè l'immagine è bi-dimensionale)
//	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f,0.0f,1.0f));
//	//LA FUNZIONE SCALE PARTE DALLA MATRICE RISULTATO DELL'OPERAZIONE DI ROTATE E LA SCALA IN BASE AL VETTORE DI TRASFORMAZIONE PASSATO.
//	trans = glm::scale(trans, glm::vec3(0.5f,0.5f,0.5f));
//	
//	//LA FUNZIONE TRANSLATE COSTRUISCE UNA MATRICE DI TRASLAZIONE PARTENDO DA UNA MATRICE UNITARIA (trans) E DA UN VETTORE TRANSLAZIONE (glm::vec3)
//	//trans = glm::translate(trans, glm::vec3(1.0f,1.0f,0.0f));
//	//vec = trans * vec;
//	//std::cout << vec.x << vec.y << vec.z << std::endl;
//	
//	GLuint transformLoc = glGetUniformLocation(shader.Program, "transform");
//	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	
	//AVVIO IL RENDER LOOP
	while(!glfwWindowShouldClose(window)){
		processInput(window);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f,0.0f,1.0f));
		
		shader.Use();
		
		GLuint transformLoc = glGetUniformLocation(shader.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();		
	}
	
	//DEALLOCO VAO, VBO
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	
	return 0;		
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}