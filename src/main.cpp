#include "glheader.h"
#include "Model.h"
#include <filesystem>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;


// Camera
Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(15.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Input Declare
float lastX;
float lastY;
bool firstMouse = true;

// Move character
float step = 0.0f;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.speedZ = 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.speedZ = -1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        step = step - 0.1f;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        step = step + 0.1f;
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.speedY = -1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.speedY = 1.0f;
    }
    else
    {
        camera.speedX = 0;
        camera.speedY = 0;
        camera.speedZ = 0;
    }
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse == true)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    float xOffset, yOffset;
    xOffset = xPos - lastX;
    yOffset = yPos - lastY;
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement(xOffset, yOffset);
}

int main(int argc, const char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif

    // Create a window
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "fail to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Open deep test
    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(true);

    // Init shader
    Shader myShader("../src/Shaders/character.vert", "../src/Shaders/character.frag");
    // Shader* triangleShader2 = new Shader("../src/Shaders/vertexShaderSource.vert", "../src/Shaders/fragmentShaderSource2.frag");

    // Load model
    Model character(filesystem::path("/Users/manmantou/Desktop/Study/TCD/ComputerGraphics/Lab1/src/Models/donut/Donut.obj"));

    glm::mat4 modelMat(1.0f);
    glm::mat4 viewMat(1.0f);
    glm::mat4 projMat(1.0f);
    projMat = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    
    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera position
        viewMat = camera.GetViewMatrix();

        // MVP
        modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(step,  0.0f,  0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
        myShader.use();
        myShader.setVec3("viewPos", camera.Position);
        myShader.setMat4("modelMat", modelMat);
        myShader.setMat4("viewMat", viewMat);
        myShader.setMat4("projMat", projMat);

        // Lighting
        // Directional light
        myShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        myShader.setFloat("dirLight.ambient", 0.5f);
        myShader.setFloat("dirLight.diffuse", 0.4f);
        myShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // Model
        character.Draw(myShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
        camera.UpdateCameraPos();
    }

    glfwTerminate();
    return 0;
}