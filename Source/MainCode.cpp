#include <iostream>         // error handling and output
#include <cstdlib>          // EXIT_FAILURE

#include <GL/glew.h>        // GLEW library
#include "GLFW/glfw3.h"     // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneManager.h"
#include "ViewManager.h"
#include "ShapeMeshes.h"
#include "ShaderManager.h"

// Namespace for declaring global variables
namespace
{
    // Macro for window title
    const char* const WINDOW_TITLE = "7-1 FinalProject and Milestones";

    // Main GLFW window
    GLFWwindow* g_Window = nullptr;

    // scene manager object for managing the 3D scene prepare and render
    SceneManager* g_SceneManager = nullptr;

    // shader manager object for dynamic interaction with the shader code
    ShaderManager* g_ShaderManager = nullptr;

    // view manager object for managing the 3D view setup and projection to 2D
    ViewManager* g_ViewManager = nullptr;

    
    float yaw = -90.0f;
    float pitch = 0.0f;

    float movementSpeed = 5.0f;
    float mouseSensitivity = 0.1f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    
}

// ============================
// CAMERA STATE (Milestone 3)
// ============================
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 8.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.2f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool usePerspective = true;   // Milestone 3 — projection toggle

// Function declarations - all functions that are called manually
// need to be pre-declared at the beginning of the source code.
bool InitializeGLFW();
bool InitializeGLEW();

// ==========================================================
// ProcessInput() — Keyboard movement (Milestone 3)
// ==========================================================
void ProcessInput(GLFWwindow* window)
{
    float velocity = movementSpeed * deltaTime;

    // Forward / Backward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraFront * velocity;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraFront * velocity;

    // Left / Right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;

    // Up / Down
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos.y += velocity;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos.y -= velocity;

    // Toggle projection mode (Milestone 3)
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        usePerspective = true;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        usePerspective = false;

}

// ==========================================================
// MouseCallback() — Mouse look (Milestone 3)
// ==========================================================
void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    static bool firstMouse = true;
    static float lastX = 400.0f;
    static float lastY = 300.0f;

    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed: y goes down
    lastX = (float)xpos;
    lastY = (float)ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to avoid flipping
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Recalculate cameraFront vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// ==========================================================
// ScrollCallback() — Adjust camera speed (Milestone 3)
// ==========================================================
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    movementSpeed += (float)yoffset;

    // Prevent negative or zero speed
    if (movementSpeed < 1.0f)
        movementSpeed = 1.0f;

    // Optional: cap max speed
    if (movementSpeed > 20.0f)
        movementSpeed = 20.0f;
}


/***********************************************************
 *  main(int, char*)
 *
 *  This function gets called after the application has been
 *  launched.
 ***********************************************************/
int main(int argc, char* argv[])
{
    // if GLFW fails initialization, then terminate the application
    if (InitializeGLFW() == false)
    {
        return(EXIT_FAILURE);
    }

    // try to create a new shader manager object
    g_ShaderManager = new ShaderManager();

    // try to create a new view manager object
    g_ViewManager = new ViewManager(g_ShaderManager);

    // try to create the main display window
    g_Window = g_ViewManager->CreateDisplayWindow(WINDOW_TITLE);

    // Enable mouse-look controls (Milestone 3)
    glfwSetCursorPosCallback(g_Window, MouseCallback);
    // glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable scroll-wheel speed control (Milestone 3)
    glfwSetScrollCallback(g_Window, ScrollCallback);

    // if GLEW fails initialization, then terminate the application
    if (InitializeGLEW() == false)
    {
        return(EXIT_FAILURE);
    }

    // load the shader code from the external GLSL files
    g_ShaderManager->LoadShaders(
        "../../Utilities/shaders/vertexShader.glsl",
        "../../Utilities/shaders/fragmentShader.glsl");
    g_ShaderManager->use();

    // try to create a new scene manager object and prepare the 3D scene
    g_SceneManager = new SceneManager(g_ShaderManager);
    g_SceneManager->PrepareScene();

    // loop will keep running until the application is closed 
    // or until an error has occurred
    while (!glfwWindowShouldClose(g_Window))
    {
        // ============================
        // FRAME TIMING (Milestone 3)
        // ============================
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Enable z-depth
        glEnable(GL_DEPTH_TEST);

        // Clear the frame and z buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Lock cursor AFTER the window is active
        glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Handle keyboard input (we add this function next)
        ProcessInput(g_Window);

        // convert from 3D object space to 2D view
        g_ViewManager->PrepareSceneView();

        // refresh the 3D scene
        g_SceneManager->RenderScene();

        // Flips the the back buffer with the front buffer every frame.
        glfwSwapBuffers(g_Window);

        // query the latest GLFW events
        glfwPollEvents();
    }

    // clear the allocated manager objects from memory
    if (NULL != g_SceneManager)
    {
        delete g_SceneManager;
        g_SceneManager = NULL;
    }
    if (NULL != g_ViewManager)
    {
        delete g_ViewManager;
        g_ViewManager = NULL;
    }
    if (NULL != g_ShaderManager)
    {
        delete g_ShaderManager;
        g_ShaderManager = NULL;
    }

    // Terminates the program successfully
    exit(EXIT_SUCCESS);
}

/***********************************************************
 *  InitializeGLFW()
 *
 *  This function is used to initialize the GLFW library.
 ***********************************************************/
bool InitializeGLFW()
{
    // GLFW: initialize and configure library
    // --------------------------------------
    glfwInit();

#ifdef __APPLE__
    // set the version of OpenGL and profile to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // set the version of OpenGL and profile to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    // GLFW: end -------------------------------

    return(true);
}

/***********************************************************
 *  InitializeGLEW()
 *
 *  This function is used to initialize the GLEW library.
 ***********************************************************/
bool InitializeGLEW()
{
    // GLEW: initialize
    // -----------------------------------------
    GLenum GLEWInitResult = GLEW_OK;

    // try to initialize the GLEW library
    GLEWInitResult = glewInit();
    if (GLEW_OK != GLEWInitResult)
    {
        std::cerr << glewGetErrorString(GLEWInitResult) << std::endl;
        return false;
    }
    // GLEW: end -------------------------------

    // Displays a successful OpenGL initialization message
    std::cout << "INFO: OpenGL Successfully Initialized\n";
    std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << "\n" << std::endl;

    return(true);
}
