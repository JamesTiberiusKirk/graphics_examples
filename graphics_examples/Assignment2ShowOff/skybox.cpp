#include "skybox.h"


/* Generic constructor which needs paths for textures files, vertex and frag shaders. */
SkyBox::SkyBox(const std::vector<std::string> textureFilesPath, const char* vertShaderPath, const char* fragShaderPath)
{
    program = new Program(vertShaderPath,fragShaderPath);
    float vertices[] = 
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

	skyText.initCubeMap(textureFilesPath);

    int loc = glGetUniformLocation(program->uid, "skybox");
    if (loc > 0) glUniform1i(loc, 0);

    vao = 0;
	cam = new Camera(program, CameraVecs
		{
		glm::vec3(0, 0, -1.5f),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 1, 0)
		}
	);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

/* To be used in the update method. */
void SkyBox::draw() 
{
    cam->draw();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vao);
    glVertexAttribPointer(vao, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glActiveTexture(GL_TEXTURE0);
    skyText.bindTexture();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    skyText.unbindTexture();
}
