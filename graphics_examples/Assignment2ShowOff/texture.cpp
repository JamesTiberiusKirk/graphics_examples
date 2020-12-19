#include "texture.h"

Texture::Texture(const char* filename)
{
	unsigned char* imageData;
	int width, height, nrChannels;

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

CubemapTexture::CubemapTexture(std::vector<std::string> filepaths)
{

	glGenTextures(1, &uid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, uid);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < filepaths.size(); i++)
	{
		unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &nrChannels, 0);
		std::cout << "Loaded " << filepaths[i] << " loaded. " << i + 1 << "/" << filepaths.size() << std::endl;
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << filepaths[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
