/* Codice ricavato dai primi 4 tutorial della categoria Getting Started.
 * Pagina web di riferimento: https://learnopengl.com/#!Getting-started/OpenGL
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

// Dimensioni della finestra dell'applicazione
GLuint WIDTH = 1024, HEIGHT = 768;

const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
	
// Registra gli eventi che modificano le dimensioni della finestra
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(){
	float vertices[] = {
     0.5f,  0.5f, 0.0f,// top right
     0.5f, -0.5f, 0.0f,// bottom right
    -0.5f, -0.5f, 0.0f,// bottom left
    -0.5f,  0.5f, 0.0f // top left 
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
	
	glfwWindowHint(GLFW_SAMPLES,4);
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
	
	//CREO IL VERTEX SHADER
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	//CONTROLLO CHE NON CI SIANO ERRORI DURANTE LA COMPILAZIONE DEL VS
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
	if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	
	//CREO IL FRAGMENT SHADER
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
	//CONTROLLO CHE NON CI SIANO ERRORI DURANTE LA COMPILAZIONE DEL FS
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
	
	//CREO IL PROGRAM SHADER, CHE SERVE DA TRAMITE DA IL VS E FS
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	//CONTROLLO CHE NON CI SIANO ERRORI DURANTE LA COMPILAZIONE DEL PS
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
	
	//ELIMINO GLI SHADER, DATO CHE SONO STATI COLLEGATI ALLO SHAREDPROGRAM
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	//CREO ED INIZIALIZZO IL VBO (vertex buffer object), VAO (vertex array object), EBO (element buffer object)
	
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//10. Scollego il VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//11. Scollego il VAO
	glBindVertexArray(0);
	
	//VISUALIZZO IL RETTANGOLO IN MODALITA' WIREFRAME
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
	//AVVIO IL RENDER LOOP
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		processInput(window);
	}
	
	//DEALLOCO VAO, VBO, EBO
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