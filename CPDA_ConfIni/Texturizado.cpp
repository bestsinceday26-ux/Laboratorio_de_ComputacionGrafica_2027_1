// Práctica 7
// Cadena Palafox Diego Aaron
// Fecha de entrega: 04 de Octubre del 2026
// Número de cuenta: 419047650

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"

// ============================================================
// FUNCTION PROTOTYPES
// ============================================================

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// ============================================================
// WINDOW DIMENSIONS
// ============================================================
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// ============================================================
// CAMERA
// ============================================================
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;

bool keys[1024] = {};
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
// ========================================================
// INIT GLFW
// // ========================================================
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

// ========================================================
// CREATE WINDOW
// ========================================================

    GLFWwindow* window = glfwCreateWindow(
        WIDTH, HEIGHT,
        "PRACTICA 7 DIEGO CADENA",
        nullptr, nullptr
    );

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
// ========================================================
// CALLBACKS
// ========================================================

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

// ========================================================
// INITIALIZE GLEW
// ========================================================
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
// ========================================================
// VIEWPORT
// ========================================================

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Profundidad para que las caras del cubo se dibujen correctamente.
    glEnable(GL_DEPTH_TEST);

    // ------------------------------------------------------------
    // SHADER
    // ------------------------------------------------------------
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // ------------------------------------------------------------
    // VERTICES
    // ------------------------------------------------------------
    GLfloat vertices[] =
    {
        // Frente 
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.3642f,0.5095f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.6450f,0.5095f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.6275f,0.6983f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.3642f,0.6983f,


        // Atrás 
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.3742f,0.015f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.6550f,0.015f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.6550f,0.2542f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.3742f,0.2542f,

         // Derecha 
          0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.6642f,0.5095f,
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.9455f,0.5095f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.9642f,0.6983f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.6375f,0.6983f,

          // Izquierda 
          -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,0.52f,
          -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.34f,0.52f,
          -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.34f,0.69f,
          -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,0.69f,

          // Abajo 
          -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.3642f,0.3242f,
           0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.6450f,0.3242f,
           0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.6450f,0.4295f,
          -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.3642f,0.4295f,

          // Arriba
          -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.35f,0.75f,
           0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.60f,0.75f,
           0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.60f,0.95f,
          -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.35f,0.95f,
    };

    GLuint indices[] =
    {
        0, 1, 2,  2, 3, 0,       // Frente
        4, 5, 6,  6, 7, 4,       // Atrás
        8, 9,10, 10,11, 8,       // Derecha
       12,13,14, 14,15,12,       // Izquierda
       16,17,18, 18,19,16,       // Abajo
       20,21,22, 22,23,20        // Arriba
    };

    // ------------------------------------------------------------
    // VAO / VBO / EBO
    // ------------------------------------------------------------
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    // Posición
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(GLfloat),
        (GLvoid*)0
    );
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);

    // Coordenadas de textura UV
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(GLfloat),
        (GLvoid*)(6 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // ------------------------------------------------------------
    // TEXTURA
    // ------------------------------------------------------------
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    int textureWidth, textureHeight, nrChannels;


    stbi_set_flip_vertically_on_load(true);

    unsigned char* image = stbi_load(
        "images/checker_Tex3.png",
        &textureWidth,
        &textureHeight,
        &nrChannels,
        0
    );

    if (!image)
    {
        std::cout << "ERROR: No se pudo cargar images/checker_Tex3.png"
            << std::endl;

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &texture1);

        glfwTerminate();
        return EXIT_FAILURE;
    }

    GLenum format = GL_RGB;

    if (nrChannels == 4)
        format = GL_RGBA;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 1)
        format = GL_RED;

    std::cout << "Textura cargada correctamente" << std::endl;
    std::cout << "Ancho: " << textureWidth << std::endl;
    std::cout << "Alto: " << textureHeight << std::endl;
    std::cout << "Canales: " << nrChannels << std::endl;

    // Parámetros de textura.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // NEAREST evita que se mezclen las regiones de la plantilla.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Enviar la imagen a la GPU.
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        textureWidth,
        textureHeight,
        0,
        format,
        GL_UNSIGNED_BYTE,
        image
    );


    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);

    // ------------------------------------------------------------
    // GAME LOOP
    // ------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lampShader.Use();

        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 projection = glm::perspective(
            camera.GetZoom(),
            (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
            0.1f,
            100.0f
        );

   
        glm::mat4 model(1.0f);

        model = glm::rotate(
            model,
            glm::radians(20.0f),
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        model = glm::rotate(
            model,
            glm::radians(-35.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        GLint modelLoc =
            glGetUniformLocation(lampShader.Program, "model");

        GLint viewLoc =
            glGetUniformLocation(lampShader.Program, "view");

        GLint projLoc =
            glGetUniformLocation(lampShader.Program, "projection");

        // Matrices
        glUniformMatrix4fv(
            modelLoc, 1, GL_FALSE, glm::value_ptr(model)
        );

        glUniformMatrix4fv(
            viewLoc, 1, GL_FALSE, glm::value_ptr(view)
        );

        glUniformMatrix4fv(
            projLoc, 1, GL_FALSE, glm::value_ptr(projection)
        );

        // --------------------------------------------------------
        // TEXTURA 
        // --------------------------------------------------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);


        GLint textureLoc =
            glGetUniformLocation(lampShader.Program, "ourTexture");

        glUniform1i(textureLoc, 0);

        // Dibujar cubo
        glBindVertexArray(VAO);

        glDrawElements(
            GL_TRIANGLES,
            36,
            GL_UNSIGNED_INT,
            0
        );

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // ------------------------------------------------------------
    // CLEAN UP
    // ------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture1);

    glfwTerminate();

    return 0;
}

// ------------------------------------------------------------
// MOVEMENT
// ------------------------------------------------------------
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// ------------------------------------------------------------
// KEY CALLBACK
// ------------------------------------------------------------
void KeyCallback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

// ------------------------------------------------------------
// MOUSE CALLBACK
// ------------------------------------------------------------
void MouseCallback(
    GLFWwindow* window,
    double xPos,
    double yPos)
{
    if (firstMouse)
    {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}