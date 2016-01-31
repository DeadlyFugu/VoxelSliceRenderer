#include "ShaderProgram.h"
#include "Dependencies\glew.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

char* ReadFile(char* filePath) {
	FILE* file;
	fopen_s(&file, filePath, "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		char* str = (char*) calloc(size+1, sizeof(char));
		rewind(file);
		fread(str, 1, size, file);
		fclose(file);
		return str;
	} else {
		std::cout << "Unable to open file " << filePath << std::endl;
		return NULL;
	}
}

void CheckShaderErrors(uint32_t shader) {
	int success;
	char log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, log);
		std::cout << "Shader program linking error:" << std::endl << log << std::endl;
	}
}

bool CheckProgramErrors(uint32_t program) {
	int success;
	char log[512];
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, log);
		std::cout << "Shader compilation error:" << std::endl << log << std::endl;
	}
	return !success;
}

ShaderProgram::ShaderProgram(char* vertexShaderFile, char* fragmentShaderFile) {
	char* vertexShaderCode = ReadFile(vertexShaderFile);
	char* fragmentShaderCode = ReadFile(fragmentShaderFile);
	std::cout << vertexShaderCode << std::endl;
	std::cout << fragmentShaderCode << std::endl;
	if ((vertexShaderCode == NULL) || (fragmentShaderCode == NULL)) {
		return;
	}
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	CheckShaderErrors(vertexShader);
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	CheckShaderErrors(fragmentShader);

	uint32_t program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	bool error = CheckProgramErrors(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (!error)
		this->program = program;
}


ShaderProgram::~ShaderProgram() {
	glDeleteProgram(this->program);
}


void ShaderProgram::Bind() {
	glUseProgram(this->program);
}

int ShaderProgram::GetLocation(char* variable) {
	return glGetUniformLocation(this->program, variable);
}