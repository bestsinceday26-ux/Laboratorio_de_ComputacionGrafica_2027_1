// Practica 8
// Cadena Palafox Diego Aaron
// Fecha de entrega: 11 de Octubre del 2026
// Numero de cuenta: 419047650

#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include "SOIL2/SOIL2.h"
#include "stb_image.h"

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

int SCREEN_WIDTH, SCREEN_HEIGHT;

void KeyCallback(GLFWwindow* window, int key, int scancode,
    int action, int mode);

void MouseCallback(GLFWwindow* window, double xPos, double yPos);

void DoMovement();

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

bool keys[1024];

GLfloat lastX = 400.0f;
GLfloat lastY = 300.0f;

bool firstMouse = true;

// Animación

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Angulo de la orbita, en radianes.
float rot = 0.0f;

// R inicia y detiene la animacion.
bool activanim = false;

// 1 = Sol, 2 = Luna.
int luzActiva = 1;

// ============================================================
// CONFIGURACION DE LA ORBITA
// ============================================================

// Radio de la trayectoria.
const float radioOrbita = 5.0f;

// Centro de la trayectoria.
const float centroX = 0.0f;
const float centroY = 1.0f;

// Profundidad de los modelos.
const float alturaZ = 0.0f;

// Velocidad angular: 15 grados por segundo.
const float velocidadAngular = glm::radians(15.0f);

// El cambio de iluminacion ocurre al alcanzar 90 grados.
const float anguloCambio = glm::half_pi<float>();

// Permite detectar el cruce del punto de transicion.
bool yaCruzoPuntoCambio = false;

// FUNCION PRINCIPAL

