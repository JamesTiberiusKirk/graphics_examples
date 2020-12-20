#include "skybox.h"


/* Generic constructor which needs paths for textures files, vertex and frag shaders. */
SkyBox::SkyBox()
{
    vao = 0;
}

void SkyBox::init(ShaderProgram &p, const std::vector<std::string> textureFilesPath, const char* vertShaderPath, const char* fragShaderPath)
{
    program = p;
    std::cout << "Error Code " << glGetError() << " from program " << program.uid << std::endl;

    std::vector<std::string> filepaths =
    {
        "textures\\skybox\\right.jpg",
        "textures\\skybox\\left.jpg",
        "textures\\skybox\\top.jpg",
        "textures\\skybox\\bottom.jpg",
        "textures\\skybox\\front.jpg",
        "textures\\skybox\\back.jpg"
    };

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < filepaths.size(); i++)
	{
		unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &nrChannels, 0);
		std::cout << "Texture " << filepaths[i] << " loaded. " << i + 1 << "/" << filepaths.size() << std::endl;
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

    int loc = glGetUniformLocation(program.uid, "skybox");
    if (loc > 0) glUniform1i(loc, 0);

    GLfloat vertices[] = 
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    //skyTex = new CubemapTexture(textureFilesPath);


    //program->passInt("skybox",0);



    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

/* To be used in the update method. */
void SkyBox::draw(glm::mat4 &view, glm::mat4 &projection) 
{
	//glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    program.use();

    glm::mat4 p= glm::perspective(glm::radians(60.0f), 1920.f/1080.f, 0.1f, 100.0f);
    glm::mat4 v= glm::lookAt(
        glm::vec3(0.0f, 0.0f, -1.5f),
        glm::vec3(0.0f, 0.0f, -1.f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // This is to remove the translation from the view matrix
    //program.passMat4("view",view);
    program.passMat4("view",v);
	//program->passMat4("view", glm::lookAt(
	//	glm::vec3(0, 0, -1.5f),
	//	glm::vec3(0, 0, -1),
	//	glm::vec3(0, 1, 0)
	//));
	program.passMat4("projection", p);

    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(1)); 
    //model = glm::scale(model, glm::vec3(1)); 
    //program->passMat4("modlel", model);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vao);
    glVertexAttribPointer(vao, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //skyTex->bindTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

    glDrawArrays(GL_TRIANGLES, 0, 36);


	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


	glUseProgram(0);
	//glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}
