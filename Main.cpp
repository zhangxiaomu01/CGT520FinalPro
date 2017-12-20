#include <windows.h>
#include <iostream>
#include <stack>
#include<math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <irrKlang/irrKlang.h>


#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "imgui_impl_glut.h"
#include "Cube.h"

//symbolic const value.
//SHADOWRES - shadow map resolution
//SHADOW_SAMPLE - lightview binding box, orthometric view 
//MAXFISHPARTICLE - Max fish per particle container
#define SHADOWRES 8192
#define SHADOW_SAMPLE 50
#define WINDOW_WIDTH 1920	
#define WINDOW_HEIGHT 1080
#define MAXFISHPARTICLE 10
#define PI 3.14159265

// Defines max texture, normal map  and mesh data ID, shadowMesh is a special mesh used for shadow calculation
GLuint texture_id[100] = { -1 };
GLuint normalTexture_id[100] = { -1 };
MeshData mesh_data[30];
MeshData shadowMesh;

GLuint shadowMap = -1;
GLuint shadowFramebuffer;


//Texture files and Mesh IDs
static const std::string cube_name = "cubemap";
GLuint cubemap_id = -1; //Texture id for cubemap

static const std::string shadowmesh_name = ".\\Models\\00ShadowMapping.obj";

static const std::string groundTexture = ".\\Textures\\01SandDetail_BC.jpg";
static const std::string groundNormalTexture = ".\\Textures\\01SandDetail_N.jpg";
static const std::string groundmesh_name = ".\\Models\\01GroundMesh.obj";

static const std::string DragonBoneTexture = ".\\Textures\\02DragonBoneD01.jpg";
static const std::string DragonBoneNormalTexture = ".\\Textures\\01SandDetail_N.jpg";
static const std::string DragonBonemesh_name = ".\\Models\\02DragonBoneMesh.obj";

static const std::string SubMarineTexture = ".\\Textures\\03SubMarineD.jpg";
static const std::string SubMarineNormalTexture = ".\\Textures\\03SubMarineN.jpg";
static const std::string SubMarinemesh_name = ".\\Models\\03SubMarineMesh.obj";

static const std::string PlaneTexture = ".\\Textures\\04PlaneD.jpg";
static const std::string PlaneNormalTexture = ".\\Textures\\04PlaneN.jpg";
static const std::string Planemesh_name = ".\\Models\\04PlaneMesh.obj";

static const std::string RedAlgeaTexture = ".\\Textures\\05RedAlgeaD.tga";
static const std::string RedAlgeaNormalTexture = ".\\Textures\\05RedAlgeaN.jpg";
static const std::string RedAlgeamesh_name = ".\\Models\\05RedAlgeaMesh.obj";

static const std::string SpongeTexture = ".\\Textures\\06SpongeD.jpg";
static const std::string SpongeNormalTexture = ".\\Textures\\06SpongeN.jpg";
static const std::string Spongemesh_name = ".\\Models\\06SpongeMesh.obj";

static const std::string AlgeaTexture = ".\\Textures\\07AlgeaD.jpg";
static const std::string AlgeaNormalTexture = ".\\Textures\\07AlgeaN.jpg";
static const std::string Algeamesh_name = ".\\Models\\07AlgeaMesh.obj";

static const std::string CoralineTexture = ".\\Textures\\08CoralineD.jpg";
static const std::string CoralineNormalTexture = ".\\Textures\\08CoralineN.jpg";
static const std::string Coralinemesh_name = ".\\Models\\08CoralineMesh.obj";

static const std::string SeeweedTexture = ".\\Textures\\09SeeweedD.jpg";
static const std::string SeeweedNormalTexture = ".\\Textures\\09SeeweedN.jpg";
static const std::string Seeweedmesh_name = ".\\Models\\09SeeweedMesh.obj";

static const std::string Rock01Texture = ".\\Textures\\10Rock01D.jpg";
static const std::string Rock01NormalTexture = ".\\Textures\\10Rock01N.jpg";
static const std::string Rock01mesh_name = ".\\Models\\10Rock01Mesh.obj";

static const std::string Rock02Texture = ".\\Textures\\11Rock02D.jpg";
static const std::string Rock02NormalTexture = ".\\Textures\\11Rock02N.jpg";
static const std::string Rock02mesh_name = ".\\Models\\11Rock02Mesh.obj";

static const std::string Rock03Texture = ".\\Textures\\12Rock03D.jpg";
static const std::string Rock03NormalTexture = ".\\Textures\\12Rock03N.jpg";
static const std::string Rock03mesh_name = ".\\Models\\12Rock03Mesh.obj";

static const std::string Rock04Texture = ".\\Textures\\13Rock04D.jpg";
static const std::string Rock04NormalTexture = ".\\Textures\\13Rock04N.jpg";
static const std::string Rock04mesh_name = ".\\Models\\13Rock04Mesh.obj";

static const std::string Rock05Texture = ".\\Textures\\13Rock05D.jpg";
static const std::string Rock05NormalTexture = ".\\Textures\\13Rock05N";
static const std::string Rock05mesh_name = ".\\Models\\13Rock05Mesh.obj";

static const std::string fishTexture01 = ".\\Textures\\15T_FishA_BC.TGA";
static const std::string fishTexture02 = ".\\Textures\\15T_FishB_BC.TGA";
static const std::string fishTexture03 = ".\\Textures\\15T_FishC_BC.TGA";
static const std::string fishTextureN01 = ".\\Textures\\15T_FishA_N.jpg";
static const std::string fishTextureN02 = ".\\Textures\\15T_FishB_N.jpg";
static const std::string fishTextureN03 = ".\\Textures\\15T_FishC_N.jpg";
static const std::string fishmesh = ".\\Models\\15Particle_fish.obj";

static const std::string bubbleTexture = ".\\Textures\\16T_Bubbles_BC.jpg";
static const std::string bubbleTextureN = ".\\Textures\\16T_Bubbles_N.jpg";
static const std::string bubbleMesh = ".\\Models\\15Particle_fish.obj";

static const std::string causticTexture = ".\\Textures\\17T_Caustics_BC.jpg";

static const std::string lightRayTexture = ".\\Textures\\18T_LightRay.jpg";
static const std::string lightRayMesh = ".\\Models\\17Ray.obj";

static const std::string BackgroundTexture = ".\\Textures\\20T_BackgroundMask.jpg";

//Soundlist
static const std::string backGroundMusic(".\\Sound\\SongOftheSea.mp3");

//Shader IDS
static const std::string ground_VS(".\\Shaders\\01PhongNormal_vs.glsl");
static const std::string ground_FS(".\\Shaders\\01PhongNormal_fs.glsl");
GLuint PhongNormal_SP =  -1 ;

