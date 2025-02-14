#include "Window.h"

int Window::initWindow()
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, this->resizeable);

	//For Mac OS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, this->title, NULL, NULL);
	if (!this->window)
	{
		std::cerr << "ERROR::INIT::WINDOW::WINDOW.CPP" << '\n';
		glfwTerminate();
		return -1;
	}

	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);\
	glfwSetFramebufferSizeCallback(window, Window::framebuffer_resize_callback);
	glViewport(0, 0, this->frameBufferWidth, this->frameBufferHeight);
	glfwMakeContextCurrent(this->window);

	return 0;
}

void Window::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

Window::Window(const char* window_title, int window_width, int window_height, bool can_resize)
	: windowHeight(window_height), windowWidth(window_width), resizeable(can_resize), fullscreen(false), window(nullptr), title(window_title),
	frameBufferHeight(window_height), frameBufferWidth(window_width)
{
	if (!glfwInit())
	{
		std::cerr << "ERROR::INIT::GLFW::WINDOW.CPP" << '\n';
		exit(EXIT_FAILURE);
	}

	if (this->initWindow() != -1)
	{
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "ERROR::INIT::GLEW::WINDOW.CPP" << '\n';
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		std::cerr << "ERROR::INIT::WINDOW::WINDOW.CPP" << '\n';
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
}

Window::~Window()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}


void Window::toggleFullscreen()
{
	this->fullscreen = !this->fullscreen;

	GLFWmonitor* monitor = this->fullscreen ? glfwGetPrimaryMonitor() : nullptr;
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if (this->fullscreen)
	{
		glfwSetWindowMonitor(this->window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else
	{
		int xpos = 100, ypos = 100;
		glfwSetWindowMonitor(this->window, nullptr, xpos, ypos, this->windowWidth, this->windowHeight, 0);
	}
	std::cout << "DEBUG::TOGGLED_FULLSCREEN::" << (this->fullscreen ? "ON" : "OFF") << '\n';
}
