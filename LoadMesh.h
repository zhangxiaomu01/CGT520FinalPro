#ifndef __LOADMESH_H__
#define __LOADMESH_H__


#include <string>
#include <GL/glew.h>
#include "assimp/Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct MeshData
{
   unsigned int mVao;
   unsigned int mVboVerts;
   unsigned int mVboNormals;
   unsigned int mVboTexCoords;
   unsigned int mIndexBuffer;
   unsigned int mTangentSpace;
   unsigned int mBitangetSpace;

   float mScaleFactor; //TODO replace with bounding box
   unsigned int mNumIndices;
   const aiScene* mScene;
   aiVector3D mBbMin, mBbMax;
   MeshData::MeshData(): mVao(-1), mVboVerts(-1), mVboNormals(-1), mVboTexCoords(-1), mIndexBuffer(-1), mScaleFactor(0.0f), mNumIndices(0), mScene(NULL) {}
      
};


void BufferIndexedVerts(MeshData& meshdata);
void BufferIndexedVertsParticles(MeshData& meshdata, glm::vec3 pos[]);
MeshData LoadMesh( const std::string& pFile);
MeshData LoadParticles(const std::string& pFile, glm::vec3 pos[]);

void GetBoundingBox (const aiScene* scene, aiVector3D* min, aiVector3D* max);
void GetBoundingBox (const aiMesh* mesh, aiVector3D* min, aiVector3D* max);

#endif