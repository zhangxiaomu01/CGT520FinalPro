#include<iostream>
#include<string>
#include <windows.h>
#include "Define_Mesh_and_Textures.h"
#include "LoadMesh.h"
#include "LoadTexture.h"


model_tex_Stru loadM_T(std::string models, std::string textures)
{
	model_tex_Stru x;
	x.models = LoadMesh(models);
	x.textures = LoadTexture(textures);
	return x;
}




