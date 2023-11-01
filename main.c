#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h> 

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transform;
out vec3 vertexColor;  // Output variable to transfer vertex position to fragment shader
void main() {
    gl_Position = transform * vec4(aPos, 1.0);
    vertexColor = aPos;  // Transfer vertex position
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;  // Input vertex position from vertex shader
out vec4 FragColor;
void main() {
    FragColor = vec4(abs(vertexColor), 1.0);  // Use the absolute value of the vertex position as color
}
)";



void generateRotationMatrix(float angle, float matrix[4][4]) {
    // Rotation around X-axis
    float cosAx = cos(angle);
    float sinAx = sin(angle);

    // Rotation around Y-axis
    float cosAy = cos(angle * 0.5f);
    float sinAy = sin(angle * 0.5f);

    // Matrix for X-axis rotation
    float rotationX[4][4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, cosAx, -sinAx, 0.0f},
        {0.0f, sinAx, cosAx, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    // Matrix for Y-axis rotation
    float rotationY[4][4] = {
        {cosAy, 0.0f, sinAy, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {-sinAy, 0.0f, cosAy, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };

    // Multiply together to get combined rotation matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                matrix[i][j] += rotationX[i][k] * rotationY[k][j];
            }
        }
    }
}



int main() {
    // Initialization
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Cube Renderer", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    // Shader creation
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data
    float vertices[] = {
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        1, 2, 6, 6, 5, 1,
        2, 3, 7, 7, 6, 2,
        0, 3, 7, 7, 4, 0
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float rotationMatrix[4][4];

    // Rendering loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        generateRotationMatrix((float)glfwGetTime(), rotationMatrix);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, &rotationMatrix[0][0]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