static const std::string shadowMap_VS(".\\Shaders\\02Shadow_vs.glsl");
static const std::string shadowMap_FS(".\\Shaders\\02Shadow_fs.glsl");
GLuint depthShadowProgramID = -1;

static const std::string cube_vs(".\\Shaders\\03cube_vs.glsl");
static const std::string cube_fs(".\\Shaders\\03cube_fs.glsl");
GLuint cube_shader_program = -1;
GLuint cube_vao = -1;

static const std::string particleFish_VS(".\\Shaders\\04PhongParticlefish_vs.glsl");
static const std::string particleFish_FS(".\\Shaders\\04PhongParticlefish_fs.glsl");
GLuint particleFish_SP = -1;

static const std::string particleBubble_VS(".\\Shaders\\05bubble_vs.glsl");
static const std::string particleBubble_FS(".\\Shaders\\05bubble_fs.glsl");
GLuint particleBubble_SP = -1;

static const std::string groundRock_VS(".\\Shaders\\07PhongGround_vs.glsl");
static const std::string groundRock_FS(".\\Shaders\\07PhongGround_fs.glsl");
GLuint groundRock_SP = -1;

static const std::string seaWeed_VS(".\\Shaders\\08SeaWeed_vs.glsl");
static const std::string seaWeed_FS(".\\Shaders\\08SeaWeed_fs.glsl");
GLuint seaWeed_SP = -1;

static const std::string redAlgea_VS(".\\Shaders\\09RedAlgea_vs.glsl");
static const std::string redAlgea_FS(".\\Shaders\\09RedAlgea_fs.glsl");
GLuint redAlgea_SP = -1;

static const std::string lightRay_VS(".\\Shaders\\10LightRay_vs.glsl");
static const std::string lightRay_FS(".\\Shaders\\10LightRay_fs.glsl");
GLuint lightRay_SP = -1;

static const std::string cameraMask_VS(".\\Shaders\\11cameraMask_vs.glsl");
static const std::string cameraMask_FS(".\\Shaders\\11cameraMask_fs.glsl");
GLuint cameraMask_SP = -1;


//define lights
glm::vec3 directLight(0.3f, 1.0f, 1.0f);
glm::vec3 lightColor(1.08f, 1.08f, 1.04f);

// predefined particle initial offset
glm::vec3 offsetVec[MAXFISHPARTICLE] = { glm::vec3(0.0f),
glm::vec3(1.0 + rand() % 10 / 10.0f,1.0f + rand() % 16 / 2.0f,1.0f - rand() % 4 / 5.0f),
glm::vec3(2.0 + rand() % 10 / 10.0f ,rand() % 4 / 2.0f,rand() % 13 / 5.0f),
glm::vec3(4.0 + rand() % 10 / 10.0f,rand() % 12 / 2.0f,0.0f - rand() % 4 / 5.0f),
glm::vec3(3.0 + rand() % 10 / 10.0f, rand() % 10 / 2.0f,1.0f - rand() % 2 / 5.0f),
glm::vec3(3.0 + rand() % 10 / 10.0f, rand() % 5 / 2.0f,1.0f - rand() % 4 / 5.0f),
glm::vec3(2.0 + rand() % 10 / 10.0f ,rand() % 4 / 2.0f,rand() % 4 / 5.0f),
glm::vec3(6.0 + rand() % 10 / 10.0f ,rand() % 2 / 2.0f,rand() % 10 / 5.0f),
glm::vec3(-4.0 + rand() % 10 / 10.0f ,rand() % 4 / 2.0f,rand() % 4 / 5.0f),
glm::vec3(-2.0 + rand() % 10 / 10.0f ,rand() % 4 / 2.0f,rand() % -14 / 5.0f) };
glm::vec3 offsetVec01[MAXFISHPARTICLE] = { glm::vec3(0.0f),
glm::vec3(10.0 + rand() % 10 / 10.0f, 1.0f + rand() % 16 / 2.0f,1.0f - rand() % 4 / 5.0f),
glm::vec3(2.0 + rand() % 10 / 10.0f , rand() % 4 / 2.0f,        rand() % 13 / 5.0f),
glm::vec3(4.0 + rand() % 10 / 10.0f,  rand() % 12 / 2.0f,       0.0f - rand() % 4 / 5.0f),
glm::vec3(3.0 + rand() % 10 / 10.0f,  rand() % 10 / 2.0f,       1.0f - rand() % -2 / 5.0f),
glm::vec3(-1.0 + rand() % 10 / 10.0f, rand() % 5 / 2.0f,        1.0f - rand() % 4 / 5.0f),
glm::vec3(2.0 + rand() % 10 / 10.0f , rand() % 4 / 2.0f,        rand() % 4 / 5.0f),
glm::vec3(6.0 + rand() % 10 / 10.0f , rand() % 2 / 2.0f,        rand() % 10 / 5.0f),
glm::vec3(-4.0 + rand() % 10 / 10.0f, rand() % 8 / 2.0f,        rand() % 4 / 5.0f),
glm::vec3(-2.0 + rand() % 10 / 10.0f, rand() % -8 / 2.0f,        rand() % -14 / 5.0f) };
glm::vec3 offsetVecBubble[10] = { glm::vec3( rand() % 10 /20.0f, 1.0f + rand() % 3 / 2.0f,1.0f - rand() % 4 / 5.0f),
glm::vec3( rand() % 10 / 30.0f , 0.2f+rand() % 4 / 20.0f,        rand() % 2 / 5.0f),
glm::vec3( rand() % 10 / 30.0f,  0.3f+rand() % 12 / 20.0f,       0.0f - rand() % 4 / 5.0f),
glm::vec3( rand() % 10 / 30.0f,  0.1f+rand() % 10 / 20.0f,       0.5f - rand() % -2 / 5.0f),
glm::vec3( rand() % 10 / 30.0f, -0.1f+rand() % 5 / 20.0f,        1.0f - rand() % 4 / 5.0f),
glm::vec3(rand() % 10 / 20.0f, 1.0f + rand() % 3 / 2.0f,1.0f - rand() % 4 / 5.0f),
glm::vec3(rand() % 10 / 30.0f , 0.2f + rand() % 4 / 20.0f,        rand() % 2 / 5.0f),
glm::vec3(rand() % 10 / 30.0f,  0.3f + rand() % 12 / 20.0f,       0.0f - rand() % 4 / 5.0f),
glm::vec3(rand() % 10 / 30.0f,  0.1f + rand() % 10 / 20.0f,       0.5f - rand() % -2 / 5.0f),
glm::vec3(rand() % 10 / 30.0f, -0.1f + rand() % 5 / 20.0f,        1.0f - rand() % 4 / 5.0f),
};

