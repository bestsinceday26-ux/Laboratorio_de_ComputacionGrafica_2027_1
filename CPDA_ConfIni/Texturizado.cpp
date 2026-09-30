// Previo 7
// Cadena Palafox Diego Aaron
// Fecha de entrega: 29 de Septiembre del 2026
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

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// ============================================================
// WINDOW DIMENSIONS
// ============================================================

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;


// ============================================================
// CAMERA
// ============================================================

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;

bool keys[1024];
bool firstMouse = true;


// ============================================================
// LIGHT ATTRIBUTES
// ============================================================

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


// ============================================================
// DELTA TIME
// ============================================================

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


// ============================================================
// MAIN
// ============================================================

int main()
{

    // ========================================================
    // INIT GLFW
    // ========================================================

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );

    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE
    );

    glfwWindowHint(
        GLFW_RESIZABLE,
        GL_FALSE
    );


    // ========================================================
    // CREATE WINDOW
    // ========================================================

    GLFWwindow* window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        "PREVIO 7 DIEGO CADENA",
        nullptr,
        nullptr
    );


    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window"
            << std::endl;

        glfwTerminate();

        return EXIT_FAILURE;
    }


    glfwMakeContextCurrent(window);


    glfwGetFramebufferSize(
        window,
        &SCREEN_WIDTH,
        &SCREEN_HEIGHT
    );


    // ========================================================
    // CALLBACKS
    // ========================================================

    glfwSetKeyCallback(
        window,
        KeyCallback
    );

    glfwSetCursorPosCallback(
        window,
        MouseCallback
    );


    // Disable mouse cursor
    glfwSetInputMode(
        window,
        GLFW_CURSOR,
        GLFW_CURSOR_DISABLED
    );


    // ========================================================
    // INITIALIZE GLEW
    // ========================================================

    glewExperimental = GL_TRUE;


    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW"
            << std::endl;

        return EXIT_FAILURE;
    }


    // ========================================================
    // VIEWPORT
    // ========================================================

    glViewport(
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );


    // ========================================================
    // OPENGL OPTIONS
    // ========================================================

    // Depth test
    glEnable(GL_DEPTH_TEST);


    // ========================================================
    // TRANSPARENCY
    // ========================================================

    // Enable transparency
    glEnable(GL_BLEND);

    // Transparency equation
    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );


    // ========================================================
    // SHADER
    // ========================================================

    Shader lampShader(
        "Shader/lamp.vs",
        "Shader/lamp.frag"
    );


    // ========================================================
    // VERTEX DATA
    // ========================================================

    GLfloat vertices[] =
    {
        // Position              // Color              // Texture Coordinates

        -0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f,    0.0f, 0.0f,

         0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 1.0f,    1.0f, 0.0f,

         0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 1.0f,    1.0f, 1.0f,

        -0.5f,  0.5f, 0.0f,      1.0f, 1.0f, 1.0f,    0.0f, 1.0f
    };


    // ========================================================
    // INDICES
    // ========================================================

    GLuint indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };


    // ========================================================
    // VAO / VBO / EBO
    // ========================================================

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;


    glGenVertexArrays(
        1,
        &VAO
    );

    glGenBuffers(
        1,
        &VBO
    );

    glGenBuffers(
        1,
        &EBO
    );


    // Bind VAO
    glBindVertexArray(VAO);


    // ========================================================
    // VBO
    // ========================================================

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    // ========================================================
    // EBO
    // ========================================================

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        EBO
    );


    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );


    // ========================================================
    // POSITION ATTRIBUTE
    // ========================================================

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GLfloat),
        (GLvoid*)0
    );

    glEnableVertexAttribArray(0);


    // ========================================================
    // COLOR ATTRIBUTE
    // ========================================================

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(1);


    // ========================================================
    // TEXTURE COORDINATE ATTRIBUTE
    // ========================================================

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(GLfloat),
        (GLvoid*)(6 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(2);


    // Unbind VAO
    glBindVertexArray(0);


    // ========================================================
    // LOAD TEXTURE
    // ========================================================

    GLuint texture1;


    glGenTextures(
        1,
        &texture1
    );


    glBindTexture(
        GL_TEXTURE_2D,
        texture1
    );


    // Image information
    int textureWidth;
    int textureHeight;
    int nrChannels;


    // Flip image vertically
    stbi_set_flip_vertically_on_load(true);


    // ========================================================
    // LOAD IMAGE
    // ========================================================

    unsigned char* image = stbi_load(
        "images/window3.png",
        &textureWidth,
        &textureHeight,
        &nrChannels,
        0
    );


    // ========================================================
    // TEXTURE PARAMETERS
    // ========================================================

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR
    );


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );


    // ========================================================
    // CHECK IMAGE
    // ========================================================

    if (image)
    {

        std::cout << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << "     INFORMACION DE TEXTURA" << std::endl;
        std::cout << "==================================" << std::endl;

        std::cout << "Textura cargada correctamente"
            << std::endl;

        std::cout << "Ancho: "
            << textureWidth
            << std::endl;

        std::cout << "Alto: "
            << textureHeight
            << std::endl;

        std::cout << "Canales: "
            << nrChannels
            << std::endl;


        // ====================================================
        // DETERMINE FORMAT
        // ====================================================

        GLenum format;


        if (nrChannels == 4)
        {
            format = GL_RGBA;

            std::cout
                << "Formato: RGBA"
                << std::endl;

            std::cout
                << "La imagen contiene canal Alpha."
                << std::endl;
        }
        else if (nrChannels == 3)
        {
            format = GL_RGB;

            std::cout
                << "Formato: RGB"
                << std::endl;

            std::cout
                << "La imagen NO contiene canal Alpha."
                << std::endl;
        }
        else if (nrChannels == 1)
        {
            format = GL_RED;

            std::cout
                << "Formato: RED"
                << std::endl;

            std::cout
                << "ADVERTENCIA: la imagen tiene un solo canal."
                << std::endl;
        }
        else
        {
            format = GL_RGB;

            std::cout
                << "Formato desconocido."
                << std::endl;
        }


        // ====================================================
        // SEND IMAGE TO GPU
        // ====================================================

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


        // Generate mipmaps
        glGenerateMipmap(
            GL_TEXTURE_2D
        );

    }
    else
    {

        std::cout
            << "=================================="
            << std::endl;

        std::cout
            << "ERROR: No se pudo cargar la textura"
            << std::endl;

        std::cout
            << "=================================="
            << std::endl;
    }


    // Free image memory
    stbi_image_free(image);


    // ========================================================
    // GAME LOOP
    // ========================================================

    while (!glfwWindowShouldClose(window))
    {

        // ====================================================
        // DELTA TIME
        // ====================================================

        GLfloat currentFrame = glfwGetTime();

        deltaTime =
            currentFrame - lastFrame;

        lastFrame = currentFrame;


        // ====================================================
        // EVENTS
        // ====================================================

        glfwPollEvents();

        DoMovement();


        // ====================================================
        // CLEAR SCREEN
        // ====================================================

        glClearColor(
            0.1f,
            0.1f,
            0.1f,
            1.0f
        );


        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );


        // ====================================================
        // USE SHADER
        // ====================================================

        lampShader.Use();


        // ====================================================
        // CONNECT TEXTURE WITH SHADER
        // ====================================================

        glUniform1i(
            glGetUniformLocation(
                lampShader.Program,
                "ourTexture"
            ),
            0
        );


        // ====================================================
        // CAMERA
        // ====================================================

        glm::mat4 view;

        view =
            camera.GetViewMatrix();


        glm::mat4 projection =
            glm::perspective(
                camera.GetZoom(),
                (GLfloat)SCREEN_WIDTH /
                (GLfloat)SCREEN_HEIGHT,
                0.1f,
                100.0f
            );


        glm::mat4 model(1.0f);


        // ====================================================
        // GET UNIFORM LOCATIONS
        // ====================================================

        GLint modelLoc =
            glGetUniformLocation(
                lampShader.Program,
                "model"
            );


        GLint viewLoc =
            glGetUniformLocation(
                lampShader.Program,
                "view"
            );


        GLint projLoc =
            glGetUniformLocation(
                lampShader.Program,
                "projection"
            );


        // ====================================================
        // ACTIVATE TEXTURE
        // ====================================================

        glActiveTexture(GL_TEXTURE0);


        glBindTexture(
            GL_TEXTURE_2D,
            texture1
        );


        // ====================================================
        // SEND MATRICES TO SHADER
        // ====================================================

        glUniformMatrix4fv(
            viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );


        glUniformMatrix4fv(
            projLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );


        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );


        // ====================================================
        // DRAW QUAD
        // ====================================================

        glBindVertexArray(VAO);


        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            0
        );


        glBindVertexArray(0);


        // ====================================================
        // SWAP BUFFERS
        // ====================================================

        glfwSwapBuffers(window);
    }


    // ========================================================
    // CLEAN UP
    // ========================================================

    glDeleteVertexArrays(
        1,
        &VAO
    );


    glDeleteBuffers(
        1,
        &VBO
    );


    glDeleteBuffers(
        1,
        &EBO
    );


    glDeleteTextures(
        1,
        &texture1
    );


    // Terminate GLFW
    glfwTerminate();


    return 0;
}


