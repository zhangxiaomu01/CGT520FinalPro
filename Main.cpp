#include <windows.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "imgui_impl_glut.h" 

#include "Define_Mesh_and_Textures.h"
#include "UseShader.h"

#define MAXNUM_SHADER 2
#define MAXNUM_MODEL 2



static const char* modelPath = ".\\Models\\*";  // no need to copy this into a buffer, just use a const
static std::string modelPathAdder = ".\\Models\\";

static const char* shaderPath = ".\\Shaders\\*";
static std::string shaderPathAdder = ".\\Shaders\\";

static std::string vertex_shader[MAXNUM_SHADER];// = { ".\\Shaders\\Default_vs.glsl", ".\\Shaders\\Phong_vs.glsl" };
static std::string fragment_shader[MAXNUM_SHADER];// = { ".\\Shaders\\Default_fs.glsl", ".\\Shaders\\Phong_fs.glsl" };
static GLuint shader_program[MAXNUM_SHADER];// = { -1, -1 };

static std::string model_Record[100];
static std::string shader_Record[100];

static GLuint texture_id[MAXNUM_SHADER] = { -1, -1 }; //Texture map for fish
MeshData mesh_data[MAXNUM_SHADER];

int ShaderCounter = 0;


float angle = 0.0f;
static float aspectRatio = 0.0;



//Draw Gui
//void draw_gui()
//{
//   ImGui_ImplGlut_NewFrame();
//
//   //uncomment the following line to create a slider which changes the viewing angle
//   //ImGui::SliderFloat("View angle", &angle, -180.0f, +180.0f);
//
//   static bool show_test = false;
//   ImGui::ShowTestWindow(&show_test);
//   ImGui::Render();
// }

void ListFile(const char* filePath, std::string pathAdder, static std::string label_Record[])
{
	
	char label[128];
	int i = 0;
	int k = 0;
	HANDLE hFind;
	WIN32_FIND_DATAA data2; // <- WIN32_FIND_DATAA if using char strings (instead of TCHAR strings)
	hFind = FindFirstFileA(filePath, &data2);  // <- FindFirstFileA if using char strings (instead of TCHAR strings)

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			sprintf(label, data2.cFileName, i);
			label_Record[i] = data2.cFileName;
			if (label_Record[i].find(".obj") <20 || label_Record[i].find(".jpg") <20 || label_Record[i].find(".bmp") <20
				|| label_Record[i].find("_vs") <20 || label_Record[i].find("_fs") <20)
			{
				label_Record[k] = data2.cFileName;
				
				label_Record[k] = pathAdder + label_Record[k];
				std::cout << label_Record[k] << std::endl;
				k++;
			}
			i++;
		} while (FindNextFileA(hFind, &data2));  // FindNextFileA if using char strings (instead of TCHAR strings)

		FindClose(hFind);  // don't forget to close it when you're done!
	}

}


void loadShaders()
{
	int selectShader = 0;
	ListFile(shaderPath,shaderPathAdder, shader_Record);
	for (int i =0; i<MAXNUM_SHADER;i++)
	{
		vertex_shader[i] = shader_Record[selectShader+1];
		fragment_shader[i] = shader_Record[selectShader];
		shader_program[i] = -1;
		selectShader += 2;
	}

}

void loadModelTexs()
{
	int selectModel = 0;
	ListFile(modelPath,modelPathAdder, model_Record);
	for (int i = 0; i< MAXNUM_MODEL;i += 1)
	{
		mesh_data[i] = loadM_T(model_Record[selectModel], model_Record[selectModel+1]).models;
		texture_id[i] = loadM_T(model_Record[selectModel], model_Record[selectModel+1]).textures;
		selectModel = selectModel + 2;
	}
}


// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer
   
   glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 
				 glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 P = glm::perspective(40.0f, aspectRatio, 0.1f, 100.0f);
   glm::mat4 M = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f))*glm::scale(glm::vec3(mesh_data[0].mScaleFactor));
   
   
   shaderSelctor(shader_program[0], ShaderCounter, mesh_data[0], texture_id[0], M, V, P);

   M = glm::translate(glm::vec3(1.0f,0.0f,0.0f))*glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f))
	   *glm::scale(glm::vec3(mesh_data[1].mScaleFactor));;
   shaderSelctor(shader_program[1], ShaderCounter, mesh_data[1], texture_id[1], M, V, P);


   //draw_gui();

   glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   float time_sec = 0.001f*time_ms;
   int time_loc = glGetUniformLocation(shader_program[0], "time");
   if (time_loc != -1)
   {
      glUniform1f(time_loc, time_sec);
   }
}

void testLoadedModel()
{
	for (int i = 0; i < 2; i++) 
	{
		if (mesh_data[i].mVao != -1)
		{
			BufferIndexedVerts(mesh_data[i]);
		}
	}
		
}


void printGlInfo()
{
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
}

void initOpenGl(int &shader_C)
{
    glewInit();

    glEnable(GL_DEPTH_TEST);


	loadShaders();

	testLoadedModel();
	//ListFile(modelPath,modelPathAdder,model_Record);

    reload_shader(shader_program[0], vertex_shader[0], fragment_shader[0]);
	shader_C += 1;
	reload_shader(shader_program[1], vertex_shader[1], fragment_shader[1]);
	shader_C += 1;
	
	loadModelTexs();

   //mesh and texture to be rendered
   
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);
   //std::cout << "key : " << key << ", x: " << x << ", y: " << y << std::endl;

   switch(key)
   {
      case 'r':
      case 'R':
		  reload_shader(shader_program[1], vertex_shader[1], fragment_shader[1]);
      break;
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
   ImGui_ImplGlut_PassiveMouseMotionCallback(x,y);
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

void changeSize(int x, int y)
{
	glViewport(0, 0, x, y);
	if (y != 0)
	{
		aspectRatio = float(x) / float(y);
	}
	
}

int main (int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (900, 900);
   int win = glutCreateWindow ("Final Project Jun Zhang");

   printGlInfo();
   initOpenGl(ShaderCounter);
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
   glutReshapeFunc(changeSize);

   
   ImGui_ImplGlut_Init(); // initialize the imgui system

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}