//Initial mesh transformation, used for Imgui, hard coded for final scene
glm::vec3 meshpos(0.0f, 0.0f, 0.0f);
glm::vec3 particlepos(0.0f, 0.7f, 0.5f);
float scaleMesh = 1.0f;
bool mesh_enabled = true;

//define camera parameters
float cameraOffset = 1.0f;
float camangle = 0.0f;
glm::vec3 campos(0.0f, 0.7f, -0.5f);
glm::vec3 camLookAt(0.0f, 0.7f, -1.0f);
float aspect = 1.0f;
std::stack<glm::mat4> glmStack_modelViewMatrix;

//Matrix used for calculate shadow map
//this offsetMat converts shadowMap sampling from [-1.0, 1.0] to [0.0, 1.0]
//depthProjectionMatrix is the light projection volume which defines the bondary for shadowmap
glm::mat4 offsetMat = glm::mat4(
	glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
	glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
	glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
	glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
);
glm::mat4 depthProjectionMatrix = glm::ortho<float>(-SHADOW_SAMPLE, SHADOW_SAMPLE, -SHADOW_SAMPLE, 
													SHADOW_SAMPLE, -SHADOW_SAMPLE, SHADOW_SAMPLE);
//defines the Loop time for particle system
float life_span = 40.0f;
float begin_time = 0.0f;

//playsound, from irrKlang librabry
void PlaySound()
{
	// start the sound engine with default parameters
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	if (!engine)
		printf("sound Error"); // error starting up the engine

	// play some sound stream, looped
	irrklang::ISound* snd  = engine->play2D(".\\Sound\\SongOftheSea.mp3", true, false, true, irrklang::ESM_AUTO_DETECT, true);

	if (snd)
	{
		irrklang::ISoundEffectControl* fx = snd->getSoundEffectControl();
		if (fx)
		{
			// enable the echo sound effect for this sound
			//fx->enableEchoSoundEffect();
			//fx->enableWavesReverbSoundEffect();
			fx->enableChorusSoundEffect();
		}
	}
	
}

//a periodical function which produce periodical piecewise function.
//the function starts from 0 and increase with time, with slope 1 and period 40s
float GetTimer()
{

	float delta_t = glutGet(GLUT_ELAPSED_TIME)*0.001f - begin_time;
	float y = delta_t;
	if (delta_t >= life_span)
	{
		begin_time = glutGet(GLUT_ELAPSED_TIME)*0.001f;
	}
	return y;
}

//Gui function used for debugging
void draw_gui()
{
   glUseProgram(PhongNormal_SP);
   static bool first_frame = true;
   ImGui_ImplGlut_NewFrame();
   static bool show_window = true;

   ImGui::Begin("Texture and Alpha", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

   ImGui::Checkbox("Draw mesh", &mesh_enabled); ImGui::SameLine();
   

   ImGui::SliderFloat("View angle", &camangle, -180.0f, +180.0f);
   ImGui::SliderFloat("ScaleRay", &scaleMesh, -50.0f, +50.0f);
   ImGui::SliderFloat3("directLight", &directLight[0], -1.0f, 1.0f);

   ImGui::SliderFloat3("cam Pos", &campos[0],-20.0f,20.0f);
   ImGui::SliderFloat3("cam Look At", &camLookAt[0], -20.0f, 20.0f);
   ImGui::SliderFloat3("ParticlePos", &particlepos[0], -100.0f, 50.0f);
   ImGui::SliderFloat3("LightColor", &lightColor[0], 0.0f, 2.0f);

   static float slider = 0.0;


   /*static float alpha = 10.0f;
   if (ImGui::SliderFloat("Dullness", &alpha, 1.0f, 100.0f))
   {
      int alpha_loc = glGetUniformLocation(shader_program, "alpha");
      if (alpha_loc != -1)
      {
         glUniform1f(alpha_loc, alpha);
      }
   }

   static float f = 0.0f;
   if (ImGui::SliderFloat("Wrap", &f, 0.0f, 1.0f))
   {
      int f_loc = glGetUniformLocation(shader_program, "f");
      if (f_loc != -1)
      {
         glUniform1f(f_loc, f);
      }
   }

   static bool use_texture = true;
   if (ImGui::Checkbox("Use Texture", &use_texture))
   {
      int use_tex_loc = glGetUniformLocation(shader_program, "use_texture");
      if (use_tex_loc != -1)
      {
         glUniform1i(use_tex_loc, use_texture);
      }
   }*/

   const GLenum minification_modes[2] = {GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR };
   const char* minification_modes_labels[] = { "GL_NEAREST", "GL_LINEAR_MIPMAP_LINEAR" };
   static int current_minification_mode = 1;
   if(ImGui::Combo("Texture man filter", &current_minification_mode, minification_modes_labels, 2))
   {
	   //glBindTexture(GL_TEXTURE_2D, texture_id[0]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification_modes[current_minification_mode]);
	  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minification_modes[current_minification_mode]);
	  //glGenerateMipmap(GL_TEXTURE_2D);
   }


   ImGui::End();

   ImGui::Render();
   first_frame = false;
}

// Call this function to generate shadow map|| called by initOpenGL()
void genShadowMap()
{
	// size of the shadow map
	GLuint shadowMapSize = SHADOWRES;

	// create the shadow map

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadowMapSize, shadowMapSize,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// GL_CLAMP_TO_EDGE setups the shadow map in such a way that
	// fragments for which the shadow map is undefined
	// will get values from closest edges of the shadow map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// comparison mode of the shadow map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	////////////////////////////////////////////////////

	// create framebuffer
	glGenFramebuffers(1, &shadowFramebuffer);

	// attach the shadow map to framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowFramebuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, shadowMap, 0);


	// depth is stored in z-buffer to which the shadow map is attached,
	// so there is no need for any color buffers
	glDrawBuffer(GL_NONE);
}

