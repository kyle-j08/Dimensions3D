#pragma once
#include "../needed_libs.h"

class Shader
{
private:
	GLuint ID;


	std::string loadShaderFromFile(const std::string& filePath);
	GLuint createShaderProgramFromFiles(const std::string& vertexPath,
		const std::string& fragmentPath);


public:

	Shader();
	~Shader();

	void loadShader(const std::string& vertexShader, const std::string& fragmentShader);

	void use();
	void unuse();
	inline GLuint getID() { return this->ID; }
};

