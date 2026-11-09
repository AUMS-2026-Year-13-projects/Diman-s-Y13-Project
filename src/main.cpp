#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include "shaders.hpp"
#include "camera.hpp"
#include "perlin.hpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH/2;
float lastY = SCR_HEIGHT/2;

Camera camera = Camera(90.0f, glm::vec3(0,0,5.0f));

// g++ src/main.cpp src/glad.c src/utils/shaders.cpp src/utils/camera.cpp src/utils/perlin.cpp include/imgui/imgui.cpp include/imgui/imgui_draw.cpp include/imgui/imgui_tables.cpp include/imgui/imgui_widgets.cpp include/imgui/backends/imgui_impl_glfw.cpp include/imgui/backends/imgui_impl_opengl3.cpp -Iinclude/ -Iinclude/imgui/ -Iinclude/imgui/backends/ -Llib -lglfw3 -lopengl32 -lgdi32 -o main.exe && main.exe

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.direction = glm::normalize(direction);
}

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.WantCaptureMouse = true;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //style.FontSizeBase = 16.0f;           // Default to 13.0f when unspecified, which allows crisp legacy font for non-4K screens.
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = nullptr;
    ImGui_ImplOpenGL3_Init(glsl_version);

    Shader testShader = Shader("src/shaders/vertex.glsl","src/shaders/frag.glsl");
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    // float vertices[] = {
    //     -0.5f, 0, -0.5f,
    //      0.5f, 0, -0.5f,
    //      0.5f, 0, 0.5f,
    //      0.5f, 0, 0.5f, 
    //     -0.5f, 0, 0.5f,
    //     -0.5f, 0, -0.5f,
    // };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera.fov), 800.0f/600.0f, 0.1f, 100.0f);
    glm::mat4 view;
    view = glm::lookAt(camera.position,camera.position + camera.direction, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glCullFace(GL_BACK);
    // glfwSetCursorPosCallback(window, mouse_callback);  
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    float angle = 0;
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    
    static float cell_size = 1;
    float octaves = 1;
    float seed = 1;

    std::vector<float> saved_world;
    
    // for (int x=camera.position.x-50; x<camera.position.x+50; x+=1) {
    //     for (int z=camera.position.z-50; z<camera.position.z+50; z+=1) {
    //         glm::vec2 chunk_coords = glm::vec2((int)x/8,(int)z/8);
    //     }
    // }

    bool toggle_mouse = false;
    float max_height = 1.0f;
    float percistence = 0.1f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame; 
        float speed = 5.5f * deltaTime;

        view = glm::lookAt(camera.position,camera.position + camera.direction, glm::vec3(0.0f, 1.0f, 0.0f));
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            glm::vec3 tempDir = glm::normalize(glm::vec3(camera.direction.x, 0, camera.direction.z));
            camera.position += (tempDir * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            glm::vec3 tempDir = glm::normalize(glm::vec3(camera.direction.x, 0, camera.direction.z));
            camera.position -= (tempDir * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.position += (camera.right * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.position -= (camera.right * speed);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.position.y += speed;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            camera.position.y -= speed;
        }
        // if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        //     if (toggle_mouse == true) {
        //         glfwSetCursorPosCallback(window, mouse_callback);
        //         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
        //     } else {
        //         glfwSetCursorPosCallback(window, nullptr);  
        //         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
        //     }
        //     toggle_mouse = !toggle_mouse;
        // }

        camera.right = glm::normalize(glm::cross(camera.direction, glm::vec3(0,1,0)));
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        for (int x=camera.position.x-50; x<camera.position.x+50; x+=1) {
            for (int z=camera.position.z-50; z<camera.position.z+50; z+=1) {
                glm::vec2 chunk_coords = glm::vec2((int)x/8,(int)z/8);
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(x,octavesperlin(x,z,cell_size, octaves, percistence, seed)*max_height,z));
                // std::cout << perlina(x,z)*10 << std::endl;
                // model = glm::translate(model, glm::vec3(x,1,z));
                glUseProgram(testShader.shaderProgram);
                testShader.setUniform("model", model);
                testShader.setUniform("projection",projection);
                testShader.setUniform("view", view);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Perlin noise configuration");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::SliderFloat("Scale", &cell_size, 1.0f, 100.0f);
        ImGui::SliderFloat("Octaves", &octaves, 1.0f, 10.0f);
        ImGui::SliderFloat("Max height", &max_height, 1.0f, 100.0f);
        ImGui::SliderFloat("Percistence", &percistence, 1.0f, 64.0f);
        ImGui::SliderFloat("Seed", &seed, 1.0f, 64.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(testShader.shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}