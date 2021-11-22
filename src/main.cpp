#include "glheader.h"
#include "Model.h"
#include "Player.h"

// imgui
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <filesystem>
#include <vector>
#include <stdlib.h>
#include <ctime>
#define Randmod(x) rand()%x

// Input 
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
// Calculate transform
void calculatePlayerTransform(glm::vec4 & modelMat);
void calculateObjectTransform(glm::mat4& modelMat);
glm::mat4 calculateNpcTransform();
// Set shader
void setPlayerShader(Shader & shader);
void setObjectShader(Shader & shader);
// Player actions
void playerAttackZ();
void playerJump();

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// Camera
Camera camera(glm::vec3(0, 1.3f, 9.0f), glm::radians(-7.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Input Declare
float lastX;
float lastY;
bool firstMouse = true;

// Player 
Player player;

// Player transform
bool playerDir = true; 
float stepX = 0.0f;
float stepY = 0.09f;
int jumpCount = 50;
int attackCount = 0;

// MVP
glm::mat4 modelMatPlayer(1.0f); // model matrix for player
glm::mat4 modelMatObject(1.0f); // model matrix for objects that stay still
vector<glm::mat4> modelMatNpc; // model matrix for npc
glm::mat4 viewMat(1.0f);
glm::mat4 projMat(1.0f);


// One-time Press keyboard input
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        playerAttackZ();
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        playerJump();
}

// Process keyboard input
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
        camera.speedX = -1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.speedX = 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.speedY = -1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.speedY = 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        stepX += 0.05f;
        player.updatePlayerDirection(true);
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        stepX -= 0.05f;
        player.updatePlayerDirection(false);
    }
    else
    {
        camera.speedX = 0;
        camera.speedY = 0;
        camera.speedZ = 0;
    }
}

// Mouse control camera
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

// Calculate the model matrix of player
void calculatePlayerTransform(glm::mat4& modelMat)
{
    // update player position
    // if player is jumping
    if(player.ifPlayerJumping())
    {
        if(jumpCount > 25){
            stepY += 0.03f;
            --jumpCount;
        } else if (jumpCount > 0) {
            stepY -= 0.03f;
            --jumpCount;
        } else {
            player.updatePlayerJump(false);
            jumpCount = 50;
        }
    }
    player.updatePlayerPosition(stepX, stepY);

    // translate
    modelMat = glm::translate(glm::mat4(1.0f), player.getPlayerPosition());

    // scale
    glm::vec3 scaleMatrix;
    if (player.getPlayerDirection()) // going right
    {
        scaleMatrix = glm::vec3(0.2f, 0.2f, 0.2f);
    }
    else {
        scaleMatrix = glm::vec3(-0.2f, 0.2f, 0.2f);
    }
    modelMat = glm::scale(modelMat, scaleMatrix);
}

// Calculate the model matrix of object
void calculateObjectTransform(glm::mat4& modelMat)
{
    // translate
    modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,  -0.7f,  0.0f));

    // scale
    glm::vec3 scaleMatrix = glm::vec3(0.15f, 0.15f, 0.15f);
    modelMat  = glm::scale(modelMat, scaleMatrix);

}

// Calculate the model matrix of npc
glm::mat4 calculateNpcTransform()
{
    // translate
    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f + 0.1f * float(Randmod(5)),  0.09f,  0.0f));
    // scale
    glm::vec3 scaleMatrix = glm::vec3(-0.2f, 0.2f, 0.2f);
    modelMat  = glm::scale(modelMat, scaleMatrix);
    return modelMat;
}

void setPlayerShader(Shader & shader)
{
    // MVP
    shader.setVec3("viewPos", camera.Position);
    shader.setMat4("modelMat", modelMatPlayer);
    shader.setMat4("viewMat", viewMat);
    shader.setMat4("projMat", projMat);
    // Lighting
    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setFloat("dirLight.ambient", 0.5f);
    shader.setFloat("dirLight.diffuse", 0.4f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
}

void setObjectShader(Shader & shader)
{
    // MVP
    shader.setVec3("viewPos", camera.Position);
    shader.setMat4("modelMat", modelMatObject);
    shader.setMat4("viewMat", viewMat);
    shader.setMat4("projMat", projMat);
    // Lighting
    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setFloat("dirLight.ambient", 0.5f);
    shader.setFloat("dirLight.diffuse", 0.4f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
}

void playerAttackZ()
{
    if(!player.ifPlayerAttacking())
    {
        player.updatePlayerAttack(true);
        attackCount++;
        player.updatePlayerAttack(false);
    }
}

void playerJump()
{
    if(!player.ifPlayerJumping())
    {
        player.updatePlayerJump(true);
    }
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
    // glfwSetCursorPosCallback(window, mouse_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Open deep test
    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(false);

    // Init shader
    Shader playerShader("../src/Shaders/character.vert", "../src/Shaders/character.frag");
    Shader objectShader("../src/Shaders/object.vert", "../src/Shaders/object.frag");

    // MVP
    projMat = glm::perspective(glm::radians(30.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // Load model
    Model character(filesystem::path("/Users/manmantou/Desktop/Study/TCD/ComputerGraphics/Lab1/src/Models/character1/character.obj"));
    Model ground(filesystem::path("/Users/manmantou/Desktop/Study/TCD/ComputerGraphics/Lab1/src/Models/ground/ground.obj"));
    std::vector<Model> npc;
    for (int i = 0; i < 3; i++){
        npc.push_back(Model(filesystem::path("/Users/manmantou/Desktop/Study/TCD/ComputerGraphics/Lab1/src/Models/character2/character2.obj")));
    }

    // Npc
    for(int i = 0; i < npc.size(); i++)
    {
        modelMatNpc.push_back(calculateNpcTransform());
    }

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug");
        ImGui::Text("Hello Kongfu");
        ImGui::TextColored(ImVec4(1,1,0,1), "Player Action");
        ImGui::BeginChild("Scrolling");

        // Input
        processInput(window);
        glfwSetKeyCallback(window, keyCallback);

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera position
        viewMat = camera.GetViewMatrix();
        
        // Ground
        calculateObjectTransform(modelMatObject);
        objectShader.use();
        setObjectShader(objectShader);
        ground.Draw(objectShader);

        // Player
        calculatePlayerTransform(modelMatPlayer);
        playerShader.use();
        setPlayerShader(playerShader);
        character.Draw(playerShader);

        // Display attack info (temp)
        for (int i = 0; i < attackCount; i++)
        {
            ImGui::Text("Attack~");
        }

        // Npc
        for (int i = 0; i < npc.size(); i++)
        {
            objectShader.use();
            objectShader.setMat4("modelMat", modelMatNpc[i]);
            npc[i].Draw(objectShader);
        }

        ImGui::EndChild();
        ImGui::End();
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
        camera.SetCameraPos(player.getPlayerPosition());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}