//Call this function to draw depth value in shadow map
/*	General step for depth map shadow:
	First create a new framebuffer, and calculate depth value from the light view
	store the depth value in a predefined shadow map (the higher the resolution, the better quality it will be)
	change to the normal buffer and draw the scene from the camera view and light view
	compare the depth value between the light view and depth map, if the value is greater,which means in shadow
*/
void draw_Shadows(glm::vec3 meshPos, MeshData mesh_data,
	GLuint shadowFrameBuffer, GLuint shadowMap, GLuint shadowProgram,
	const glm::mat4& P, const glm::mat4& V, glm::vec3& Light)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowFramebuffer);
	// size of the viewport should be equal to size of the shadow map
	glViewport(0, 0, SHADOWRES, SHADOWRES);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// clear the shadow map with default value
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POLYGON_OFFSET_FILL);

	glPolygonOffset(2.0f, 1.0f);

	glUseProgram(depthShadowProgramID);

	glm::mat4 T = glm::translate(meshpos);
	glm::mat4 S = glm::scale(glm::vec3(1.0f));
	//glm::mat4 R = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 M = T*S;// *R;

					  // combination of matrices into shadowMat
	glm::mat4 depthViewMatrix = glm::lookAt(Light, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


	int u_modelMat_loc = glGetUniformLocation(shadowProgram, "u_modelMat");
	if (u_modelMat_loc != -1)
	{
		glUniformMatrix4fv(u_modelMat_loc, 1, GL_FALSE, glm::value_ptr(M));
	}

	int u_shadowViewMat_loc = glGetUniformLocation(shadowProgram, "u_shadowViewMat");
	if (u_shadowViewMat_loc != -1)
	{
		glUniformMatrix4fv(u_shadowViewMat_loc, 1, GL_FALSE, glm::value_ptr(depthViewMatrix));
	}

	int u_shadowprojection_loc = glGetUniformLocation(shadowProgram, "u_shadowProjMat");
	if (u_shadowprojection_loc != -1)
	{
		glUniformMatrix4fv(u_shadowprojection_loc, 1, GL_FALSE, glm::value_ptr(depthProjectionMatrix));
	}
	glBindVertexArray(mesh_data.mVao);
	glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0);

}

//Draw cube map
void draw_cube(const glm::mat4& P, const glm::mat4& V)
{
	glUseProgram(cube_shader_program);
	int PVM_loc = glGetUniformLocation(cube_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::vec3 translateCube(0.0f, 0000.0f, 0.0f);
		glm::mat4 Msky = glm::translate(translateCube) * glm::scale(glm::vec3(10000.0f));
		glm::mat4 PVM = P*V*Msky;
		PVM[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);
	int cube_loc = glGetUniformLocation(cube_shader_program, "cubemap");
	if (cube_loc != -1)
	{
		glUniform1i(cube_loc, 1); // we bound our texture to texture unit 1
	}
	draw_cube_vao(cube_vao);
}

//Draw objects with normal map, specular, diffuse|| Shadow is also applied for most situations
//main drawing function, most objects are drawn by this function call
void draw_SolidNormal(glm::vec3 meshPos, GLuint shader_program,
	GLuint texture_ID, GLuint normal_ID,  MeshData mesh_data,
	GLuint shadowMap, 
	const glm::mat4& P, const glm::mat4& V, glm::vec3& Light, GLuint caustic_ID = -1 , 
	GLfloat shininess = 1.0f)
{
	glm::mat4 T = glm::translate(meshpos);
	glm::mat4 S = glm::scale(glm::vec3(1.0f));
	//glm::mat4 R = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 M = T*S;

	glm::mat4 depthViewMatrix = glm::lookAt(Light, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	// combination of matrices into shadowMat
	glm::mat4 shadowMat = offsetMat * depthProjectionMatrix * depthViewMatrix;
	

	glUseProgram(shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	int tex_loc = glGetUniformLocation(shader_program, "diffuseMap");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	if (normal_ID != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_ID);
		int normal_loc = glGetUniformLocation(shader_program, "normalMap");
		if (normal_loc != -1)
		{
			glUniform1i(normal_loc, 1); // we bound our texture to texture unit 1
		}
	}
	if (caustic_ID != -1)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, caustic_ID);
		int caustic_loc = glGetUniformLocation(shader_program, "causticMap");
		if (caustic_loc != -1)
		{
			glUniform1i(caustic_loc, 3); // we bound our texture to texture unit 1
		}
	}

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	int shadowmap_loc = glGetUniformLocation(shader_program, "u_shadowMap");
	if (shadowmap_loc != -1)
	{
		glUniform1i(shadowmap_loc, 2); // we bound our texture to texture unit 2
	}

	int Light_loc = glGetUniformLocation(shader_program, "directL");
	if (Light_loc != -1)
	{
		glUniform3f(Light_loc, Light[0], Light[1], Light[2]); // we bound our texture to texture unit 2
	}
	int LightColor_loc = glGetUniformLocation(shader_program, "LightColor");
	if (LightColor_loc != -1)
	{
		glUniform3f(LightColor_loc, lightColor[0], lightColor[1], lightColor[2]); // we bound our texture to texture unit 2
	}

	int Shiness_loc = glGetUniformLocation(shader_program, "shine");
	if (Shiness_loc != -1)
	{
		glUniform1f(Shiness_loc, shininess); // we bound our texture to texture unit 2
	}

	int PVM_loc = glGetUniformLocation(shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P*V*M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int M_loc = glGetUniformLocation(shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	int shadowMat_loc = glGetUniformLocation(shader_program, "u_shadowMat");
	if (shadowMat_loc != -1)
	{
		glUniformMatrix4fv(shadowMat_loc, 1, false, glm::value_ptr(shadowMat));
	}

	glBindVertexArray(mesh_data.mVao);
	glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0);
}

//Draw god light rays
void draw_Rays(glm::vec3 scale, glm::vec3 meshPos, GLuint shader_program,
	GLuint texture_ID,  MeshData mesh_data,
	const glm::mat4& P, const glm::mat4& V)
	{
	glm::mat4 T = glm::translate(meshPos);
	glm::mat4 S = glm::scale(scale);
	//glm::mat4 R = glm::rotate(0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 M = T*S;

		
	glUseProgram(shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	int tex_loc = glGetUniformLocation(shader_program, "diffuseMap");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}
		
	int T_loc = glGetUniformLocation(shader_program, "T");
	if (T_loc != -1)
	{
		
		glUniformMatrix4fv(T_loc, 1, false, glm::value_ptr(T));
	}
	int S_loc = glGetUniformLocation(shader_program, "S");
	if (S_loc != -1)
	{
		
		glUniformMatrix4fv(S_loc, 1, false, glm::value_ptr(S));
	}

	int M_loc = glGetUniformLocation(shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}
	int P_loc = glGetUniformLocation(shader_program, "PVM");
	if (P_loc != -1)
	{
		glm::mat4 PVM = P*V*M;
		glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(PVM));
	}

	glBindVertexArray(mesh_data.mVao);
	glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0);
}

