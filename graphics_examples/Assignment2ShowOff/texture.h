#pragma once

#include <iostream>
#include <vector>

#include "stb_image.h"
#include "common.h"

class Texture
{
public:
	/* Uniform ID for the texture. */
	GLuint uid;

	/*
	 * This function is for loading the texture.
	 */
	Texture(const char* filename);

	/*
	 * This is to bind the texture to the object.
	 */
	void bindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, uid);
	}

	/*
	 * This is to unbind the texture to the object.
	 */
	void unbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};


class CubemapTexture
{
public:
	/* Uniform ID for the texture. */
	GLuint uid;

	/* 
	 * Default constructor which loads the textures from file. 
	 */
	CubemapTexture(std::vector<std::string> filePaths);
		
	/* 
	 * Binds the texture in opengl.
	 */
	void bindTexture(GLenum TextureUnit)
	{
		glActiveTexture(TextureUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, uid);
	}
};
