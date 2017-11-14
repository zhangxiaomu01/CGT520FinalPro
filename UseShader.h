#ifndef USESHADER_H
#define USESHADER_H

#include "LoadMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void reload_shader(GLuint &shaders, std::string vertexShader, std::string fragmentShader);

void shaderSelctor(GLuint x, GLuint i, MeshData mesh_data,
	GLuint texture_id, glm::mat4 M, glm::mat4 V, glm::mat4 P);
void renderGeometry(GLuint shderPro, MeshData mesh_data, GLuint texture_id, glm::mat4 M, glm::mat4 V, glm::mat4 P);
void renderPhong(GLuint shderPro, MeshData mesh_data, GLuint texture_id, glm::mat4 M, glm::mat4 V, glm::mat4 P);

#endif