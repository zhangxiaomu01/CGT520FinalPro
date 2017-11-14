#include "LoadMesh.h"
#include "UseShader.h"
#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "imgui_impl_glut.h"

#include<iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>



void reload_shader(GLuint &shaders, std::string vertexShader, std::string fragmentShader)
{
	GLuint new_shader = InitShader(vertexShader.c_str(), fragmentShader.c_str());

	if (new_shader == -1) // loading failed
	{
		glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	}
	else
	{
		glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

		if (shaders != -1)
		{
			glDeleteProgram(shaders);
		}
		shaders = new_shader;
		//std::cout << shaders << std::endl;
	}
}





void shaderSelctor(GLuint x, GLuint i, MeshData mesh_data, 
					GLuint texture_id, glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	switch (i)
	{
	case 1:
		renderGeometry(x,mesh_data,texture_id,M,V,P);
		break;
	case 2:
		renderPhong(x, mesh_data, texture_id, M, V, P);
		break;
	}
}


void renderGeometry(GLuint shderPro, MeshData mesh_data, 
					GLuint texture_id, glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	glUseProgram(shderPro);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	int PVM_loc = glGetUniformLocation(shderPro, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P*V*M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(shderPro, "texture");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	glBindVertexArray(mesh_data.mVao);
	glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0);
}

void renderPhong(GLuint shderPro, MeshData mesh_data, 
				GLuint texture_id, glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	glUseProgram(shderPro);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	int PVM_loc = glGetUniformLocation(shderPro, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P*V*M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(shderPro, "texture");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	glBindVertexArray(mesh_data.mVao);
	glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0);
}