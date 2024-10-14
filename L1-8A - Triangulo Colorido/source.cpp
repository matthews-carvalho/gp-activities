#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 600;

// Vertex Shader (GLSL)
const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 vertexColor;

    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        vertexColor = aColor;
    }
)";

// Fragment Shader (GLSL)
const char *fragmentShaderSource = R"(
    #version 330 core
    in vec3 vertexColor;
    out vec4 FragColor;

    void main() {
        FragColor = vec4(vertexColor, 1.0);
    }
)";

// Função de callback de redimensionamento da janela
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Função MAIN
int main()
{
    // Inicializa o GLFW
    if (!glfwInit())
    {
        std::cerr << "Falha ao inicializar o GLFW" << std::endl;
        return -1;
    }

    // Cria a janela com contexto OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "Triangulo Colorido", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Carrega todas as funções OpenGL usando GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    // Configura o viewport
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    // Deleta os shaders, pois já estão linkados no programa
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Configuração dos vértices e cores
    float vertices[] = {
        // Posições      // Cores
         0.0f,  1.0f,    1.0f, 0.0f, 0.0f,  // P1 - Vermelho
        -1.0f, -1.0f,    0.0f, 1.0f, 0.0f,  // P2 - Verde
         1.0f, -1.0f,    0.0f, 0.0f, 1.0f   // P3 - Azul
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    // Criação dos buffers e arrays
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Vincula o VAO
    glBindVertexArray(VAO);

    // Vincula e configura o VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vincula e configura o EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Posição
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Desvincula o VAO
    glBindVertexArray(0);


    // Loop de renderização
    while (!glfwWindowShouldClose(window))
    {
        // Verifica eventos de entrada
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenha o triângulo
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Troca os buffers
        glfwSwapBuffers(window);
    }
    
    // Libera memória e termina o GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}