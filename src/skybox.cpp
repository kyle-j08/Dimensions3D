
/*
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, 1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f
};


unsigned int cubemapTexture = loadCubemap({
	"resources/images/vz_apocalypse_ocean_right.png",
	"resources/images/vz_apocalypse_ocean_left.png",
	"resources/images/vz_apocalypse_ocean_up.png",
	"resources/images/vz_apocalypse_ocean_down.png",
	"resources/images/vz_apocalypse_ocean_front.png",
	"resources/images/vz_apocalypse_ocean_back.png"
	});

if (cubemapTexture == 0) {
	std::cerr << "Failed to load cubemap texture!" << std::endl;
}
Shader skyboxShader;


GLuint skyboxVAO, skyboxVBO;
glGenVertexArrays(1, &skyboxVAO);
glGenBuffers(1, &skyboxVBO);
glBindVertexArray(skyboxVAO);
glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

skyboxShader.loadShader("resources/shaders/SkyboxVertex.glsl", "resources/shaders/SkyboxFragment.glsl");

skyboxShader.use();
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
glUniform1i(glGetUniformLocation(skyboxShader.getID(), "skybox"), 0);


glCullFace(GL_FRONT);
glDepthFunc(GL_LEQUAL);
skyboxShader.use();


glm::mat4 view = glm::mat4(glm::mat3(mainCamera.getViewMatrix()));
glUniformMatrix4fv(glGetUniformLocation(
	skyboxShader.getID(), "view"), 1, GL_FALSE,
	glm::value_ptr(view));

glUniformMatrix4fv(glGetUniformLocation(
	skyboxShader.getID(), "projection"), 1, GL_FALSE,
	glm::value_ptr(mainCamera.getProjectionMatrix()));


glBindVertexArray(skyboxVAO);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

glDrawArrays(GL_TRIANGLES, 0, 36);

glBindVertexArray(0);
glDepthFunc(GL_LESS);
glCullFace(GL_BACK);


	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

*/