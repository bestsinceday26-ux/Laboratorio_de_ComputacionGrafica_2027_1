// Práctica 3
// Cadena Palafox Diego Aaron
// Fecha de entrega: 06 de Septiembre del 2026
// Número de cuenta: 419047650

#include <iostream>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;


int main()
{
   
    // INICIALIZACIÓN DE GLFW
   

    glfwInit();

    // La ventana no podrá cambiar de tamaño
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        "Práctica 3 Diego Cadena",
        nullptr,
        nullptr
    );

    int screenWidth, screenHeight;

    glfwGetFramebufferSize(
        window,
        &screenWidth,
        &screenHeight
    );

    // Verificación de creación de ventana
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    // INICIALIZACIÓN DE GLEW
  

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialise GLEW" << std::endl;

        return EXIT_FAILURE;
    }

    // CONFIGURACIÓN DEL VIEWPORT
 
    glViewport(
        0,
        0,
        screenWidth,
        screenHeight
    );

    // Activar profundidad
    glEnable(GL_DEPTH_TEST);

    // Activar transparencia
    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    // SHADERS
   

    Shader ourShader(
        "Shader/core.vs",
        "Shader/core.frag"
    );


    GLfloat vertices[] =
    {
       
        // CARA FRONTAL - ROJO

        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

 
        // CARA TRASERA - VERDE
      
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,

         0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,


        // CARA DERECHA - AZUL

         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

         0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

         // CARA IZQUIERDA - AMARILLO

         -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,

         -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

         // CARA INFERIOR - CIAN

         -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,

          0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,


         // CARA SUPERIOR - ROSA

         -0.5f,  0.5f, -0.5f,   1.0f, 0.2f, 0.5f,
          0.5f,  0.5f, -0.5f,   1.0f, 0.2f, 0.5f,
          0.5f,  0.5f,  0.5f,   1.0f, 0.2f, 0.5f,

          0.5f,  0.5f,  0.5f,   1.0f, 0.2f, 0.5f,
         -0.5f,  0.5f,  0.5f,   1.0f, 0.2f, 0.5f,
         -0.5f,  0.5f, -0.5f,   1.0f, 0.2f, 0.5f
    };


    //
    // CREACIÓN DEL VAO Y VBO
    

    GLuint VBO, VAO;

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);


    // Activar VAO
    glBindVertexArray(VAO);


    // Activar VBO
    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );


    // Copiar los vértices al VBO
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    
    // ATRIBUTO DE POSICIÓN
  

    glVertexAttribPointer(
        0,                          // location
        3,                          // X Y Z
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),        // stride
        (GLvoid*)0
    );

    glEnableVertexAttribArray(0);

    // 8. ATRIBUTO DE COLOR
 
    glVertexAttribPointer(
        1,                          // location
        3,                          // R G B
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    // PROYECCIÓN ORTOGRÁFICA
    

    glm::mat4 projection = glm::mat4(1.0f);

    projection = glm::ortho(
        0.0f,
        (GLfloat)screenWidth,
        0.0f,
        (GLfloat)screenHeight,
        0.1f,
        2000.0f
    );

    // 10. BUCLE PRINCIPAL

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(
            0.15f,
            0.15f,
            0.15f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );


      
        // ACTIVAR SHADER
      
        ourShader.Use();

        glm::mat4 view = glm::mat4(1.0f);

        // Movemos el origen al centro de la pantalla
        view = glm::translate(
            view,
            glm::vec3(
                screenWidth / 2.0f,
                screenHeight / 2.0f,
                -800.0f
            )
        );



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


        // Enviar View y Projection
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


        glBindVertexArray(VAO);


       
        // CAJA 1 - INFERIOR IZQUIERDA
       

        glm::mat4 model1 = glm::mat4(1.0f);

        // Posición
        model1 = glm::translate(model1,glm::vec3(-100.0f,-170.0f,0.0f));
        // Rotación
        model1 = glm::rotate(model1,glm::radians(180.0f),glm::vec3(-10.0f, -10.0f, 90.0f));

        // Escala
        model1 = glm::scale(model1,glm::vec3(170.0f,150.0f,170.0f));

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model1)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );


        // CAJA 2 - INFERIOR DERECHA


        glm::mat4 model2 = glm::mat4(1.0f);

        model2 = glm::translate(
            model2,
            glm::vec3(
                125.0f,
                -170.0f,
                0.0f
            )
        );

        model2 = glm::rotate(model2, glm::radians(195.0f), glm::vec3(175.0f,-25.0f, 140.0f));

        model2 = glm::scale(
            model2,
            glm::vec3(
                170.0f,
                150.0f,
                170.0f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model2)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );
  
        // CAJA 3 - CENTRO


        glm::mat4 model3 = glm::mat4(1.0f);

        model3 = glm::translate(
            model3,
            glm::vec3(
                0.0f,
                15.0f,
                40.0f
            )
        );

        model3 = glm::rotate(
            model3,
            glm::radians(-90.0f),
            glm::vec3(165.0f, 78.0f, -26.0f)
        );

        model3 = glm::rotate(
            model3,
            glm::radians(60.0f),
            glm::vec3(224.0f, -85.0f, 224.0f)
        );

        model3 = glm::scale(
            model3,
            glm::vec3(
                145.0f,
                145.0f,
                145.0f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model3)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );

        // CAJA 4 - SUPERIOR IZQUIERDA
  

        glm::mat4 model4 = glm::mat4(1.0f);

        model4 = glm::translate(
            model4,
            glm::vec3(
                -90.0f,
                205.0f,
                20.0f
            )
        );

        model4 = glm::rotate(
            model4,
            glm::radians(-25.0f),
            glm::vec3(1.0f, 0.0f, 10.0f)
        );

        model4 = glm::rotate(
            model4,
            glm::radians(85.6f),
            glm::vec3(36.0f, 28.0f, 35.0f)
        );

        model4 = glm::scale(
            model4,
            glm::vec3(
                125.0f,
                115.0f,
                125.0f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model4)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );



        // CAJA 5 - SUPERIOR DERECHA


        glm::mat4 model5 = glm::mat4(1.0f);

        model5 = glm::translate(
            model5,
            glm::vec3(
                115.0f,
                136.0f,
                10.0f
            )
        );

        model5 = glm::rotate(
            model5,
            glm::radians(90.0f),
            glm::vec3(-5.0f, 0.0f, 10.0f)
        );

        model5 = glm::rotate(
            model5,
            glm::radians(180.0f),
            glm::vec3(66.0f, 10.0f, 0.0f)
        );

        model5 = glm::scale(
            model5,
            glm::vec3(
                125.0f,
                115.0f,
                125.0f
            )
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model5)
        );

        glDrawArrays(
            GL_TRIANGLES,
            0,
            36
        );


        // ========================================================
        // DESACTIVAR VAO
        // ========================================================

        glBindVertexArray(0);


        // ========================================================
        // MOSTRAR EL FRAME
        // ========================================================

        glfwSwapBuffers(window);
    }


    // ============================================================
    // LIBERAR RECURSOS
    // ============================================================

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
      