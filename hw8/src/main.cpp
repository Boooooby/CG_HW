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
		// 转化到[-1,1]区间
		float x = lastX / SCR_WIDTH * 2 - 1;
		float y = -(lastY / SCR_HEIGHT * 2 - 1);
		// 增加顶点
		hw8->addcontrolllPoint(glm::vec2(x, y));
	}
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
		// 删除最新添加的顶点
		hw8->deletecontrolllPoint();
	}
}

int main() {
	// 初始化，将主版本号和次版本号都设为3，使用核心模式
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 如果是 Mac OS X 系统，还要加上这个
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	

	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	// 将该窗口的上下文设为当前线程的上下文
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// 窗口大小改变的回调函数
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 初始化 GLAD，加载 OpenGL 函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ImGui 配置
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	// 生成 VBO VAO EBO 对象
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	hw8 = new HW8(window);
	hw8->draw();

	// 释放 ImGui 资源
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// 删除所有生成的对象
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	hw8->clear();

	// 释放清除所有资源并退出应用程序
	glfwTerminate();
	return 0;
}