// ============================================================
// MOVEMENT
// ============================================================

void DoMovement()
{

    // Forward
    if (
        keys[GLFW_KEY_W] ||
        keys[GLFW_KEY_UP]
        )
    {
        camera.ProcessKeyboard(
            FORWARD,
            deltaTime
        );
    }


    // Backward
    if (
        keys[GLFW_KEY_S] ||
        keys[GLFW_KEY_DOWN]
        )
    {
        camera.ProcessKeyboard(
            BACKWARD,
            deltaTime
        );
    }


    // Left
    if (
        keys[GLFW_KEY_A] ||
        keys[GLFW_KEY_LEFT]
        )
    {
        camera.ProcessKeyboard(
            LEFT,
            deltaTime
        );
    }


    // Right
    if (
        keys[GLFW_KEY_D] ||
        keys[GLFW_KEY_RIGHT]
        )
    {
        camera.ProcessKeyboard(
            RIGHT,
            deltaTime
        );
    }
}


// ============================================================
// KEY CALLBACK
// ============================================================

void KeyCallback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mode)
{

    // ESCAPE
    if (
        GLFW_KEY_ESCAPE == key &&
        GLFW_PRESS == action
        )
    {
        glfwSetWindowShouldClose(
            window,
            GL_TRUE
        );
    }


    // Keyboard state
    if (
        key >= 0 &&
        key < 1024
        )
    {

        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }

        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}


// ============================================================
// MOUSE CALLBACK
// ============================================================

void MouseCallback(
    GLFWwindow* window,
    double xPos,
    double yPos)
{

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;

        firstMouse = false;
    }


    GLfloat xOffset =
        xPos - lastX;


    GLfloat yOffset =
        lastY - yPos;


    lastX = xPos;
    lastY = yPos;


    camera.ProcessMouseMovement(
        xOffset,
        yOffset
    );
}