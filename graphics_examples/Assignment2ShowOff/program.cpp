#pragma once

#include <iostream>
#include <vector>
#include <fstream>

#include "program.h"

/* Read a text file into a string*/
inline std::string readFile(const char *filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}


Program::Program(const char* vertPath, const char* fragPath)
{
	try
	{
		this->uid = loadShader(vertPath, fragPath);

	}
	catch (std::exception& e)
	{
		std::cout << "SHADER LOAD EXCEPTION: " << e.what() << std::endl;
		std::cin.ignore();
		exit(1); // 1 to tell the system that there was an error
	}
}


/* Loads from file then passes to buildShader */
GLuint Program::loadShader(const char* vertPath, const char* fragPath)
{
	GLuint vertShader, fragShader;

	// Read shaders
	std::string vertShaderStr = readFile(vertPath);
	std::string fragShaderStr = readFile(fragPath);

	GLint result = GL_FALSE;
	int logLength;

	vertShader = buildShader(GL_VERTEX_SHADER, vertShaderStr);
	fragShader = buildShader(GL_FRAGMENT_SHADER, fragShaderStr);

	std::cout << "Linking program" << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;

}

/* Builds the shader from text string */
GLuint Program::buildShader(GLenum shaderType, const std::string &shaderText)
{
	GLuint shader = glCreateShader(shaderType);
	const char *strFileData = shaderText.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Output the compile errors
		
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (shaderType)
		{
			case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
			case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		std::cerr << "Compile error in " << strShaderType << "\n\t" << strInfoLog << std::endl;
		delete[] strInfoLog;

		throw std::exception("Shader compile exception");
	}

	return shader;
}