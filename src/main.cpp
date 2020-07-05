#pragma once

#include <glad/glad.h>	   // cross-platform opengl loader
#include <GLFW/glfw3.h>	   // winapi window generator
#include "spdlog/spdlog.h" // logger

// matrix/vector utilities
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

int main()
{
	// Initialize GLFW
	spdlog::info("Initializing GLFW");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Main GLFW window object
	auto window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "OpenGL window wow", NULL, NULL);
	if (window == NULL)
	{
		spdlog::critical("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD (system specific header loader)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		spdlog::critical("Failed to initialize GLAD");
		return -1;
	}

	// Compile shaders
	Shader shader("src/shader.vert", "src/shader.frag");

	// Vertex data, buffers, attribues
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	  // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f	  // top left
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3};

	// Generate vertex buffer object
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind vertex array object, then bind vertex buffer(s) then configure attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load and create textures
	Texture tex1("assets/dog.jpeg", GL_RGB);
	Texture tex2("assets/dog_with_hat.png", GL_RGBA);

	// assign textures to uniforms
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // resize viewport on window resize
	glfwSetKeyCallback(window, inputKeyCallback);					 // key input logging

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe

	// Render loop
	spdlog::info("Init success, entering render loop");
	while (!glfwWindowShouldClose(window)) // check if window should still be open
	{
		// Input
		processInput(window);
		float timeValue = (float)glfwGetTime();

		// Render
		glClearColor(sin(-timeValue * 2.0f) / 2.0f + 0.2f, sin(-timeValue * 0.5f) / 2.0f + 0.3f, sin(-timeValue * 3.0f) / 2.0f + 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2.ID);
		shader.use();
		shader.setFloat("blend_amount", sin(timeValue));

		glm::mat4 trans = glm::mat4(1.0f); // init matrix to identity matrix

		// first thing
		trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));									 // move
		trans = glm::rotate(trans, glm::radians(sin(timeValue) * 360.0f), glm::vec3(0.0, 0.0, 1.0)); // rotate
		trans = glm::scale(trans, glm::vec3(1.5, 1.5, 1.5));										 // scale

		auto transformLoc = glGetUniformLocation(shader.ID, "transform"); // get transform uniform id
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// draw second container
		trans = glm::mat4(1.0f);									  // reset matrix
		trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f)); // move
		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));		  //scale
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Poll events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		spdlog::info("Exiting application from user input");
		glfwSetWindowShouldClose(window, true);
	}
}

// Resize viewport on window size change
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	auto key_name = glfwGetKeyName(key, scancode);
	const char *action_name[3] = {"PRESS", "RELEASE", "REPEAT"};
	if (key_name)
	{
		spdlog::info("Input key event: {} {}", key_name, action_name[action]);
	}
	else
	{
		spdlog::info("Input key event: {} {}", key, action_name[action]);
	}
}