//Draw particle fish
void draw_Particle(int behaviorCon, float speed,float rotation_offset, float rotateOffset, glm::vec3 meshPos, GLuint shader_program,
	GLuint texture_ID, GLuint normal_ID, MeshData mesh_data,
	GLuint shadowMap,
	const glm::mat4& P, const glm::mat4& V, glm::vec3& Light)
{
	//glm::mat4 T = glm::translate(particlepos);
	glm::mat4 T = glm::translate(meshPos);
	glm::mat4 T1 = glm::translate(glm::vec3(0.0f, 0.0f, rotation_offset));
	glm::mat4 S = glm::scale(glm::vec3(1.0f));
	glm::mat4 R = glm::rotate(rotateOffset + speed *glutGet(GLUT_ELAPSED_TIME)*0.02f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 M;
	if (behaviorCon == 1)
	{
		if (speed < 0.0f)
		{
			glm::mat4 S = glm::scale(glm::vec3(-1.0f, 1.0f, -1.0f));
		}
		M = T*S*R*T1;
	}
	if (behaviorCon == 2)
	{
		R = glm::rotate(rotation_offset, glm::vec3(0.0f, 1.0f, 0.0f));
		M = T*S*R;
	}
	if (behaviorCon == 3)
	{
		R = glm::rotate(rotation_offset, glm::vec3(0.0f, 1.0f, 0.0f));
		M = T*S*R;
	}
	if (behaviorCon == 4)
	{
		R = glm::rotate(rotation_offset, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 S = glm::scale(glm::vec3(-1.0f, 1.0f, 1.0f));
		M = T*S*R;
	}
	

	glm::mat4 depthViewMatrix = glm::lookAt(Light, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	// combination of matrices into shadowMat
	glm::mat4 shadowMat = offsetMat * depthProjectionMatrix * depthViewMatrix;


	glUseProgram(shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	int tex_loc = glGetUniformLocation(shader_program, "diffuseMap");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	if (normal_ID != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_ID);
		int normal_loc = glGetUniformLocation(shader_program, "normalMap");
		if (normal_loc != -1)
		{
			glUniform1i(normal_loc, 1); // we bound our texture to texture unit 1
		}
	}

	int behavior_loc = glGetUniformLocation(shader_program, "behavior");
	if (behavior_loc != -1)
	{
		glUniform1i(behavior_loc, behaviorCon); // we bound our texture to texture unit 1
	}
	int speed_loc = glGetUniformLocation(shader_program, "fishSpeed");
	if (speed_loc != -1)
	{
		glUniform1f(speed_loc, speed); // we bound our texture to texture unit 1
	}

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, shadowMap);
	//int shadowmap_loc = glGetUniformLocation(shader_program, "u_shadowMap");
	//if (shadowmap_loc != -1)
	//{
	//	glUniform1i(shadowmap_loc, 2); // we bound our texture to texture unit 2
	//}

	int Light_loc = glGetUniformLocation(shader_program, "directL");
	if (Light_loc != -1)
	{
		glUniform3f(Light_loc, Light[0], Light[1], Light[2]); // we bound our texture to texture unit 2
	}

	int PVM_loc = glGetUniformLocation(shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P*V*M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int M_loc = glGetUniformLocation(shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	/*int shadowMat_loc = glGetUniformLocation(shader_program, "u_shadowMat");
	if (shadowMat_loc != -1)
	{
		glUniformMatrix4fv(shadowMat_loc, 1, false, glm::value_ptr(shadowMat));
	}*/

	glBindVertexArray(mesh_data.mVao);
	glDrawElementsInstanced(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0, MAXFISHPARTICLE);
}

//Draw particle bubble
void draw_ParticleBubble(  glm::vec3 meshPos, GLuint shader_program,
	GLuint texture_ID, GLuint normal_ID, MeshData mesh_data,
	GLuint shadowMap,
	const glm::mat4& P, const glm::mat4& V, glm::vec3& Light)
{
	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	//glm::mat4 T = glm::translate(particlepos);
	glm::mat4 T = glm::translate(meshPos);
	glm::mat4 S = glm::scale(glm::vec3(1.0f));
	glm::mat4 R = glm::rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 M = T*S*R;
	
	
	glUseProgram(shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	int tex_loc = glGetUniformLocation(shader_program, "diffuseMap");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	if (normal_ID != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal_ID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		int normal_loc = glGetUniformLocation(shader_program, "normalMap");
		if (normal_loc != -1)
		{
			glUniform1i(normal_loc, 1); // we bound our texture to texture unit 1
		}
	}


	int Light_loc = glGetUniformLocation(shader_program, "directL");
	if (Light_loc != -1)
	{
		glUniform3f(Light_loc, Light[0], Light[1], Light[2]); // we bound our texture to texture unit 2
	}

	int T_loc = glGetUniformLocation(shader_program, "T");
	if (T_loc != -1)
	{
		glm::mat4 PVM = P*V*M;
		glUniformMatrix4fv(T_loc, 1, false, glm::value_ptr(T));
	}

	int S_loc = glGetUniformLocation(shader_program, "S");
	if (S_loc != -1)
	{
		glUniformMatrix4fv(S_loc, 1, false, glm::value_ptr(S));
	}

	int R_loc = glGetUniformLocation(shader_program, "R");
	if (R_loc != -1)
	{
		glUniformMatrix4fv(R_loc, 1, false, glm::value_ptr(R));
	}

	int V_loc = glGetUniformLocation(shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}
	int P_loc = glGetUniformLocation(shader_program, "P");
	if (P_loc != -1)
	{
		glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(P));
	}


	
	glBindVertexArray(mesh_data.mVao);
	glDrawElementsInstanced(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0, 10);
}

//Main draw function, called every frame
void display()
{
	//std::stack<mat4> glm_ModelViewMatrix; //cpu-side
	//glm_ModelViewMatrix.push(glm_ModelViewMatrix.top());
	//glm_ModelViewMatrix.top() *= translate(vec3(0.5, 0.5, 0.0));
	//glm_ModelViewMatrix.top() *= rotate(45.0f, vec3(0.0, 0.0, 1.0));
	//glm_ModelViewMatrix.top() *= scale(vec3(0.2, 0.2, 0.2));
	//glUniformMatrix4fv(glm_ModelViewMatrix_id, 1, false, address(glm_ModelViewMatrix.top()));  // set the rotation/translation/scale matrix 
	//Draw_vao(0);
	//glm_ModelViewMatrix.pop();
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer
   
   //Create camera shaking effects
   //using Sin to create periodical function, and move the camera based on the period
   float time_sec = 0.001f*glutGet(GLUT_ELAPSED_TIME);
   if (sin(8*time_sec * PI/180.0f) < 0)
   {
	   cameraOffset = -1.0f;
   }
   else
   {
	   cameraOffset = 1.0f;
   }
   campos = glm::vec3(campos.x + 0.0008f*cameraOffset, campos.y + 0.0008f*cameraOffset, campos.z);
   camLookAt = glm::vec3(camLookAt.x + 0.0008f*cameraOffset, camLookAt.y + 0.0008f*cameraOffset, camLookAt.z);
   directLight.x = directLight.x + 0.0002f*cameraOffset;

   //View amd projection matrix
   glm::mat4 V = glm::lookAt(campos, camLookAt, glm::vec3(0.0f, 1.0f, 0.0f))*glm::rotate(camangle, glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 P = glm::perspective(55.0f, aspect, 0.1f, 6000.0f); //not affine
  
   if(mesh_enabled)
   {
	   //First draw shadow map, write the depth value in shadow map
	   draw_Shadows(meshpos, shadowMesh, shadowFramebuffer, shadowMap, depthShadowProgramID,
			   P, V, directLight);
	  
	   // Change the buffer to main frame buffer and began normal object draw
	   glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	   //Reset the view port from shadow map draw viewport size
	   glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	   //Reset offset function. Offset is for hadling the shadow acne effects
	   glDisable(GL_POLYGON_OFFSET_FILL);
	   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   glEnable(GL_CULL_FACE);
	   glCullFace(GL_FRONT);
	   //Draw the back face of cube
	   draw_cube(P, V);

	   glCullFace(GL_BACK);
	   
	   //Draw the scene elements one by one
	   
	   //Draw submarine
	   draw_SolidNormal(meshpos, PhongNormal_SP, texture_id[3], normalTexture_id[3], mesh_data[2],
		   shadowMap, P, V, directLight, -1, 3.0f);
	   //Draw plane
	   draw_SolidNormal(meshpos, PhongNormal_SP, texture_id[3], normalTexture_id[3], mesh_data[3],
		   shadowMap, P, V, directLight, -1, 3.0f);
	   //Draw seaweed
	   draw_SolidNormal(meshpos, seaWeed_SP, texture_id[8], normalTexture_id[8], mesh_data[8],
		   shadowMap, P, V, directLight, -1, 3.0f);
	   //Draw algea
	   draw_SolidNormal(meshpos, PhongNormal_SP, texture_id[6], normalTexture_id[6], mesh_data[6],
		   shadowMap, P, V, directLight, -1, 3.0f);
	   
	   
	   //Draw dragonbone
	   draw_SolidNormal(meshpos, groundRock_SP, texture_id[1], normalTexture_id[1], mesh_data[1],
		   shadowMap, P, V, directLight, texture_id[18]);
	   //Draw ground
	   draw_SolidNormal(meshpos, groundRock_SP, texture_id[0], normalTexture_id[0], mesh_data[0],
		   shadowMap, P, V, directLight,texture_id[18]);
	   //Draw Sponge
	   draw_SolidNormal(meshpos, groundRock_SP, texture_id[5], normalTexture_id[5], mesh_data[5],
		   shadowMap, P, V, directLight, texture_id[18]);
	   //Draw rock01
	   draw_SolidNormal(meshpos, groundRock_SP, texture_id[9], normalTexture_id[9], mesh_data[9],
		   shadowMap, P, V, directLight, texture_id[18]);
	   //Draw rock02
	   draw_SolidNormal(meshpos, groundRock_SP, texture_id[10], normalTexture_id[10], mesh_data[10],
		   shadowMap, P, V, directLight, texture_id[18]);
	   //Draw rock03
	   draw_SolidNormal(meshpos, groundRock_SP, texture_id[11], normalTexture_id[11], mesh_data[11],
		   shadowMap, P, V, directLight, texture_id[18]);
	   //Draw rock04
	   draw_SolidNormal(meshpos, groundRock_SP, texture_id[12], normalTexture_id[12], mesh_data[12],
		   shadowMap, P, V, directLight, texture_id[18]);
	   //Draw rock05
	   draw_SolidNormal(meshpos, groundRock_SP, texture_id[13], normalTexture_id[12], mesh_data[12],
		   shadowMap, P, V, directLight, texture_id[18]);
	   
	   glDisable(GL_CULL_FACE);
	   

	   //Draw Red Algea. Double face
	   draw_SolidNormal(meshpos, redAlgea_SP, texture_id[4], normalTexture_id[4], mesh_data[4],
		   shadowMap, P, V, directLight, -1, 3.0f);
	   //Draw coraline. Double face
	   draw_SolidNormal(meshpos, PhongNormal_SP, texture_id[7], normalTexture_id[7], mesh_data[7],
		   shadowMap, P, V, directLight);

	   //Draw Particle Fish
	   draw_Particle(2, 2.0f,0.0f,0.0f,glm::vec3(-20.0f,0.0f,-20.0f), particleFish_SP, texture_id[15], normalTexture_id[15], mesh_data[14],
		   shadowMap, P, V, directLight);
	   draw_Particle(3, 1.5f, 20.0f, 0.0f,glm::vec3(-20.0f, 0.0f, -25.0f), particleFish_SP, texture_id[14], normalTexture_id[14], mesh_data[14],
		   shadowMap, P, V, directLight);
	   draw_Particle(3,2.2f, 0.0f,0.0f,glm::vec3(-30.0f, 0.0f, -25.0f), particleFish_SP, texture_id[15], normalTexture_id[15], mesh_data[14],
		   shadowMap, P, V, directLight);
	   for (int i = 0; i <= 3; i++)
	   {
		   draw_Particle(4, 2.0f, -15.0f -2*i, 0.0f, glm::vec3(20.0f + i*4.0f, 0.0f + i, -15.0f - i*2.0f), particleFish_SP, texture_id[14 + i % 3], normalTexture_id[14 + i % 3], mesh_data[14],
			   shadowMap, P, V, directLight);
		   draw_Particle(4, 2.0f, 30.0f + 3.0*i, 0.0f, glm::vec3(25.0f + i*4.0f, 0.0f - i, -15.0f + i*4.0f), particleFish_SP, texture_id[14 + i % 3], normalTexture_id[14 + i % 3], mesh_data[14],
			   shadowMap, P, V, directLight);
	   }
	   draw_Particle(1, 1.0f, 12.0f,0.0f, glm::vec3(31.0f, 8.0f, -85.0f), particleFish_SP, texture_id[15], normalTexture_id[15], mesh_data[14],
		   shadowMap, P, V, directLight);
	   draw_Particle(1, 1.2f, 12.0f, 0.0f, glm::vec3(-35.0f, 16.0f, -52.0f), particleFish_SP, texture_id[15], normalTexture_id[15], mesh_data[14],
		   shadowMap, P, V, directLight);
	   draw_Particle(1, 1.5f, 8.0f, 0.0f, glm::vec3(-35.0f, 16.0f, -52.0f), particleFish_SP, texture_id[14], normalTexture_id[14], mesh_data[14],
		   shadowMap, P, V, directLight);
	   draw_Particle(1, -1.5f, -9.0f, 0.0f, glm::vec3(32.0f, 12.0f, -51.0f), particleFish_SP, texture_id[16], normalTexture_id[16], mesh_data[14],
		   shadowMap, P, V, directLight);
	   draw_Particle(1, 1.5f, 8.0f, 0.0f, glm::vec3(0.0f, 16.0f, -26.0f), particleFish_SP, texture_id[14], normalTexture_id[14], mesh_data[14],
		   shadowMap, P, V, directLight);
	   draw_Particle(1, 1.0f, 8.0f, 0.0f, glm::vec3(0.0f, 0.0f, -26.0f), particleFish_SP, texture_id[14], normalTexture_id[14], mesh_data[14],
		   shadowMap, P, V, directLight);
	   draw_Particle(1, 1.0f, 8.0f, 0.0f, glm::vec3(12.0f, 4.0f, -15.0f), particleFish_SP, texture_id[14], normalTexture_id[14], mesh_data[14],
		   shadowMap, P, V, directLight);
	   draw_Particle(1, 1.6f, 12.0f, 0.0f, glm::vec3(12.0f, 6.0f, -100.0f), particleFish_SP, texture_id[14], normalTexture_id[14], mesh_data[14],
		   shadowMap, P, V, directLight);
		
	   //draw bubbles
	   glEnable(GL_BLEND);
	   //Blend mode should be additive for bubbles and rays, make them brighter
	   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	   glDepthMask(GL_FALSE);
	   for (int i = 0; i < 6; i++)
	   {
		   draw_ParticleBubble(glm::vec3(-2, -6-i*5.0f, -7), particleBubble_SP, texture_id[17], normalTexture_id[17], mesh_data[15],
			   shadowMap, P, V, directLight);
		   draw_ParticleBubble(glm::vec3(8, -10 - i*5.0f, -18), particleBubble_SP, texture_id[17], normalTexture_id[17], mesh_data[15],
			   shadowMap, P, V, directLight);
		   draw_ParticleBubble(glm::vec3(2.1, -2.1 - i*5.0f, -19), particleBubble_SP, texture_id[17], normalTexture_id[17], mesh_data[15],
			   shadowMap, P, V, directLight);
		   draw_ParticleBubble(glm::vec3(-5, -15 - i*5.0f, -23), particleBubble_SP, texture_id[17], normalTexture_id[17], mesh_data[15],
			   shadowMap, P, V, directLight);
		   draw_ParticleBubble(glm::vec3(14.5, -15 - i*5.0f, -60), particleBubble_SP, texture_id[17], normalTexture_id[17], mesh_data[15],
			   shadowMap, P, V, directLight);
	   }
	   
	   //Draw god rays
	   draw_Rays(glm::vec3(2.6), glm::vec3(9.6,15.4,-36), lightRay_SP, texture_id[19], mesh_data[16], P, V);
	   draw_Rays(glm::vec3(4.1), glm::vec3(-25, 27, -75), lightRay_SP, texture_id[19], mesh_data[16], P, V);
	   draw_Rays(glm::vec3(1.55), glm::vec3(15.4, 9.6, -22), lightRay_SP, texture_id[19], mesh_data[16], P, V);
	   draw_Rays(glm::vec3(1.55), glm::vec3(-28, 6.8, -33), lightRay_SP, texture_id[19], mesh_data[16], P, V);

	   glDepthMask(GL_TRUE);
	   glDisable(GL_BLEND);
   }

   //Disable this for final scene
   //draw_gui();
   glutSwapBuffers();
}

//Calculate the time
void idle()
{
   glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   float time_sec = 0.001f*time_ms;
   float time_Loop = GetTimer();
   
   

   glUseProgram(particleFish_SP);
   int time_loc = glGetUniformLocation(particleFish_SP, "time");
   if (time_loc != -1)
   {
      //double check that you are using glUniform1f
      glUniform1f(time_loc, time_sec);
   }

   int timeloop_loc = glGetUniformLocation(particleFish_SP, "timeLoop");
   if (timeloop_loc != -1)
   {
	   glUniform1f(timeloop_loc, time_Loop);
   }

  
   glUseProgram(particleBubble_SP);
   int timebubble_loc = glGetUniformLocation(particleBubble_SP, "time");
   if (timebubble_loc != -1)
   {
	   //double check that you are using glUniform1f
	   glUniform1f(timebubble_loc, time_sec);
   }
   int timeloopbubble_loc = glGetUniformLocation(particleBubble_SP, "timeLoop");
   if (timeloopbubble_loc != -1)
   {
	   glUniform1f(timeloopbubble_loc, time_Loop);
   }

   glUseProgram(groundRock_SP);
   int timeRock_loc = glGetUniformLocation(groundRock_SP, "time");
   if (timeRock_loc != -1)
   {
	   //double check that you are using glUniform1f
	   glUniform1f(timeRock_loc, time_sec);
   }

   glUseProgram(seaWeed_SP);
   int timeSeaWeed_loc = glGetUniformLocation(seaWeed_SP, "time");
   if (timeSeaWeed_loc != -1)
   {
	   //double check that you are using glUniform1f
	   glUniform1f(timeSeaWeed_loc, time_sec);
   }

   
	glUseProgram(redAlgea_SP);
   int timeredAlgea_loc = glGetUniformLocation(redAlgea_SP, "time");
   if (timeredAlgea_loc != -1)
   {
	   //double check that you are using glUniform1f
	   glUniform1f(timeredAlgea_loc, time_sec);
   }
   glUseProgram(lightRay_SP);
   int timeLightRay_loc = glGetUniformLocation(lightRay_SP, "time");
   if (timeLightRay_loc != -1)
   {
	   //double check that you are using glUniform1f
	   glUniform1f(timeLightRay_loc, time_sec);
   }
}

//Initial OpenGL
void initOpenGl()
{
   glewInit();

   //Enable multisampling
   glEnable(GL_MULTISAMPLE);

   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_POINT_SPRITE);       // allows textured points
   glEnable(GL_PROGRAM_POINT_SIZE); //allows us to set point size in vertex shader
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  
 
   genShadowMap();

   //Load textures here
   //Load Ground Textures
   texture_id[0] = LoadTexture(groundTexture);
   normalTexture_id[0] = LoadTexture(groundNormalTexture);
   //Load Dragonbone Textures
   texture_id[1] = LoadTexture(DragonBoneTexture);
   normalTexture_id[1] = LoadTexture(DragonBoneNormalTexture);
   //Load Submarine
   texture_id[2] = LoadTexture(SubMarineTexture);
   normalTexture_id[2] = LoadTexture(SubMarineNormalTexture);
   //Load Plane
   texture_id[3] = LoadTexture(PlaneTexture);
   normalTexture_id[3] = LoadTexture(PlaneNormalTexture);
   //Load Red algea
   texture_id[4] = LoadTexture(RedAlgeaTexture);
   normalTexture_id[4] = LoadTexture(RedAlgeaNormalTexture);
   //Load Sponge
   texture_id[5] = LoadTexture(SpongeTexture);
   normalTexture_id[5] = LoadTexture(SpongeNormalTexture);
   //Load algea
   texture_id[6] = LoadTexture(AlgeaTexture);
   normalTexture_id[6] = LoadTexture(AlgeaNormalTexture);
   //Load Coraline
   texture_id[7] = LoadTexture(CoralineTexture);
   normalTexture_id[7] = LoadTexture(CoralineNormalTexture);
   //Load seaweed
   texture_id[8] = LoadTexture(SeeweedTexture);
   normalTexture_id[8] = LoadTexture(SeeweedNormalTexture);
   //Load rock01
   texture_id[9] = LoadTexture(Rock01Texture);
   normalTexture_id[9] = LoadTexture(Rock01NormalTexture);
   //Load rock02
   texture_id[10] = LoadTexture(Rock02Texture);
   normalTexture_id[10] = LoadTexture(Rock02NormalTexture);
   //Load rock03
   texture_id[11] = LoadTexture(Rock03Texture);
   normalTexture_id[11] = LoadTexture(Rock03NormalTexture);
   //Load rock04
   texture_id[12] = LoadTexture(Rock04Texture);
   normalTexture_id[12] = LoadTexture(Rock04NormalTexture);
   //Load rock05
   texture_id[13] = LoadTexture(Rock05Texture);
   normalTexture_id[13] = LoadTexture(Rock05NormalTexture);
   //Load fish01
   texture_id[14] = LoadTexture(fishTexture01);
   normalTexture_id[14] = LoadTexture(fishTextureN01);
   //Load fish02
   texture_id[15] = LoadTexture(fishTexture02);
   normalTexture_id[15] = LoadTexture(fishTextureN02);
   //Load fish03
   texture_id[16] = LoadTexture(fishTexture03);
   normalTexture_id[16] = LoadTexture(fishTextureN03);
   //Load bubble
   texture_id[17] = LoadTexture(bubbleTexture);
   normalTexture_id[17] = LoadTexture(bubbleTextureN);
   //Load caustic texture
   texture_id[18] = LoadTexture(causticTexture);
   //Load light rays
   texture_id[19] = LoadTexture(lightRayTexture);
   //Load Background Texture
   texture_id[21] = LoadTexture(BackgroundTexture);
   
   //Init shadow shader
   depthShadowProgramID = InitShader(shadowMap_VS.c_str(), shadowMap_FS.c_str());
   //Init default shader
   PhongNormal_SP = InitShader(ground_VS.c_str(), ground_FS.c_str());

   //Load meshes here
   shadowMesh = LoadMesh(shadowmesh_name);
   mesh_data[1] = LoadMesh(DragonBonemesh_name);
   mesh_data[2] = LoadMesh(SubMarinemesh_name);
   mesh_data[3] = LoadMesh(Planemesh_name);
   mesh_data[5] = LoadMesh(Spongemesh_name);
   mesh_data[6] = LoadMesh(Algeamesh_name);
   mesh_data[7] = LoadMesh(Coralinemesh_name);
   
   //Init rock shader, different specular compared to default
   groundRock_SP = InitShader(groundRock_VS.c_str(), groundRock_FS.c_str());
   mesh_data[0] = LoadMesh(groundmesh_name);
   mesh_data[9] = LoadMesh(Rock01mesh_name);
   mesh_data[10] = LoadMesh(Rock02mesh_name);
   mesh_data[11] = LoadMesh(Rock03mesh_name);
   mesh_data[12] = LoadMesh(Rock04mesh_name);
   mesh_data[13] = LoadMesh(Rock05mesh_name);

   //Init seaweed shader, with animation 
   seaWeed_SP = InitShader(seaWeed_VS.c_str(), seaWeed_FS.c_str());
   mesh_data[8] = LoadMesh(Seeweedmesh_name);
   //Init redalgea shader, with animation and alpha mask
   redAlgea_SP = InitShader(redAlgea_VS.c_str(), redAlgea_FS.c_str());
   mesh_data[4] = LoadMesh(RedAlgeamesh_name);
   //Init light ray shader, with additive alpha blending, no other complex channels
   lightRay_SP = InitShader(lightRay_VS.c_str(), lightRay_FS.c_str());
   mesh_data[16] = LoadMesh(lightRayMesh);
   //Init fish shader, with animation and alpha mask, different from redalgea
   particleFish_SP = InitShader(particleFish_VS.c_str(), particleFish_FS.c_str());
   mesh_data[14] = LoadParticles(fishmesh, offsetVec);

   //load the sky cube
   cubemap_id = LoadCube(cube_name);
   cube_shader_program = InitShader(cube_vs.c_str(), cube_fs.c_str());
   cube_vao = create_cube_vao();
   //Init bubble shader, with additive alpha blending
   particleBubble_SP = InitShader(particleBubble_VS.c_str(), particleBubble_FS.c_str());
   mesh_data[15] = LoadParticles(bubbleMesh, offsetVecBubble);
   
   
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);
   if (key == 'f')
   {
	   //glViewport(0, 0, 1920, 1080);
	   glutFullScreenToggle();
	   glutPostRedisplay();
   }
}

void keyboard_up(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialUpCallback(key);
}

void passive(int x, int y)
{
   ImGui_ImplGlut_PassiveMouseMotionCallback(x, y);
}

void special(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialCallback(key);
}

void motion(int x, int y)
{
   ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void mouse(int button, int state, int x, int y)
{
   ImGui_ImplGlut_MouseButtonCallback(button, state);
}

//This function handles viewport size
void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   aspect = (float)w / h;
}

int main(int argc, char **argv)
{
	
   //Configure initial window state
   glutInit(&argc, argv);
   glutSetOption(GLUT_MULTISAMPLE, 4);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH| GLUT_MULTISAMPLE);
   //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
   glutInitWindowPosition(5, 5);
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   int win = glutCreateWindow("520Final Project");


   //Register callback functions with glut. 
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutKeyboardUpFunc(keyboard_up);
   glutSpecialUpFunc(special_up);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
   glutIdleFunc(idle);
   glutReshapeFunc(reshape);


   initOpenGl();
   ImGui_ImplGlut_Init(); // initialize the imgui system
   PlaySound();
   //Enter the glut event loop.
   glutMainLoop();
   //PlaySound();
   glutDestroyWindow(win);
   
   return 0;
}