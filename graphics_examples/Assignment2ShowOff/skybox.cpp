#include "skybox.h"

#include <iostream>

#include "stb_image.h"

Skybox::Skybox(std::vector<std::string> fp, const char* vertPath, const char* fragPath)
    : facePaths(fp)
{
	skyboxVAO = 0;

    // Initialising the skybox shader.
	skyboxShader = new ShaderProgram(vertPath, fragPath);

    // Skybox cube positions.
    float skyboxVertices[] = {
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

    // To increase the size of the vertex because im too lazy to re-type them.
    for (int i = 0 ; i<( sizeof(skyboxVertices) / sizeof(float)); i++) 
    {
        skyboxVertices[i] *= 50.f;
    }

    // skybox VAO
    glGenBuffers(1, & skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Initialising the texture faces.
    faces = new CubemapTexture(facePaths);

    skyboxShader->passInt("skybox",0);

};


void Skybox::draw(glm::mat4 &view, glm::mat4 &projection)
{
    // Use the program for the cubemap.
    skyboxShader->use();

    // Enabling the depth functin.
    glDepthFunc(GL_LEQUAL);
    
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glEnableVertexAttribArray(skyboxVAO);
    glVertexAttribPointer(skyboxVAO, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Remove the last element of the mat 4 view to create the skybox perception.
    glm::mat4 v = glm::mat4(glm::mat3(view)); 

    // Pass view and projection to the shader.
    skyboxShader->passMat4("view", v);
    skyboxShader->passMat4("projection", projection);

    // Bind the texture and draw the cube.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, faces->uid);
    glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Enabling the depth functin.
    glDepthFunc(GL_LESS);
}
