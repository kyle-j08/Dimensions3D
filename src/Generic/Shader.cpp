#include "Shader.h"

std::string Shader::loadShaderFromFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error file not found: " << filePath << std::endl;
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
		std::cerr << "Error no vertex or fragment source" << '\n';
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
		std::cerr << "Error failed to compile vertex file: \n" << infoLog << '\n';
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "Error failed to compile fragment file: \n" << infoLog << '\n';
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Error failed to link program: \n" << infoLog << '\n';
	}
	else
	{
		std::cout << "Shader created successfully" << '\n';
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




