/* Codice ricavato dal tutorial sugli Shaders della categoria Getting Started,
 * partendo dal codice ottenuto dai tutorial precedenti.
 * Pagina web di riferimento: https://learnopengl.com/#!Getting-started/Shaders
 * */
#include <iostream>
#include <math.h>
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

// Dimensioni della finestra dell'applicazione
GLuint WIDTH = 1024, HEIGHT = 768;

const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

// Registra gli eventi che modificano le dimensioni della finestra
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(){
	float vertices[] = {
        // positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
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
	
	GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"Prima Finestra",nullptr,nullptr);
	
	if (!window){
		std::cout <<"Errore nella creazione della finestra!" << std::endl;
		glfwTerminate();
		
		return -1;
	}
	
	glfwMakeContextCurrent(window);
		
	// GLAD cerca di caricare il contesto impostato da GLFW
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Errore nell'inizializzazione del contesto OpenGL!" << std::endl;
        return -1;
    }
	
	//SETTO IL VIEWPORT	
	glViewport(0,0,WIDTH,HEIGHT);
	
	//SETTO LA FUNZIONE DI CALLBACK CHE SI OCCUPA DI GESTIRE LE INTERAZIONI DELL'UTENTE
	
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	
	Shader shader("basic.vert", "basic.frag");
	
	//CREO ED INIZIALIZZO IL VBO (vertex buffer object) e VAO (vertex array object)
	
	GLuint VBO, VAO;
	//1. Genero il VAO e gli assegno un ID
	glGenVertexArrays(1, &VAO);
	//2. Genero il VBO e gli assegno un ID
	glGenBuffers(1, &VBO);
	//3. Collego il VAO
	glBindVertexArray(VAO);
	//4. Collego il VBO, indicandogli il tipo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//5. Copio i dati da una variabile al VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);
	//6. Indico gli attributi del VBO ed in particolare la loro struttura. Si rifanno alle variabili layout presenti nel VS: per n variabili layout nel VS, ci saranno n dichiarazioni di attributi.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	//7. Scollego il VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//8. Scollego il VAO
	glBindVertexArray(0);
	
	//VISUALIZZO IL RETTANGOLO IN MODALITA' WIREFRAME
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float offset = 0.5f;
		
	//AVVIO IL RENDER LOOP
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		shader.Use();		
		glUniform1f(glGetUniformLocation(shader.Program,"xOffset"),offset);
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		processInput(window);
	}
	
	//DEALLOCO VAO, VBO, EBO
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	
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