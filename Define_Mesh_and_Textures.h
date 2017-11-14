

#ifndef DEFINE_MESH_TEXTURE_H
#define DEFINE_MESH_TEXTURE_H

#include<string>
#include "LoadMesh.h"
#include "LoadTexture.h"

struct model_tex_Stru
{
	MeshData models;
	GLint textures;
};

model_tex_Stru loadM_T( std::string models, std::string textures);



#endif

