#ifndef HW8_H
#define HW8_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "my_shader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <vector>

using namespace std;

class HW8 {
private:
	GLFWwindow* window;
	unsigned int VBO, controllVAO, assistVAO, bezierVAO;
	vector<glm::vec2> controllVertices;
	float curvePointsArray[100000], controllVerticesArray[100], assistVerticesArray[100];
	int curvePointsNum, controllVerticesNum;
	int factorial[13];
	float frame;

public:
	HW8(GLFWwindow* _window) {
		window = _window;
		controllVerticesNum = 0;
		curvePointsNum = 0;
		frame = 0.0f;

		for (int i = 0; i < 100000; i++) {
			curvePointsArray[i] = 0.0f;
		}
		for (int i = 0; i < 100; i++) {
			controllVerticesArray[i] = 0.0f;
		}
		for (int i = 0; i < 100; i++) {
			assistVerticesArray[i] = 0.0f;
		}

		factorial[0] = 1;
		for (int i = 1; i < 13; i++) {
			factorial[i] = i * factorial[i - 1];
		}
	}

	float getBernstein(int i, int n, float t) {
		return factorial[n] / (factorial[i] * factorial[n - i]) * pow(t, i) * pow(1 - t, n - i);
	}

	void addcontrolllPoint(glm::vec2 pos) {
		if (controllVerticesNum <= 12) {
			controllVertices.push_back(pos);
			controllVerticesArray[controllVerticesNum * 2] = pos.x;
			controllVerticesArray[controllVerticesNum * 2 + 1] = pos.y;
			controllVerticesNum++;
		}		
	}

	void deletecontrolllPoint() {
		if (controllVerticesNum > 0) {
			controllVertices.pop_back();
			controllVerticesNum--;
		}
	}

	void generateBezierCurve() {
		int n = controllVerticesNum - 1;
		for (float t = 0.0; t < 1.0; t += 0.001) {
			glm::vec2 temp(0, 0);
			for (int i = 0; i <= n; i++) {
				temp += controllVertices[i] * getBernstein(i, n, t);
			}
			curvePointsArray[curvePointsNum * 2] = temp.x;
			curvePointsArray[curvePointsNum * 2 + 1] = temp.y;
			curvePointsNum++;
		}
	}

	void renderAssistLines(float t) {
		for (int i = 0; i < controllVerticesNum * 2; i++) {
			assistVerticesArray[i] = controllVerticesArray[i];
		}

		for (int i = controllVerticesNum; i > 1; i--) {
			for (int j = 0; j < i - 1; j++) {
				// 插值
				glm::vec2 p1 = glm::vec2(assistVerticesArray[j * 2], assistVerticesArray[j * 2 + 1]);
				glm::vec2 p2 = glm::vec2(assistVerticesArray[(j + 1) * 2], assistVerticesArray[(j + 1) * 2 + 1]);
				glm::vec2 p = p1 * (1 - t) + p2 * t;

				assistVerticesArray[j * 2] = p.x;
				assistVerticesArray[j * 2 + 1] = p.y;
			}
			drawAssistLines(i - 1);
		}
	}

	void drawAssistLines(int count) {
		glGenVertexArrays(1, &assistVAO);
		glBindVertexArray(assistVAO);
		glDeleteBuffers(1, &VBO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(assistVerticesArray), assistVerticesArray, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(assistVAO);
		glPointSize(10);
		glDrawArrays(GL_POINTS, 0, count);
		glDrawArrays(GL_LINE_STRIP, 0, count);
	}

	void draw() {
		// 创建着色器对象
		Shader shader("hw8_shader.vs", "hw8_shader.fs");

		// 着色器
		shader.use();

		// 渲染循环
		while (!glfwWindowShouldClose(window)) {
			// 输入控制
			processInput(window);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓存

			curvePointsNum = 0;

			// 绘制控制点
			glGenVertexArrays(1, &controllVAO);
			glBindVertexArray(controllVAO);
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(controllVerticesArray), controllVerticesArray, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			
			glBindVertexArray(controllVAO);
			shader.setVec3("pointColor", glm::vec3(1.0f, 1.0f, 1.0f));
			if (controllVerticesNum > 0) { // 点
				glPointSize(10);
				glDrawArrays(GL_POINTS, 0, controllVerticesNum);
			}
			if (controllVerticesNum > 1) { // 连线
				glPointSize(3);
				glDrawArrays(GL_LINE_STRIP, 0, controllVerticesNum);
			}

			
			if (controllVerticesNum > 1) {
				// 计算贝塞尔曲线
				generateBezierCurve();

				// 绘制贝塞尔曲线
				glGenVertexArrays(1, &bezierVAO);
				glBindVertexArray(bezierVAO);
				glDeleteBuffers(1, &VBO);
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(curvePointsArray), curvePointsArray, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glBindVertexArray(bezierVAO);
				shader.setVec3("pointColor", glm::vec3(1.0f, 0.0f, 0.0f));
				glPointSize(3);
				glDrawArrays(GL_POINTS, 0, curvePointsNum);
			}

			// 绘制辅助线
			shader.setVec3("pointColor", glm::vec3(0.0f, 1.0f, 0.5f));
			frame += 0.001;
			frame = frame > 1 ? 0 : frame;
			if (controllVerticesNum == 0) frame = 0;
			renderAssistLines(frame);


			glfwSwapBuffers(window);	// 交换颜色缓冲
			glfwPollEvents();	// 检查有没有触发什么事件并更新窗口状态
		}
	}

	void clear() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &controllVAO);
		glDeleteVertexArrays(1, &bezierVAO);
		glDeleteVertexArrays(1, &assistVAO);
		glDeleteBuffers(1, &VBO);
	}

	void processInput(GLFWwindow *window) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}
};

#endif