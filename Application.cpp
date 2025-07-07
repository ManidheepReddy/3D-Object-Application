#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

int main(void)
{
    GLFWwindow* window;

    // Initialize GLFW
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create window
    window = glfwCreateWindow(960, 540, "3D Cube Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make context current and enable vsync
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialization failed" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

   
    GLCall(glEnable(GL_DEPTH_TEST)); 
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    float vertices[] = {
        // Front face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        // Back face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        // Right face
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         // Bottom face
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    //cube indices (6 faces, 2 triangles each, 36 indices)
    unsigned int indices[] = {
        0, 1, 2,  2, 3, 0,     // Front
        4, 5, 6,  6, 7, 4,     // Back
        8, 9, 10, 10, 11, 8,   // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Bottom
        20, 21, 22, 22, 23, 20  // Top
    };

   
    VertexArray vao;
    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.Push<float>(3); 
    layout.Push<float>(3); 
    layout.Push<float>(2); 
    vao.AddBuffer(vb, layout);
    IndexBuffer ibo(indices, 36);

    
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f), 
        glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f)  
    );

   
    Shader shader("OpenGL\\res\\shaders\\vertex.shader", "OpenGL\\res\\shaders\\fragment.shader");
    shader.Bind();

    
    Texture texture("OpenGL\\res\\textures\\Flower.png");
    texture.Bind(0); 
    shader.SetUniform1i("u_Texture", 0);

    // ImGui 1.60
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    // Cube transformation and lighting parameters
    float position[3] = { 0.0f, 0.0f, 0.0f };
    float rotation = 0.0f;
    float scale = 1.0f;
    float lightDir[3] = { 1.0f, 1.0f, 1.0f };
    float lightColor[3] = { 1.0f, 1.0f, 1.0f };
    bool wireframe = false;

    Renderer renderer;

   
    while (!glfwWindowShouldClose(window))
    {
        
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

       
        ImGui_ImplGlfwGL3_NewFrame();

       
        ImGui::Begin("Cube Controls");
        ImGui::SliderFloat3("Position", position, -5.0f, 5.0f);
        ImGui::SliderFloat("Rotation (Y-axis)", &rotation, 0.0f, 360.0f);
        ImGui::SliderFloat("Scale", &scale, 0.1f, 2.0f);
        ImGui::SliderFloat3("Light Direction", lightDir, -1.0f, 1.0f);
        ImGui::ColorEdit3("Light Color", lightColor);
        ImGui::Checkbox("Wireframe Mode", &wireframe);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();

        
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position[0], position[1], position[2]));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
        model = glm::scale(model, glm::vec3(scale));
        glm::mat4 mvp = proj * view * model;

        
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniformMat4f("u_Model", model);
        glm::vec3 normalizedLightDir = glm::normalize(glm::vec3(lightDir[0], lightDir[1], lightDir[2]));
        shader.SetUniform3f("u_LightDir", normalizedLightDir.x, normalizedLightDir.y, normalizedLightDir.z);
        shader.SetUniform3f("u_LightColor", lightColor[0], lightColor[1], lightColor[2]);
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        // Set wireframe or fill mode
        if (wireframe)
        {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        }
        else
        {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        }
        
        renderer.Draw(vao, ibo, shader);

        
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        
        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }

    
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
