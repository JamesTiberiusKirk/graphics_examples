#pragma once

#include <iostream>
#include <vector>

#include "stb_image.h"
#include "common.h"

class Texture
{
public:
	GLuint uid;
	unsigned char* imageData;
	int width, height, nrChannels;

	/*
	This function is for loading the texture.
	*/
	void initTexture(const char* filename)
	{
		// Loading the image
		imageData = stbi_load(filename, &width, &height, &nrChannels, 0);

		if (imageData) // To check if the image was loaded properly
		{
			glGenTextures(1, &uid);
			glBindTexture(GL_TEXTURE_2D, uid);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			stbi_image_free(imageData);
			glGenerateMipmap(GL_TEXTURE_2D);

			std::cout << "Texture " << filename << " loaded." << std::endl;
		}
		else
		{
			std::cout << "Error reaing texture" << std::endl;
		}

	}

	/*
	This is to bind the texture to the object.
	*/
	void bindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, uid);
	}

	/*
	This is to unbind the texture to the object.
	*/
	void unbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};


class CubemapTexture
{
public:
	GLuint uid;

	CubemapTexture(std::vector<std::string> filePaths) 
	{
		glGenTextures(1, &uid);
		glBindTexture(GL_TEXTURE_CUBE_MAP, uid);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < filePaths.size(); i++)
		{
			unsigned char* data = stbi_load(filePaths[i].c_str(), &width, &height, &nrChannels, 0);
			std::cout << "Loaded " << filePaths[i] << " loaded. " << i + 1 << "/" << filePaths.size() << std::endl;
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);

				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path: " << filePaths[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	void bindTexture(GLenum TextureUnit) 
	{
		glActiveTexture(TextureUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, uid);
	}
};
