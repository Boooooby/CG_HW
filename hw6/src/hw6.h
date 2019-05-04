#ifndef HW6_H
#define HW6_H

#include "my_shader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class HW6 {
private:
	GLFWwindow* window;
	unsigned int VBO, cubeVAO, lightVAO;
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	int shaderType = 0;
	bool isAutoRotation = true;

	float ambientStrength = 0.5;
	float diffuseStrength = 1.0;
	float specularStrength = 0.5;
	int shininess = 32;

	float angle = 0.0;


public:
	HW6(unsigned int _VBO, unsigned int _cubeVAO, unsigned int _lightVAO, GLFWwindow* _window) {
		VBO = _VBO;
		cubeVAO = _cubeVAO;
		lightVAO = _lightVAO;
		window = _window;
	}

	void draw() {
		glEnable(GL_DEPTH_TEST);

		// 创建着色器对象
		Shader gouraudShader("hw6_lighting_shader_gouraud.vs", "hw6_lighting_shader_gouraud.fs");
		Shader phongShader("hw6_lighting_shader_phong.vs", "hw6_lighting_shader_phong.fs");
		Shader lampShader("hw6_lamp_shader.vs", "hw6_lamp_shader.fs");

		glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
		glm::vec3 viewPos(0.0f, 0.0f, -5.0f);

		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		glBindVertexArray(cubeVAO);	// 绑定VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);	// 绑定到 GL_ARRAY_BUFFER 目标上
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// 将顶点数据复制到缓冲的内存中

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// 指定如何解释顶点数据
		glEnableVertexAttribArray(0);	// 启用顶点属性
		// 法向量属性
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(lightVAO);	// 绑定VAO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);	// 绑定到 GL_ARRAY_BUFFER 目标上

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// 指定如何解释顶点数据
		glEnableVertexAttribArray(0);	// 启用顶点属性

		// 渲染循环
		while (!glfwWindowShouldClose(window)) {
			// 输入控制
			processInput(window);

			// 创建 ImGui 窗口
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Translation Setting");
			ImGui::RadioButton("Phong Shading", (int*)&shaderType, 0);
			ImGui::RadioButton("Gouraud Shading", (int*)&shaderType, 1);
			ImGui::Checkbox("Auto Rotation", (bool*)&isAutoRotation);
			ImGui::SliderFloat("ambientStrength", (float*)&ambientStrength, 0.0f, 1.0f);
			ImGui::SliderFloat("diffuseStrength", (float*)&diffuseStrength, 0.0f, 1.0f);
			ImGui::SliderFloat("specularStrength", (float*)&specularStrength, 0.0f, 1.0f);
			ImGui::SliderInt("shininess", (int*)&shininess, 2, 256);
			ImGui::End();

			// 渲染
			int display_w, display_h;
			glfwMakeContextCurrent(window);
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (isAutoRotation) {
				lightPos.x = 1.0f * sin(glfwGetTime() / 2.0f);
				lightPos.y = 1.0f * cos(glfwGetTime() / 2.0f);
			}		
			
			glm::mat4 projection = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);			
			glm::mat4 model = glm::mat4(1.0f);

			projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			view = glm::translate(view, viewPos);

			if (shaderType == 0) {
				phongShader.use();
				phongShader.setFloat("ambientStrength", ambientStrength);
				phongShader.setFloat("diffuseStrength", diffuseStrength);
				phongShader.setFloat("specularStrength", specularStrength);
				phongShader.setInt("shininess", shininess);
				phongShader.setVec3("lightPos", lightPos);
				phongShader.setVec3("viewPos", viewPos);
				phongShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
				phongShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
				phongShader.setMat4("projection", projection);
				phongShader.setMat4("view", view);
				model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
				phongShader.setMat4("model", model);
			}
			
			if (shaderType == 1) {
				gouraudShader.use();
				phongShader.setFloat("ambientStrength", ambientStrength);
				phongShader.setFloat("diffuseStrength", diffuseStrength);
				phongShader.setFloat("specularStrength", specularStrength);
				phongShader.setInt("shininess", shininess);
				gouraudShader.setVec3("lightPos", lightPos);
				gouraudShader.setVec3("viewPos", viewPos);
				gouraudShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
				gouraudShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
				gouraudShader.setMat4("projection", projection);
				gouraudShader.setMat4("view", view);
				model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
				gouraudShader.setMat4("model", model);
			}

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			
			lampShader.use();
			lampShader.setMat4("projection", projection);
			lampShader.setMat4("view", view);
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.setMat4("model", model);

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);	// 交换颜色缓冲
			glfwPollEvents();	// 检查有没有触发什么事件并更新窗口状态
		}
	}

	void processInput(GLFWwindow *window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}
};

#endif