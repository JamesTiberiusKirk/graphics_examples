#include "skybox.h"

//inline glm::mat4 GenLookAt()
//{
//	return glm::lookAt(
//		glm::vec3(0, 0, -1.5f),
//		glm::vec3(0, 0, -1),
//		glm::vec3(0, 1, 0)
//	);
//}
//
//inline void CamUniformSetup(GLuint& program, GLuint& viewId, GLuint& projectionId)
//{
//	viewId = glGetUniformLocation(program, "view");
//	projectionId = glGetUniformLocation(program, "projection");
//}
//
//
//inline void DrawCam(GLuint& program, GLuint& viewId, GLuint& projectionId)
//{
//    glm::mat4 lookAt = GenLookAt();
//	glm::mat4 projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
//	glUniformMatrix4fv(projectionId, 1, GL_FALSE, &projection[0][0]);
//	glUniformMatrix4fv(viewId, 1, GL_FALSE, &lookAt[0][0]);
//}

SkyBox::SkyBox(const char* vertShaderPath, const char* fragShaderPath) 
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

	std::vector<std::string> textureFiles = 
    {
		"textures\\cubemap\\right.jpg",
		"textures\\cubemap\\left.jpg",
		"textures\\cubemap\\top.jpg",
		"textures\\cubemap\\bottom.jpg",
		"textures\\cubemap\\front.jpg",
		"textures\\cubemap\\back.jpg"
	};
	
	skyText.initCubeMap(textureFiles);

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
