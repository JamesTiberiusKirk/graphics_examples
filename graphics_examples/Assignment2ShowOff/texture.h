#pragma once

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "wrapper_glfw.h"

class Texture {
public:
	unsigned int textureId;
	unsigned char* imageData;
	int width, height, nrChannels;

	/*
	This function is for loading the texture.
	*/
	void initTexture(const char* filename)
	{
		// Loading the image
		imageData = stbi_load(filename, &width, &height, &nrChannels, 0);

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
			std::cout << "Texture " << filename << " loaded." << std::endl;

			// Free it at the end
			stbi_image_free(imageData);
		}
		else
		{
			std::cout << "Error reaing texture" << std::endl;
		}

	}

	/*
	This is to bind the texture to the object.
	*/
	void bindTexture(const bool genMipMap = true)
	{
		//if (genMipMap)
		//{
		//	glGenerateMipmap(GL_TEXTURE_2D);
		//}
		//else
		//{
		//	// these are called texture params, controls how the texture is rendered. If not
		//	//  defined the texture will not render

		//	// MIN_FILTER = controls how the texture will look when the pixels are larger than the pixel fragments
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//	// MAG_FILTER = controls how the texture will look when the pixels are smaller than the pixel fragments
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//	// GL_TEXTURE_WRAP_S(_AND_T) controls how the texture s/t (s = local x, t = local y texture coordinates) is smaller than the
		//	//  polygon is repeated or stretched
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//}
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	/*
	This is to unbind the texture to the object.
	*/
	void unbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};