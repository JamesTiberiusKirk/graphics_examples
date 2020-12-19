#include "skybox.h"


/* Generic constructor which needs paths for textures files, vertex and frag shaders. */
SkyBox::SkyBox(const std::vector<std::string> textureFilesPath, const char* vertShaderPath, const char* fragShaderPath)
{
    program = new Program(vertShaderPath,fragShaderPath);
    float vertices[] = 
    {
        // positions          
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

    int loc = glGetUniformLocation(program->uid, "skybox");
    if (loc > 0) glUniform1i(loc, 0);

    vao = 0;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    skyTex = new CubemapTexture(textureFilesPath);
}

/* To be used in the update method. */
void SkyBox::draw(glm::mat4 view, glm::mat4 projection) 
{
	glDepthFunc(GL_LEQUAL);

    program->use();

    // This is to remove the translation from the view matrix
    program->passMat4("view",glm::mat4(glm::mat3(view)));
    //program->passMat4("view",glm::lookAt(
    //    glm::vec3(0,0,-1.5f),
    //    glm::vec3(0,0,-1),
    //    glm::vec3(0,1,0)
    //    ));
    program->passMat4("projection",projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(1)); 
    program->passMat4("modlel", model);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vao);
    glVertexAttribPointer(vao, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    skyTex->bindTexture(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);

	glUseProgram(0);
	glDepthFunc(GL_LESS);

}
