#pragma once
#include <stdint.h>

class ShaderProgram
{
public:
	ShaderProgram(char* vertexShaderFile, char* fragmentShaderFile);
	~ShaderProgram();
private:
	uint32_t program;
public:
	void Bind();
	int GetLocation(char* variable);
};