int main()
{

    // INICIALIZAR GLFW
    if (!glfwInit())
    {
        std::cout << "Error al inicializar GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(
        WIDTH,
        HEIGHT,
        "Practica8 DIEGO CADENA",
        nullptr,
        nullptr
    );

    if (window == nullptr)
    {
        std::cout << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(
        window,
        &SCREEN_WIDTH,
        &SCREEN_HEIGHT
    );

    // --------------------------------------------------------
    // REGISTRAR EVENTOS
    // --------------------------------------------------------

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // --------------------------------------------------------
    // INICIALIZAR GLEW
    // --------------------------------------------------------

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error al inicializar GLEW" << std::endl;

        glfwTerminate();
        return EXIT_FAILURE;
    }

    // --------------------------------------------------------
    // CONFIGURAR OPENGL
    // --------------------------------------------------------

    glViewport(
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    glEnable(GL_DEPTH_TEST);

    // --------------------------------------------------------
    // CARGAR SHADERS
    // --------------------------------------------------------

    Shader shader(
        "Shader/modelLoading.vs",
        "Shader/modelLoading.frag"
    );

    Shader lightingShader(
        "Shader/lighting.vs",
        "Shader/lighting.frag"
    );

    // --------------------------------------------------------
    // CARGAR MODELOS
    // --------------------------------------------------------

    Model dog((char*)"Models/RedDog.obj");

    Model apa((char*)"Models/Aparador.obj");

    Model sol((char*)"Models/Sun.obj");

    Model luna((char*)"Models/moon.obj");

    // --------------------------------------------------------
    // MATRIZ DE PROYECCION
    // --------------------------------------------------------

    glm::mat4 projection = glm::perspective(
        camera.GetZoom(),
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
        0.1f,
        100.0f
    );

    // --------------------------------------------------------
    // VAO Y VBO
    // --------------------------------------------------------

    GLuint VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(GLfloat),
        (GLvoid*)(3 * sizeof(GLfloat))
    );

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ========================================================
    // BUCLE PRINCIPAL
    // ========================================================

    while (!glfwWindowShouldClose(window))
    {
        // ----------------------------------------------------
        // ACTUALIZAR TIEMPO
        // ----------------------------------------------------

        GLfloat currentFrame = (GLfloat)glfwGetTime();

        deltaTime = currentFrame - lastFrame;

        lastFrame = currentFrame;

        // Procesar eventos y movimiento.
        glfwPollEvents();

        DoMovement();

        // ----------------------------------------------------
        // LIMPIAR PANTALLA
        // ----------------------------------------------------

        glClearColor(
            0.0f,
            0.0f,
            0.0f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );

        // ====================================================
        // CALCULAR POSICIONES DEL SOL Y LA LUNA
        // ====================================================

        // El Sol recorre la orbita.
        glm::vec3 posicionSol(
            centroX + radioOrbita * std::cos(rot),
            centroY + radioOrbita * std::sin(rot),
            alturaZ
        );

        // La Luna se encuentra al lado opuesto del Sol.
        glm::vec3 posicionLuna(
            centroX + radioOrbita * std::cos(
                rot + glm::pi<float>()
            ),
            centroY + radioOrbita * std::sin(
                rot + glm::pi<float>()
            ),
            alturaZ
        );

        // ====================================================
        // CONFIGURAR SHADER DE ILUMINACION
        // ====================================================

        lightingShader.Use();

        // ----------------------------------------------------
        // POSICIONES DE LAS FUENTES DE LUZ
        // ----------------------------------------------------

        // Luz 1: asociada al Sol.
        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "light.position"
            ),
            posicionSol.x,
            posicionSol.y,
            posicionSol.z
        );

        // Luz 2: asociada a la Luna.
        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "light2.position"
            ),
            posicionLuna.x,
            posicionLuna.y,
            posicionLuna.z
        );

        // Seleccionar la fuente activa.
        glUniform1i(
            glGetUniformLocation(
                lightingShader.Program,
                "luzActiva"
            ),
            luzActiva
        );

        // ----------------------------------------------------
        // POSICION DE LA CAMARA
        // ----------------------------------------------------

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "viewPos"
            ),
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z
        );

        // ----------------------------------------------------
        // PROPIEDADES DE LA LUZ 1: SOL
        // ----------------------------------------------------

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "light.ambient"
            ),
            0.55f,
            0.50f,
            0.30f
        );

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "light.diffuse"
            ),
            1.0f,
            0.80f,
            0.30f
        );

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "light.specular"
            ),
            0.55f,
            0.95f,
            0.65f
        );

        // ----------------------------------------------------
        // PROPIEDADES DE LA LUZ 2: LUNA
        // ----------------------------------------------------

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "light2.ambient"
            ),
            0.55f,
            0.65f,
            1.0f
        );

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "light2.diffuse"
            ),
            0.55f,
            0.65f,
            1.0f
        );

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "light2.specular"
            ),
            0.55f,
            0.65f,
            1.0f
        );

        // ----------------------------------------------------
        // MATRICES DE CAMARA
        // ----------------------------------------------------

        glm::mat4 view = camera.GetViewMatrix();

        glUniformMatrix4fv(
            glGetUniformLocation(
                lightingShader.Program,
                "projection"
            ),
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(
                lightingShader.Program,
                "view"
            ),
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );

        // PERRO
        // Material del perro.
        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "material.ambient"
            ),
            0.2f,
            0.2f,
            0.2f
        );

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "material.diffuse"
            ),
            0.5f,
            0.5f,
            0.5f
        );

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "material.specular"
            ),
            1.0f,
            1.0f,
            1.0f
        );

        glUniform1f(
            glGetUniformLocation(
                lightingShader.Program,
                "material.shininess"
            ),
            128.0f
        );

        glm::mat4 modelDog(1.0f);

        modelDog = glm::scale(
            modelDog,
            glm::vec3(3.0f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(
                lightingShader.Program,
                "model"
            ),
            1,
            GL_FALSE,
            glm::value_ptr(modelDog)
        );

        dog.Draw(lightingShader);

        // MOSTRADOR 
        glUniform1i(
            glGetUniformLocation(
                lightingShader.Program,
                "luzActiva"
            ),
            luzActiva
        );

        // Material del mostrador.
        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "material.ambient"
            ),
            0.3f,
            0.3f,
            0.3f
        );

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "material.diffuse"
            ),
            1.0f,
            1.0f,
            1.0f
        );

        glUniform3f(
            glGetUniformLocation(
                lightingShader.Program,
                "material.specular"
            ),
            0.25f,
            0.25f,
            0.25f
        );

        glUniform1f(
            glGetUniformLocation(
                lightingShader.Program,
                "material.shininess"
            ),
            32.0f
        );

        glm::mat4 modelApa(1.0f);

        modelApa = glm::scale(
            modelApa,
            glm::vec3(3.0f)
        );

        modelApa = glm::translate(
            modelApa,
            glm::vec3(0.0f, 0.06f, 0.0f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(
                lightingShader.Program,
                "model"
            ),
            1,
            GL_FALSE,
            glm::value_ptr(modelApa)
        );

        apa.Draw(lightingShader);

        // SOL
     
        shader.Use();

        glUniformMatrix4fv(
            glGetUniformLocation(
                shader.Program,
                "projection"
            ),
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(
                shader.Program,
                "view"
            ),
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );

        glm::mat4 modelSol(1.0f);

        modelSol = glm::translate(
            modelSol,
            posicionSol
        );

        modelSol = glm::rotate(
            modelSol,
            rot,
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        modelSol = glm::scale(
            modelSol,
            glm::vec3(2.0f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(
                shader.Program,
                "model"
            ),
            1,
            GL_FALSE,
            glm::value_ptr(modelSol)
        );

        sol.Draw(shader);

        // LUNA
  
        glm::mat4 modelLuna(1.0f);

        modelLuna = glm::translate(
            modelLuna,
            posicionLuna
        );

        modelLuna = glm::rotate(
            modelLuna,
            rot,
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        modelLuna = glm::scale(
            modelLuna,
            glm::vec3(2.0f)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(
                shader.Program,
                "model"
            ),
            1,
            GL_FALSE,
            glm::value_ptr(modelLuna)
        );

        luna.Draw(shader);

        // ----------------------------------------------------
        // FINALIZAR EL FOTOGRAMA
        // ----------------------------------------------------

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // ========================================================
    // LIBERAR RECURSOS
    // ========================================================

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return EXIT_SUCCESS;
}

void DoMovement()
{
    // --------------------------------------------------------
    // MOVIMIENTO DE LA CAMARA
    // --------------------------------------------------------

    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // --------------------------------------------------------
    // ANIMACION Y CAMBIO DE ILUMINACION
    // --------------------------------------------------------

    if (activanim)
    {
        float anguloAnterior = rot;

        // Movimiento antihorario.
        rot += velocidadAngular * deltaTime;

        // Mantener el angulo dentro de [0, 2 PI).
        if (rot >= glm::two_pi<float>())
        {
            rot = std::fmod(
                rot,
                glm::two_pi<float>()
            );
        }

        float anguloSol = rot;

        float anguloLuna = rot + glm::pi<float>();

        if (anguloLuna >= glm::two_pi<float>())
        {
            anguloLuna -= glm::two_pi<float>();
        }

        // Cuando el Sol cruza el punto de transicion,
        // activar la fuente asociada a la Luna.
        if (rot < glm::pi<float>())
        {
            luzActiva = 1;
        }

        // Al iniciar una nueva vuelta, volver a la luz del Sol.
        else
        {
            luzActiva = 2;
        }
    }
}

// ============================================================
// CALLBACK DEL TECLADO
// ============================================================

void KeyCallback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mode
)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
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

    // R inicia o detiene la animacion.
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        activanim = !activanim;
    }
}

// ============================================================
// CALLBACK DEL MOUSE
// ============================================================

void MouseCallback(
    GLFWwindow* window,
    double xPos,
    double yPos
)
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

    camera.ProcessMouseMovement(
        xOffset,
        yOffset
    );
}