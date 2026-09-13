// Práctica 4
// Cadena Palafox Diego Aaron
// Fecha de entrega: 13 de Septiembre del 2026
// Número de cuenta: 419047650

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;

int main()
{
    // --------------------------------------------------
    // INICIALIZAR GLFW
    // --------------------------------------------------

    if (!glfwInit())
    {
        std::cout << "Error al inicializar GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    // OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // --------------------------------------------------
    // CREAR VENTANA
    // --------------------------------------------------

    GLFWwindow* window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        "PrActica 4 DIEGO CADENA",
        nullptr,
        nullptr
    );

    if (window == nullptr)
    {
        std::cout << "Error al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    // --------------------------------------------------
    // INICIALIZAR GLEW
    // --------------------------------------------------

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error al inicializar GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // --------------------------------------------------
    // VIEWPORT
    // --------------------------------------------------

    int screenWidth;
    int screenHeight;

    glfwGetFramebufferSize(
        window,
        &screenWidth,
        &screenHeight
    );

    glViewport(
        0,
        0,
        screenWidth,
        screenHeight
    );

    // --------------------------------------------------
    // CONFIGURACIÓN DE OPENGL
    // --------------------------------------------------

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    // --------------------------------------------------
    // SHADER
    // --------------------------------------------------

    Shader ourShader(
        "Shader/core.vs",
        "Shader/core.frag"
    );

    // --------------------------------------------------
    // VÉRTICES DEL CUBO
    //
    // Cada vértice tiene:
    // X Y Z R G B
    //
    // 36 vértices = 12 triángulos = 6 caras
    // --------------------------------------------------

    float vertices[] =
    {
        // ------------------------------------------------
        // FRONT
        // ------------------------------------------------
         -0.5f, -0.5f,  0.5f,   0.8f, 0.8f, 0.8f,
         0.5f, -0.5f, 0.5f,   0.8f, 0.8f, 0.8f,
         0.5f,  0.5f, 0.5f,   0.8f, 0.8f, 0.8f,

         0.5f,  0.5f, 0.5f,   0.8f, 0.8f, 0.8f,
         -0.5f,  0.5f,  0.5f,   0.8f, 0.8f, 0.8f,
         -0.5f, -0.5f,  0.5f,   0.8f, 0.8f, 0.8f,



        // ------------------------------------------------
        // BACK
        // ------------------------------------------------

        -0.5f, -0.5f, -0.5f,   0.95f, 0.95f, 0.95f,
         0.5f, -0.5f, -0.5f,   0.95f, 0.95f, 0.95f,
         0.5f,  0.5f, -0.5f,   0.95f, 0.95f, 0.95f,

         0.5f,  0.5f, -0.5f,   0.95f, 0.95f, 0.95f,
        -0.5f,  0.5f, -0.5f,   0.95f, 0.95f, 0.95f,
        -0.5f, -0.5f, -0.5f,   0.95f, 0.95f, 0.95f,

        // ------------------------------------------------
        // RIGHT
        // ------------------------------------------------
         0.5f, -0.5f,  0.5f,   0.55f, 0.55f, 0.55f,
         0.5f, -0.5f,  -0.5f,   0.55f, 0.55f, 0.55f,
         0.5f,  0.5f,  -0.5f,   0.55f, 0.55f, 0.55f,

         0.5f,  0.5f,  -0.5f,   0.55f, 0.55f, 0.55f,
         0.5f,  0.5f,  0.5f,   0.55f, 0.55f, 0.55f,
         0.5f, -0.5f,  0.5f,   0.55f, 0.55f, 0.55f,

         // ------------------------------------------------
         // LEFT
         // ------------------------------------------------

         -0.5f,  0.5f,  0.5f,   0.65f, 0.65f, 0.65f,
         -0.5f,  0.5f, -0.5f,   0.65f, 0.65f, 0.65f,
         -0.5f, -0.5f, -0.5f,   0.65f, 0.65f, 0.65f,

         -0.5f, -0.5f, -0.5f,   0.65f, 0.65f, 0.65f,
         -0.5f, -0.5f,  0.5f,   0.65f, 0.65f, 0.65f,
         -0.5f,  0.5f,  0.5f,   0.65f, 0.65f, 0.65f,

         // ------------------------------------------------
         // BOTTOM
         // ------------------------------------------------

         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,

          0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,

         // ------------------------------------------------
         // TOP
         // ------------------------------------------------

         -0.5f,  0.5f, -0.5f,   0.45f, 0.45f, 0.45f,
          0.5f,  0.5f, -0.5f,   0.45f, 0.45f, 0.45f,
          0.5f,  0.5f,  0.5f,   0.45f, 0.45f, 0.45f,

          0.5f,  0.5f,  0.5f,   0.45f, 0.45f, 0.45f,
         -0.5f,  0.5f,  0.5f,   0.45f, 0.45f, 0.45f,
         -0.5f,  0.5f, -0.5f,   0.45f, 0.45f, 0.45f 
    };

    // --------------------------------------------------
    // VBO Y VAO
    // --------------------------------------------------

    GLuint VBO;
    GLuint VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

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

    // --------------------------------------------------
    // ATRIBUTO 0 = POSICIÓN
    // --------------------------------------------------

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)0
    );

    glEnableVertexAttribArray(0);

    // --------------------------------------------------
    // ATRIBUTO 1 = COLOR
    // --------------------------------------------------

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(1);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );

    glBindVertexArray(0);

    // --------------------------------------------------
    // MATRIZ DE PROYECCIÓN
    // --------------------------------------------------

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (GLfloat)screenWidth / (GLfloat)screenHeight,
        0.1f,
        100.0f
    );

    // --------------------------------------------------
    // BUCLE PRINCIPAL
    // --------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        // Entrada del teclado
        Inputs(window);

        // Eventos
        glfwPollEvents();

        // ------------------------------------------------
        // LIMPIAR PANTALLA
        // ------------------------------------------------

        glClearColor(
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );

        // ------------------------------------------------
        // ACTIVAR SHADER
        // ------------------------------------------------

        ourShader.Use();

        // ------------------------------------------------
        // MATRIZ VIEW
        // ------------------------------------------------

        glm::mat4 view = glm::mat4(1.0f);

        view = glm::translate(
            view,
            glm::vec3(
                movX,
                movY,
                movZ
            )
        );

        view = glm::rotate(
            view,
            glm::radians(rot),
            glm::vec3(
                0.0f,
                1.0f,
                0.0f
            )
        );

        // ------------------------------------------------
        // LOCALIZACIONES DE LOS UNIFORMS
        // ------------------------------------------------

        GLint modelLoc =
            glGetUniformLocation(
                ourShader.Program,
                "model"
            );

        GLint viewLoc =
            glGetUniformLocation(
                ourShader.Program,
                "view"
            );

        GLint projectionLoc =
            glGetUniformLocation(
                ourShader.Program,
                "projection"
            );

        // ------------------------------------------------
        // ENVIAR VIEW Y PROJECTION
        // ------------------------------------------------

        glUniformMatrix4fv(
            viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );

        glUniformMatrix4fv(
            projectionLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        // ------------------------------------------------
        // ACTIVAR VAO
        // ------------------------------------------------

        glBindVertexArray(VAO);

        // =================================================
        // CABEZA 
        // =================================================

        glm::mat4 model = glm::mat4(1.0f);

        // Primero trasladamos
        model = glm::translate(
            model,
            glm::vec3(
                0.0f,
                0.6f,
                0.0f
            )
        );

        // Después escalamos
        model = glm::scale(
            model,
            glm::vec3(
                1.0f,
                1.0f,
                1.0f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
 
        // =================================================
        // OREJA 1 - IZQUIERDA 
        // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                -0.3f,
                1.4f,
                -0.1f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.4f,
                1.0f,
                0.3f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );

        // =================================================
        // OREJA 2 - DERECHA
        // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.3f,
                1.4f,
                -0.1f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.4f,
                1.0f,
                0.3f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );

        // =================================================
        // NARIZ
        // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.0f,
                0.5f,
                0.6f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.3f,
                0.3f,
                0.2f
            )
        );
       model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );

         // =================================================
        // CACHETE IZQUIERDO 
       // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                -0.45f,
                0.4f,
                0.4f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.6f,
                0.4f,
                0.28f
            )
        );
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
        // =================================================
       // CACHETE DERECHO
      // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.45f,
                0.4f,
                0.4f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.6f,
                0.4f,
                0.28f
            )
        );
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );

        // =================================================
        // CUERPO
        // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.0f,
                -0.3f,
                -0.6f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                1.4f,
                0.8f,
                1.4f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
        // =================================================
       // PATA 1 - DERECHA / FRENTE
       // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.35f,
                -0.8f,
                -0.3f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.4f,
                0.4f,
                0.4f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );

        // =================================================
       // PATA 2 - IZQUIERDA/ FRENTE
       // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                -0.35f,
                -0.8f,
                -0.3f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.4f,
                0.4f,
                0.4f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
        // =================================================
        // PATA 3 - DERECHA / ATRAS
        // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.50f,
                -0.8f,
                -1.1f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.4f,
                0.4f,
                0.4f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
        // =================================================
       // PATA 4 - IZQUIERDA/ ATRAS
       // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                -0.50f,
                -0.8f,
                -1.1f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.4f,
                0.4f,
                0.4f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
        // =================================================
       // PALMA DERECHA FRENTE
      // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.35f,
                -1.14f,
                -0.2f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.5f,
                0.25f,
                0.70f
            )
        );
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
        // =================================================
       // PALMA IZQUIERDO FRENTE
      // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                -0.35f,
                -1.14f,
                -0.2f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.5f,
                0.25f,
                0.70f
            )
        );
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
        // =================================================
      // PALMA DERECHA TRASERA
     // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                -0.45f,
                -1.14f,
                -1.0f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.5f,
                0.25f,
                0.70f
            )
        );
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );


        // =================================================
       // PALMA IZQUIERDA TRASERA
      // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.45f,
                -1.14f,
                -1.0f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.5f,
                0.25f,
                0.70f
            )
        );
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );

        // =================================================
       // COLA
      // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.0f,
                -0.1f,
                -1.7f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                0.5f,
                0.8f,
                0.8f
            )
        );
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 10.0f, 0.0f));
        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );

        // =================================================
        // CHAMORRO 
        // =================================================

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            glm::vec3(
                0.0f,
                -0.45f,
                -1.0f
            )
        );

        model = glm::scale(
            model,
            glm::vec3(
                1.6f,
                0.6f,
                0.9f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
        // ------------------------------------------------
        // DESACTIVAR VAO
        // ------------------------------------------------

        glBindVertexArray(0);

        // ------------------------------------------------
        // MOSTRAR EN PANTALLA
        // ------------------------------------------------

        glfwSwapBuffers(window);
    }

    // --------------------------------------------------
    // LIBERAR RECURSOS
    // --------------------------------------------------

    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteBuffers(
        1,
        &VBO
    );

    glfwTerminate();

    return EXIT_SUCCESS;
}

// ======================================================
// ENTRADAS DEL TECLADO
// ======================================================

void Inputs(GLFWwindow* window)
{
    // ESC = salir
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(
            window,
            true
        );
    }

    // D = mover derecha
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        movX += 0.08f;
    }

    // A = mover izquierda
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        movX -= 0.08f;
    }

    // PAGE UP = subir
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
    {
        movY += 0.08f;
    }

    // PAGE DOWN = bajar
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
    {
        movY -= 0.08f;
    }

    // W = acercar
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        movZ += 0.08f;
    }

    // S = alejar
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        movZ -= 0.08f;
    }

    // Flecha derecha = rotar
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        rot += 0.4f;
    }

    // Flecha izquierda = rotar
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        rot -= 0.4f;
    }
}