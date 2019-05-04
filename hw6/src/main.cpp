#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "hw2.h"
#include "hw3.h"
#include "hw4.h"
#include "hw5.h"
#include "hw6.h"

unsigned int VBO, VAO, EBO, lightVAO;
HW5 *hw5;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	hw5->mouseCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	hw5->scrollCallback(window, xoffset, yoffset);
}

int main() {
	// ��ʼ���������汾�źʹΰ汾�Ŷ���Ϊ3��ʹ�ú���ģʽ
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// ����� Mac OS X ϵͳ����Ҫ�������
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	

	// ��������
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	// ���ô��ڵ���������Ϊ��ǰ�̵߳�������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// ���ڴ�С�ı�Ļص�����

	// ��ʼ�� GLAD������ OpenGL ����ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ImGui ����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	// ���� VBO VAO EBO ����
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(2, &lightVAO);

	hw5 = new HW5(VBO, VAO, EBO, window);
	HW6 hw6(VBO, VAO, lightVAO, window);

	// glfwSetCursorPosCallback(window, mouse_callback);
	// glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// hw5->draw();
	hw6.draw();

	// �ͷ� ImGui ��Դ
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// ɾ���������ɵĶ���
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(2, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// �ͷ����������Դ���˳�Ӧ�ó���
	glfwTerminate();
	return 0;
}