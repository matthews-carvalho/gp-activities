#include <iostream>
#include <cmath>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

const float PI = 3.14159;

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
                                     "   color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n" // Cor amarela para a estrela
                                     "}\n\0";

// Função MAIN
int main()
{
    // Inicialização da GLFW
    glfwInit();

    // Criação da janela GLFW
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Estrela", nullptr, nullptr);
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

        // Desenha a estrela
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 12); // Desenha a estrela com 10 pontos mais o centro e o ponto final
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
    // Define os raios para as pontas e os vértices internos da estrela
    const float r_outer = 0.5f;
    const float r_inner = 0.2f;
    const int numPoints = 5; // Número de pontas da estrela

    // Cria um vetor para armazenar os vértices da estrela
    std::vector<GLfloat> vertices;

    // Adiciona o ponto central da estrela (origem)
    vertices.push_back(0.0f); // x
    vertices.push_back(0.0f); // y
    vertices.push_back(0.0f); // z

    // Gera os vértices alternando entre as pontas e os vértices internos
    for (int i = 0; i < numPoints * 2; ++i)
    {
        // Alterna entre vértice externo (ponta) e interno
        float radius = (i % 2 == 0) ? r_outer : r_inner;
        float angle = i * PI / numPoints; // Ângulo entre os vértices

        float x = radius * cos(angle);
        float y = radius * sin(angle);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f); // z
    }

    // Adiciona o primeiro ponto novamente para fechar a estrela
    vertices.push_back(r_outer * cos(0.0f));
    vertices.push_back(r_outer * sin(0.0f));
    vertices.push_back(0.0f);

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