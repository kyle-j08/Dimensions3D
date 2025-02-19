#pragma once
#include "../needed_libs.h"
#ifndef SHADER_H
#include "Shader.h"
#endif

class Shader;

class Object
{
private:
	glm::mat4 transform;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;


	GLuint VAO, VBO, EBO;
	GLuint diffuseTexture;
	GLuint specularTexture;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	Material material;

	const char* modelPath;
	const char* diffusePath;
	const char* specularPath;

	bool useSpecular;

	GLint modelMatrixLoc;
	GLint viewMatrixLoc;
	GLint projectionMatrixLoc;
	GLint viewPosLoc;
	GLint materialDiffuseLoc;
	GLint materialSpecularLoc;
	GLint materialShininessLoc;
	GLint useSpecularLoc;

	Shader* program;

	void updateMatrix();
	void initBuffers();
	void initUniforms();
	GLuint loadTexture(const char* filename, GLenum textureFilteringMode, GLenum textureWrappingMode);
	void loadModel();

public:
	Object(Shader* coreProgram, const glm::vec3& startPos,
		const glm::vec3& startRot, const glm::vec3& startScale,
		const char* modelFilePath, const char* diffuseFilePath,
		const char* specularFilePath, bool hasSpecular, float specularIntensity);

	~Object();


	void load();
	void unload();

	void render(glm::mat4 projectionMatrix,
		glm::mat4 viewMatrix, glm::vec3 viewPos);

	inline void setPosition(const glm::vec3& newPos) { this->position = newPos; }
	inline void setRotation(const glm::vec3& newRot) { this->rotation = newRot; }
	inline void setScale(const glm::vec3& newScale) { this->scale = newScale; }

	inline void addPosition(const glm::vec3& toAdd) { this->position += toAdd; }
	inline void addRotation(const glm::vec3& toAdd) { this->rotation += toAdd; }
	inline void addScale(const glm::vec3& toAdd) { this->scale += toAdd; }

	inline glm::vec3 getPosition() const { return this->position; }
	inline glm::vec3 getRotation() const { return this->rotation; }
	inline glm::vec3 getScale() const { return this->scale; }


	
};

