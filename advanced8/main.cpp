/* Codice ricavato dal tutorial sul Geometry Shader della categoria Advanced OpenGL,
 * partendo dal codice ottenuto dai tutorial precedenti.
 * Pagina web di riferimento: https://learnopengl.com/Advanced-OpenGL/Geometry-Shader
 */

#include <iostream>
#include <vector>
#include <map>
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

#include <utils/shader_v2.h>
#include <utils/camera_v2.h>
#include <utils/model_v3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Libreria per il caricamento delle immagini
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

using namespace std;

// Dimensioni della finestra dell'applicazione
const GLuint SCR_WIDTH = 1280, SCR_HEIGHT = 720;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Variabili utilizzate per implementare una Camera FPS
GLfloat lastX = (float)SCR_WIDTH / 2.0f;
GLfloat lastY = (float)SCR_HEIGHT / 2.0f;
GLfloat firstMouse = true;

// Deltatime per uniformare la velocità di movimento
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Registra gli eventi che modificano le dimensioni della finestra
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

GLuint loadTexture(char const * path);
GLuint loadCubemap(vector<std::string> faces);

int main(){
//	GLfloat points[] = {
//		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
//		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
//		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
//		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
//	}; 
	
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
	
	//SETTO LE FUNZIONI DI CALLBACK CHE SI OCCUPA DI GESTIRE LE INTERAZIONI DELL'UTENTE
	
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// GLAD cerca di caricare il contesto impostato da GLFW
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cout << "Errore nell'inizializzazione del contesto OpenGL!" << std::endl;
        return -1;
    }
	
	//SETTO IL VIEWPORT	
	//glViewport(0,0,SCR_WIDTH,SCR_HEIGHT);
	
	//SETTO IL DEPTH TEST
	glEnable(GL_DEPTH_TEST);
	
	//UTILIZZO LA CLASSE SHADER CREATA PER COMPILARE IL VS ED IL FS, E LINKARLI NEL PS
	Shader shader("../basic.vert", "../basic.frag"),
		   normalShader("../normal.vert", "../normal.frag", "../normal.geom");
	
	//UTILIZZO LA CLASSE MODEL CREATA PER CARICARE E VISUALIZZARE IL MODELLO 3D
	Model modelNano("../../nanosuit/nanosuit.obj");
	
//	CREO ED INIZIALIZZO IL VBO (vertex buffer object) e VAO (vertex array object) PER IL CUBO	
//	GLuint pointsVAO, pointsVBO;
//	1. Genero il VAO e gli assegno un ID
//	glGenVertexArrays(1, &pointsVAO);
//	2. Genero il VBO e gli assegno un ID
//	glGenBuffers(1, &pointsVBO);
//	3. Collego il VAO
//	glBindVertexArray(pointsVAO);
//	4. Collego il VBO, indicandogli il tipo
//	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
//	5. Copio i dati da una variabile al VBO
//	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
//	6. Indico gli attributi del VBO ed in particolare la loro struttura. Si rifanno alle variabili layout presenti nel VS: per n variabili layout nel VS, ci saranno n dichiarazioni di attributi.
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
//	glEnableVertexAttribArray(1);
//	10. Scollego il VBO
//	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
//	11. Scollego il VAO
//	glBindVertexArray(0);
			
	// draw as wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//AVVIO IL RENDER LOOP
	while(!glfwWindowShouldClose(window)){
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		processInput(window);
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//RENDERIZZO IL MODELLO
		//glBindVertexArray(pointsVAO);
		shader.Use();
		
		//PER IMPLEMENTARE LO ZOOM MEDIANTE MOUSE, INSERISCO LA CREAZIONE DELLA MATRICE DI PROIEZIONE NEL CICLO DI RENDER.
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("projection", projection);
				
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("view", view);
		
		glm::mat4 model = glm::mat4();
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setMat4("model", model);
		
		modelNano.Draw(shader);
		
		normalShader.Use();
		normalShader.setMat4("projection", projection);
		normalShader.setMat4("view", view);
		normalShader.setMat4("model", model);
		
		modelNano.Draw(normalShader);
		//glBindVertexArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();		
	}
	
	//DEALLOCO VAO, VBO
	//glDeleteVertexArrays(1, &pointsVAO);
	//glDeleteBuffers(1, &pointsVBO);
	
	glfwTerminate();
	
	return 0;		
}

void processInput(GLFWwindow *window){
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	if(firstMouse){
		lastX = xpos;
		lastY = ypos;
		
		firstMouse = false;
	}
	
	GLfloat xOffset = xpos - lastX;
	GLfloat yOffset = lastY - ypos; // Inverto la sottrazione per l'asse è negativo in questo caso
	
	lastX = xpos;
	lastY = ypos;
	
	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(yoffset);
}

GLuint loadTexture(char const * path){
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    
	if (data){
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
				format = GL_RGB;
			 else if (nrComponents == 4)
					format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        
    }
    else {
        std::cout << "Failed to load texture: " << path << "!" << std::endl;
    }
	
	stbi_image_free(data);
	
    return textureID;
}

GLuint loadCubemap(vector<std::string> faces){
	GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}