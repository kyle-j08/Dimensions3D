#pragma once
#include "needed_libs.h"


class Window
{
private:
	int windowWidth, windowHeight;
	int frameBufferWidth, frameBufferHeight;
	bool fullscreen;
	bool resizeable;
	const char* title;
	GLFWwindow* window;


	int initWindow();
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);

public:
	Window(const char* window_title, int window_width, int window_height, bool can_resize = false);
	~Window();

	inline GLFWwindow* getWindow() { return this->window; }
	inline int getFramebufferWidth() const { return this->frameBufferWidth; }
	inline int getFramebufferHeight() const { return this->frameBufferHeight; }
	inline void updateFramebuffer() { glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight); }
	inline void setWindowWidth(int newWidth) { this->windowWidth = newWidth; }
	inline void setWindowHeight(int newHeight) { this->windowHeight = newHeight; }
	inline int getWindowShouldClose() { return glfwWindowShouldClose(this->window); }
	inline void swapBuffers() { glfwSwapBuffers(this->window); }
	inline int getKeyState(int key) { return glfwGetKey(this->window, key); }
	void toggleFullscreen();
};

