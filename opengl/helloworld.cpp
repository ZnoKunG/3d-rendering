#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

const int screenWidth = 800;
const int screenHeight = 600;

//glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 camFront = -glm::normalize(camPos);
//glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
//const float camSpeed = 20.0f;
//float fov = 45.0f;

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const float sensitivity = 0.05f;
float yaw = -90.0f, pitch = 0.0f;
float lastMousePosX = (float)screenWidth / 2.0f;
float lastMousePosY = (float)screenHeight / 2.0f;
bool firstMoveMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//camPos += camFront * camSpeed * deltaTime;
		cam.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//camPos += camFront * -camSpeed * deltaTime;
		cam.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//camPos += glm::cross(camFront, camUp) * camSpeed * deltaTime;
		cam.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//camPos += glm::cross(camFront, camUp) * -camSpeed * deltaTime;
		cam.ProcessKeyboard(LEFT, deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xPosDouble, double yPosDouble)
{
	float xPos = static_cast<float>(xPosDouble);
	float yPos = static_cast<float>(yPosDouble);

	if (firstMoveMouse)
	{
		lastMousePosX = xPos;
		lastMousePosY = yPos;
		firstMoveMouse = false;
	}

	float offsetX = xPos - lastMousePosX;
	float offsetY = yPos - lastMousePosY;
	lastMousePosX = xPos;
	lastMousePosY = yPos;

	cam.ProcessMouseMovement(offsetX, -offsetY);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	/*fov -= (float)yOffset;
	if (fov < 1.0f)
	{
		fov = 1.0f;
	}
	if (fov > 45.0f)
	{
		fov = 45.0f;
	}*/

	cam.ProcessMouseScroll(static_cast<float>(yOffset));
}

glm::mat4 lookAt(glm::vec3 camPos, glm::vec3 camTarget, glm::vec3 up)
{
	glm::mat4 rotation = glm::mat4(1.0f);
	glm::vec3 inversedCamDir = glm::normalize(camPos - camTarget);
	glm::vec3 camRight = glm::normalize(glm::cross(up, inversedCamDir));
	glm::vec3 camUp = glm::normalize(glm::cross(inversedCamDir, camRight));

	rotation[0][0] = camRight.x;
	rotation[1][0] = camRight.y;
	rotation[2][0] = camRight.z;

	rotation[0][1] = camUp.x;
	rotation[1][1] = camUp.y;
	rotation[2][1] = camUp.z;

	rotation[0][2] = inversedCamDir.x;
	rotation[1][2] = inversedCamDir.y;
	rotation[2][2] = inversedCamDir.z;

	glm::mat4 translateCam = glm::mat4(1.0f);
	translateCam[3][0] = -camPos.x;
	translateCam[3][1] = -camPos.y;
	translateCam[3][2] = -camPos.z;

	glm::mat4 lookAt = rotation * translateCam;
	return lookAt;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Try OpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader shader("shader.vs", "shader.fs");

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

	unsigned int indices[] = {
		0, 1, 3, 1, 2, 3,
	};

	float colors[3 * 6 * 6] = {};

	float offset[] = {
		0.0f, 0.0f, 0.0f,
	};

	float textures[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,

	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Load and Create Texture
	// -----------------------
	stbi_set_flip_vertically_on_load(true);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set texture wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::IMAGE::LOADING" << std::endl;
	}

	stbi_image_free(data);

	shader.use();
	shader.setInt("_Texture1", 0);
	// -----------------------

	// Model View Projection
	// ----------------------
	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(cam.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

	unsigned int modelLoc, viewLoc, projLoc;
	modelLoc = glGetUniformLocation(shader.ID, "model");
	viewLoc = glGetUniformLocation(shader.ID, "view");
	projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	// ----------------------

	unsigned int VBO_pos;
	glGenBuffers(1, &VBO_pos);

	unsigned int VBO_color;
	glGenBuffers(1, &VBO_color);

	unsigned int VBO_texture;
	glGenBuffers(1, &VBO_texture);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Copy vertices array into a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	// Set position vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set color vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	// Set texture vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	// Copy indices array into a buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		float time = glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		for (int i = 0; i < sizeof(colors) / sizeof(float); i += 3)
		{
			colors[i] = (sin(time) * 0.5f) + 0.5f;
			colors[i + 1] = (cos(time) * 0.5f) + 0.5f;
			colors[i + 2] = (sin(time) * 0.5f) + 0.5f;
		}

		// Set new color data
		glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Draw Triangle
		shader.use();
		glBindVertexArray(VAO);

		// projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		projection = glm::perspective(glm::radians(cam.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		// view = glm::lookAt(camPos, camPos + camFront, camUp);
		view = cam.GetViewMatrix();
		shader.setMat4("view", view);

		for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(22.5f) * (float)time,
				glm::vec3((float)(i % 4) / 3.0f, (float)(i % 6) / 5.0f, (float)(i % 9) / 8.0f));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}