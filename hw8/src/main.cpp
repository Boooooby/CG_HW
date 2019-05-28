#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "hw8.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX, lastY;

unsigned int VBO, VAO;
HW8 *hw8;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	lastX = xpos;
	lastY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
		// ת����[-1,1]����
		float x = lastX / SCR_WIDTH * 2 - 1;
		float y = -(lastY / SCR_HEIGHT * 2 - 1);
		// ���Ӷ���
		hw8->addcontrolllPoint(glm::vec2(x, y));
	}
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
		// ɾ��������ӵĶ���
		hw8->deletecontrolllPoint();
	}
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	// ���ô��ڵ���������Ϊ��ǰ�̵߳�������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// ���ڴ�С�ı�Ļص�����
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	hw8 = new HW8(window);
	hw8->draw();

	// �ͷ� ImGui ��Դ
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// ɾ���������ɵĶ���
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	hw8->clear();

	// �ͷ����������Դ���˳�Ӧ�ó���
	glfwTerminate();
	return 0;
}