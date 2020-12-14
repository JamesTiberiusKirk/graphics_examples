#pragma once

#include <iostream>
#include <vector>

#include "stb_image.h"
#include "common.h"

class Texture
{
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

		if (imageData) // To check if the image was loaded properly
		{
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
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

	void initCubeMap(std::vector<std::string> file_paths) {
		int width, height, nr_channels;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

		for (int i = 0; i < file_paths.size(); i++) {
			unsigned char* data = stbi_load(file_paths[i].c_str(), &width, &height, &nr_channels, 0);

			if (data) {
				// create texture with attributes
				// 2nd param defines the level
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

				// free the data after the texture is created
				stbi_image_free(data);
			}
			else {
				// free the data after the texture is created
				stbi_image_free(data);
				throw std::exception("File not found");
			}
		}
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		/*glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/
	}

	/*
	This is to bind the texture to the object.
	*/
	void bindTexture()
	{
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