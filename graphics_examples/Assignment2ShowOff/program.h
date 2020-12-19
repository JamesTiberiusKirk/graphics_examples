#pragma once

#include <string>
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

#include "common.h"

class ShaderProgram
{
public:

	GLuint uid;

	ShaderProgram(const char* vertPath, const char* fragPath);
	~ShaderProgram();

	void use();
	void passMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(uid, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(uid, name.c_str()), value);
	}
private:
	
	GLuint loadShaderFromFile(const char *vertPath, const char *fragPath);
	GLuint buildShader(GLenum ehaderType, const std::string &shaderText);
};

