#include <iostream>
#include <string>
#include <assert.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();

// Dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL)
const GLchar* vertexShaderSource = "#version 400\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

// Código fonte do Fragment Shader (em GLSL)
const GLchar* fragmentShaderSource = "#version 400\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// Função MAIN
int main()
{
    // Inicialização da GLFW
    glfwInit();

    // Criação da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triângulos Preenchidos, Contornos e Pontos", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Inicialização do GLAD
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Configuração dos shaders
    setupShader();
    GLuint VAO = setupGeometry();

    // Loop de renderização
    while (!glfwWindowShouldClose(window))
    {
        // Verifica eventos de entrada
        glfwPollEvents();

        // Limpa o buffer de cor
        glClear(GL_COLOR_BUFFER_BIT);

        // 1. Desenhar o polígono preenchido
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 2. Desenhar o contorno do polígono
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 3. Desenhar os pontos dos vértices
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glPointSize(5.0f); // Ajuste o tamanho dos pontos para melhor visualização
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Desvincula o VAO
        glBindVertexArray(0);

        // Troca os buffers
        glfwSwapBuffers(window);
    }

    // Termina a execução
    glfwTerminate();
    return 0;
}

int setupShader()
{
    // Criação e compilação dos shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Linka os shaders no programa
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Usa o programa shader
    glUseProgram(shaderProgram);

    // Limpa os shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int setupGeometry()
{
    // Coordenadas dos vértices para dois triângulos
    GLfloat vertices[] = {
        // Primeiro triângulo
        -0.5f, -0.5f, 0.0f, // Vértice inferior esquerdo
        0.5f, -0.5f, 0.0f,  // Vértice inferior direito
        0.0f, 0.5f, 0.0f,   // Vértice superior
        // Segundo triângulo
        0.5f, -0.5f, 0.0f,  // Vértice inferior esquerdo
        1.0f, 0.5f, 0.0f,   // Vértice inferior direito
        0.0f, 0.5f, 0.0f    // Vértice superior centralizado
    };

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}