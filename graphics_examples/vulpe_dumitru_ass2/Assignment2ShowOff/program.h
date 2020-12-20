#pragma once

#include <string>
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

#include "common.h"

/* Class for handling shader operations. */
class ShaderProgram
{
public:

	GLuint uid;

	ShaderProgram(const char* vertPath, const char* fragPath);
	ShaderProgram();
	~ShaderProgram();

	/* To switch to this shader. */
	void use();

	/* To pass a mat4 to the shader. */
	void toUniform(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(uid, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	/* To pass a vec4 to the shader. */
	void toUniform(const std::string& name, const glm::vec4& vec) const
	{
		glUniform4fv(glGetUniformLocation(uid, name.c_str()), 1, &vec[0]);
	}

	/* To pass a vec3 to the shader. */
	void toUniform(const std::string& name, const glm::vec3& vec) const
	{
		glUniform3fv(glGetUniformLocation(uid, name.c_str()), 1, &vec[0]);
	}

	/* To pass an int to the shader. */
	void toUniform(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(uid, name.c_str()), value);
	}
private:
	
	/* To load shader from file. */
	GLuint loadShaderFromFile(const char *vertPath, const char *fragPath);

	/* To build the shader string. */
	GLuint buildShader(GLenum ehaderType, const std::string &shaderText);
};

