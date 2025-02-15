#include "Shader.h"

std::string Shader::loadShaderFromFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "ERROR::SHADER::FILE_NOT_FOUND: " << filePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

GLuint Shader::createShaderProgramFromFiles(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexSource = loadShaderFromFile(vertexPath);
	std::string fragmentSource = loadShaderFromFile(fragmentPath);

	if (vertexSource.empty() || fragmentSource.empty()) {
		std::cerr << "ERROR::SHADER::FAILED_TO_LOAD" << std::endl;
		return 0;
	}

	const char* vShaderCode = vertexSource.c_str();
	const char* fShaderCode = fragmentSource.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
	glCompileShader(vertexShader);

	GLint success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	

	return shaderProgram;
}

Shader::Shader()
	: ID()
{
}

Shader::~Shader()
{
	glDeleteProgram(this->ID);
}

void Shader::loadShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	this->ID = createShaderProgramFromFiles(vertexShader, fragmentShader);
}

void Shader::use()
{
	glUseProgram(this->ID);
}

void Shader::unuse()
{
	glUseProgram(0);
}




