#include "needed_libs.h"

std::vector<Vertex> vertices;
std::vector<unsigned int> indices;
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

GLuint loadCubemap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (GLuint i = 0; i < faces.size(); i++)
	{
		unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, SOIL_LOAD_RGB);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			SOIL_free_image_data(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			SOIL_free_image_data(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

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

void loadModel(const std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
		return;
	}

	if (scene->mNumMeshes == 0 || !scene->mMeshes[0])
	{
		std::cerr << "ERROR::NO_MESHES::MESH::MAIN.CPP\n";
		return;
	}

	aiMesh* mesh = scene->mMeshes[0]; // Load the first mesh
	if (!mesh)
	{
		std::cerr << "ERROR::LOADING::MESH::MAIN.CPP" << '\n';
	}

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D pos = mesh->mVertices[i];
		aiVector3D norm = mesh->mNormals[i];
		aiVector3D tex = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0.0f);

		Vertex vertex;
		vertex.position = glm::vec3(pos.x, pos.y, pos.z);
		vertex.texcoord = glm::vec2(tex.x, tex.y);
		vertex.normal = glm::vec3(norm.x, norm.y, norm.z);

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
}

GLuint loadTexture(const char* filename, GLenum textureFilteringMode, GLenum textureWrappingMode)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, nrChannels;
	GLenum type = GL_RGB;
	unsigned char* image = SOIL_load_image(filename, &width, &height, &nrChannels, 0);

	if (image)
	{
		if (nrChannels == 4) type = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, image);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingMode);

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}
	else
	{
		std::cerr << "ERROR::LOADING_TEXTURE::TEXTURE::MAIN.CPP" << '\n';
		return NULL;
	}

	return NULL;
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

	Material material;
	material.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	material.diffuse = glm::vec3(1.f, 0.5f, 0.31f);
	material.shininess = 5.f;
	material.specular = glm::vec3(0.5f, 0.5f, 0.5f);

	//------------------------------------------------------------------------------------------------------------------------------


	unsigned int cubemapTexture = loadCubemap({ 
		"resources/images/vz_apocalypse_ocean_right.png",
		"resources/images/vz_apocalypse_ocean_left.png",
		"resources/images/vz_apocalypse_ocean_up.png",
		"resources/images/vz_apocalypse_ocean_down.png",
		"resources/images/vz_apocalypse_ocean_front.png",
		"resources/images/vz_apocalypse_ocean_back.png" 
		});


	Shader skyboxShader;


	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	skyboxShader.loadShader("resources/shaders/SkyboxVertex.glsl", "resources/shaders/SkyboxFragment.glsl");

	//------------------------------------------------------------------------------------------------------------------------------


	


	Shader coreProgram;

	coreProgram.loadShader("resources/shaders/VertexShader.glsl", "resources/shaders/FragmentShader.glsl");


	loadModel("resources/models/ball1.obj");
	GLuint diffuseTexture = loadTexture("resources/images/container2.png", GL_LINEAR, GL_REPEAT);
	GLuint specularTexture = loadTexture("resources/images/container2.png", GL_LINEAR, GL_REPEAT);
	glm::mat4 modelMatrix;

	modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f));
	modelMatrix = glm::rotate(modelMatrix, 0.f, glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(0,1.f,0.f));
	modelMatrix = glm::rotate(modelMatrix, 0.f, glm::vec3(0.f,0.f,1.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f));

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);

	// Position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// Texture Coord attribute (2 floats)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(1);

	// Normal attribute (3 floats)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	
	bool fullscreenKeyDown = false;


	//------------------------------------------------------------------------------------------------------------------------------

	skyboxShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glUniform1i(glGetUniformLocation(skyboxShader.getID(), "skybox"), 0);

	//------------------------------------------------------------------------------------------------------------------------------

	coreProgram.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTexture);

	glUniformMatrix4fv(glGetUniformLocation(coreProgram.getID(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(coreProgram.getID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(mainCamera.getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(coreProgram.getID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(mainCamera.getProjectionMatrix()));
	glUniform3fv(glGetUniformLocation(coreProgram.getID(), "viewPos"), 1, glm::value_ptr(mainCamera.getPosition()));

	//Material
	glUniform1i(glGetUniformLocation(coreProgram.getID(), "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(coreProgram.getID(), "material.specular"), 1);
	glUniform1f(glGetUniformLocation(coreProgram.getID(), "material.shininess"), material.shininess);

	//Light
	glUniform3fv(glGetUniformLocation(coreProgram.getID(), "light.ambient"), 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));
	glUniform3fv(glGetUniformLocation(coreProgram.getID(), "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
	glUniform3fv(glGetUniformLocation(coreProgram.getID(), "light.specular"), 1, glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
	glUniform3fv(glGetUniformLocation(coreProgram.getID(), "light.position"), 1, glm::value_ptr(glm::vec3(2.f, 2.f, 0.f)));
	
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





		coreProgram.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexture);

		glUniformMatrix4fv(glGetUniformLocation(coreProgram.getID(), "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(coreProgram.getID(), "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(mainCamera.getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(coreProgram.getID(), "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(mainCamera.getProjectionMatrix()));
		glUniform3fv(glGetUniformLocation(coreProgram.getID(), "viewPos"),1, glm::value_ptr(mainCamera.getPosition()));





		glBindVertexArray(VAO);
		if (indices.empty())
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);

//------------------------------------------------------------------------------------------------------------------------------



		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();

		glm::mat4 view = glm::mat4(glm::mat3(mainCamera.getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(
			skyboxShader.getID(), "view"), 1, GL_FALSE,
			glm::value_ptr(view));

		glUniformMatrix4fv(glGetUniformLocation(
			skyboxShader.getID(), "projection"), 1, GL_FALSE,
			glm::value_ptr(mainCamera.getProjectionMatrix()));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glUniform1i(glGetUniformLocation(skyboxShader.getID(), "skybox"), 0);


		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthFunc(GL_LESS);

		glBindVertexArray(0);


//------------------------------------------------------------------------------------------------------------------------------

		//End
		window.swapBuffers();
		glFlush();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return 0;

}



