#include "Object.h"

void Object::updateMatrix()
{
	this->transform = glm::mat4(1.f);
	this->transform = glm::translate(this->transform, this->position);
	this->transform = glm::rotate(this->transform, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
	this->transform = glm::rotate(this->transform, glm::radians(this->rotation.y), glm::vec3(0, 1.f, 0.f));
	this->transform = glm::rotate(this->transform, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	this->transform = glm::scale(this->transform, this->scale);
}

void Object::initBuffers()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);

	// Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

	// Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(this->indices.size() * sizeof(unsigned int)), this->indices.data(), GL_STATIC_DRAW);

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
}

void Object::initUniforms()
{
	this->program->use();
	this->modelMatrixLoc = glGetUniformLocation(this->program->getID(), "ModelMatrix");
	this->viewMatrixLoc = glGetUniformLocation(this->program->getID(), "ViewMatrix");
	this->projectionMatrixLoc = glGetUniformLocation(this->program->getID(), "ProjectionMatrix");
	this->viewPosLoc = glGetUniformLocation(this->program->getID(), "viewPos");
	this->materialDiffuseLoc = glGetUniformLocation(this->program->getID(), "material.diffuse");
	this->materialSpecularLoc = glGetUniformLocation(this->program->getID(), "material.specular");
	if(this->useSpecular)
		this->materialShininessLoc = glGetUniformLocation(this->program->getID(), "material.shininess");
	this->useSpecularLoc = glGetUniformLocation(this->program->getID(), "material.useSpecular");
	this->program->unuse();

}

GLuint Object::loadTexture(const char* filename, GLenum textureFilteringMode, GLenum textureWrappingMode)
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
		std::cerr << "Error loading texture: " << filename << '\n';
		return 0;
	}
}

void Object::loadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(this->modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Assimp error: " << importer.GetErrorString() << std::endl;
		return;
	}

	if (scene->mNumMeshes == 0 || !scene->mMeshes[0])
	{
		std::cerr << "Error no meshes in model:" << modelPath << '\n';
		return;
	}

	aiMesh* mesh = scene->mMeshes[0]; // Load the first mesh
	if (!mesh)
	{
		std::cerr << "Error loading mesh in model:" << modelPath << '\n';
		return;
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

	std::cout << "Model loaded successfully: " << modelPath << '\n';
}


Object::Object(Shader* coreProgram, const glm::vec3& startPos,
	const glm::vec3& startRot, const glm::vec3& startScale,
	const char* modelFilePath, const char* diffuseFilePath,
	const char* specularFilePath, bool hasSpecular, float specularIntensity)
	: program(coreProgram), position(startPos), rotation(startRot),
	scale(startScale), modelPath(modelFilePath),
	diffusePath(diffuseFilePath), specularPath(specularFilePath),
	viewPosLoc(0), materialDiffuseLoc(0), materialSpecularLoc(0),
	materialShininessLoc(0), modelMatrixLoc(0), viewMatrixLoc(0),
	projectionMatrixLoc(0), diffuseTexture(0), specularTexture(0),
	VAO(0), VBO(0), EBO(0), transform(glm::mat4(1.f)),
	material({ specularIntensity }), useSpecular(hasSpecular), useSpecularLoc(0)
{


}

Object::~Object()
{
	this->vertices.clear();
	this->indices.clear();
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

void Object::load()
{

	this->loadModel();
	this->diffuseTexture = loadTexture(this->diffusePath, GL_NEAREST, GL_REPEAT);

	if (this->useSpecular)
		this->specularTexture = loadTexture(this->specularPath, GL_NEAREST, GL_REPEAT);

	this->updateMatrix();
	this->initBuffers();
	this->initUniforms();
}

void Object::unload()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);

	if (this->diffuseTexture) glDeleteTextures(1, &this->diffuseTexture);
	if (this->useSpecular)
		if (this->specularTexture) glDeleteTextures(1, &this->specularTexture);
}


void Object::render(glm::mat4 projectionMatrix,
	glm::mat4 viewMatrix, glm::vec3 viewPos)
{
	this->updateMatrix();


	this->program->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->diffuseTexture);

	if (this->useSpecular)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->specularTexture);
		glUniform1i(materialSpecularLoc, 1);
	}


	glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(this->transform));
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
	glUniform1i(materialDiffuseLoc, 0);
	glUniform1f(materialShininessLoc, material.shininess);

	if(this->useSpecular)
		glUniform1i(useSpecularLoc, 1);
	else
		glUniform1i(useSpecularLoc, 0);




	glBindVertexArray(this->VAO);
	if (this->indices.empty())
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(this->vertices.size()));
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices.size()), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
