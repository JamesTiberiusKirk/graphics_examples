#pragma once

#include <string>
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

class Program
{
public:

	GLuint uid;

	Program(const char* vertPath, const char* fragPath);
	~Program();

private:
	
	GLuint loadShader(const char *vertPath, const char *fragPath);
	GLuint buildShader(GLenum ehaderType, const std::string &shaderText);
};

