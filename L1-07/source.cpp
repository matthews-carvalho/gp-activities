#include <iostream>
#include <cmath>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();

// Dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL)
const GLchar *vertexShaderSource = "#version 400\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                                   "}\0";

// Código fonte do Fragment Shader (em GLSL)
const GLchar *fragmentShaderSource = "#version 400\n"
                                     "out vec4 color;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n" // Cor vermelha para a espiral
                                     "}\n\0";

// Função MAIN
int main()
{
    // Inicialização da GLFW
    glfwInit();

    // Criação da janela GLFW
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Espiral de Arquimedes", nullptr, nullptr);
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

        // Desenha a espiral
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, 1000); // Desenha a espiral com 1000 pontos
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
    const int numPoints = 1000; // Número de pontos para desenhar a espiral
    const float a = 0.05f;      // Constante para deslocamento inicial
    const float b = 0.05f;      // Constante que determina a largura entre os braços da espiral

    // Cria um vetor para armazenar os vértices
    std::vector<GLfloat> vertices;

    // Calcula os vértices da espiral
    for (int i = 0; i < numPoints; ++i)
    {
        float theta = i * 0.1f;  // Incremento do ângulo
        float r = a + b * theta; // Cálculo do raio

        float x = r * cos(theta);
        float y = r * sin(theta);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f); // z
    }

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}