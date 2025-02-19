#include "needed_libs.h"




std::vector<Vertex> vertices;
std::vector<unsigned int> indices;


void mainUpdate(Window& window, bool& fullscreenKey)
{
	if (window.getKeyState(GLFW_KEY_0) == GLFW_PRESS && fullscreenKey != true)
	{
		window.toggleFullscreen();
		fullscreenKey = true;
	}
	else if (window.getKeyState(GLFW_KEY_0) == GLFW_RELEASE)
	{
		fullscreenKey = false;
	}
}

int main()
{
    
	Window window("My4 Window", 1220, 780, true); //Also creates GLFW & GlEW
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glm::vec3 lightPosition = glm::vec3(2.f, 2.f, 2.f);

	Camera mainCamera(65.f, 0.1f, 300.f, glm::vec3(3.f,0.f,0.f), glm::vec3(0.f),
		static_cast<float>(window.getFramebufferWidth()) / window.getFramebufferHeight());

	

	//------------------------------------------------------------------------------------------------------------------------------


	


	Shader* coreProgram = new Shader();

	coreProgram->loadShader("resources/shaders/VertexShader.glsl", "resources/shaders/FragmentShader.glsl");


	Object testObject(coreProgram, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f),
		"resources/models/box2.obj", "resources/images/container2.png",
		"resources/images/container2_specular.png", true, 5.f);
	testObject.load();

	Object testObject1(coreProgram, glm::vec3(0.f, 0.f, 4.f), glm::vec3(0.f), glm::vec3(1.f),
		"resources/models/box2.obj", "resources/images/container2.png", "", false, 0.f);
	testObject1.load();

	PointLight pointLight(coreProgram, glm::vec3(0.f, 0.f, 2.f),
		glm::vec3(0.f, 1.f, 1.f), 1.f, 50.f);


	DirectionalLight dirLight(coreProgram, 
		glm::vec3(0.f, -45.f, 0.f), glm::vec3(1.f, 1.f, 1.f), 1.f);

	bool fullscreenKeyDown = false;


	//------------------------------------------------------------------------------------------------------------------------------



	coreProgram->use();

	//Light
	glUniform3fv(glGetUniformLocation(coreProgram->getID(), "spotLights[0].position"), 1, glm::value_ptr(glm::vec3(2.f, 0.f, 0.f)));
	glUniform3fv(glGetUniformLocation(coreProgram->getID(), "spotLights[0].direction"), 1, glm::value_ptr(glm::vec3(-2.f, 0.f, 0.f)));
	glUniform3fv(glGetUniformLocation(coreProgram->getID(), "spotLights[0].color"), 1, glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
	glUniform1f(glGetUniformLocation(coreProgram->getID(),  "spotLights[0].brightness"), 2.0f);
	glUniform1f(glGetUniformLocation(coreProgram->getID(),  "spotLights[0].innerAngle"), glm::cos(glm::radians(35.f)));
	glUniform1f(glGetUniformLocation(coreProgram->getID(),  "spotLights[0].outerAngle"), glm::cos(glm::radians(45.f)));
	glUniform1f(glGetUniformLocation(coreProgram->getID(),  "spotLights[0].fallOffDistance"), 50.f);


	pointLight.render();
	dirLight.render();
	//------------------------------------------------------------------------------------------------------------------------------


	while (!window.getWindowShouldClose())
	{
		mainUpdate(window, fullscreenKeyDown);
		window.updateFramebuffer(); // For resizing


		if (window.getKeyState(GLFW_KEY_W) == GLFW_PRESS)
			mainCamera.addPosition(glm::vec3(-0.05f, 0.f, 0.f));
		if (window.getKeyState(GLFW_KEY_S) == GLFW_PRESS)
			mainCamera.addPosition(glm::vec3(0.05f, 0.f, 0.f));
		if (window.getKeyState(GLFW_KEY_A) == GLFW_PRESS)
			mainCamera.addPosition(glm::vec3(0.f, 0.f, 0.05f));
		if (window.getKeyState(GLFW_KEY_D) == GLFW_PRESS)
			mainCamera.addPosition(glm::vec3(0.f, 0.f, -0.05f));

		if (window.getKeyState(GLFW_KEY_UP) == GLFW_PRESS)
			mainCamera.addRotation(glm::vec3(glm::radians(-300.f), 0.f, 0.f));
		if (window.getKeyState(GLFW_KEY_DOWN) == GLFW_PRESS)
			mainCamera.addRotation(glm::vec3(glm::radians(300.f), 0.f, 0.f));
		if (window.getKeyState(GLFW_KEY_LEFT) == GLFW_PRESS)
			mainCamera.addRotation(glm::vec3(0.f, glm::radians(300.f), 0.f));
		if (window.getKeyState(GLFW_KEY_RIGHT) == GLFW_PRESS)
			mainCamera.addRotation(glm::vec3(0.f, glm::radians(-300.f), 0.f));


		mainCamera.updateMatrixes(static_cast<float>(window.getFramebufferWidth()) / window.getFramebufferHeight());



//------------------------------------------------------------------------------------------------------------------------------
		glfwPollEvents();
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		testObject.render(mainCamera.getProjectionMatrix(),
			mainCamera.getViewMatrix(), mainCamera.getPosition());
		testObject1.render(mainCamera.getProjectionMatrix(),
			mainCamera.getViewMatrix(), mainCamera.getPosition());


		GLenum error;
		if (error = glGetError())
		{
			std::cout << "Error: " << error << std::endl;
		}

		//End
		window.swapBuffers();
		glFlush();
	}



	return 0;

}



