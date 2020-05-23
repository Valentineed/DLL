// MTL_ModernOpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <GLFW/glfw3.h>
#include "Resources/Texture.h"
#include "Resources/Shader.h"
#include "Resources/Mesh.h"
#include "Resources/Scene.h"
#include "Resources/LightManager.h"
#include "ResourceGFX.h" 
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// settings
int xMonitor, yMonitor;
const unsigned int SCR_WIDTH = 1250;
const unsigned int SCR_HEIGHT = 600;
bool wireframe = false;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_iconify_callback(GLFWwindow* window, int iconified);
void window_maximize_callback(GLFWwindow* window, int maximized);
void processInput(GLFWwindow* window);
float lastTime = 0.f, now, delta;

//SCENE
Resources::Scene g_scene;

//camera settings
bool g_active_mouse = true;
bool g_mouse_cursor = true;
bool g_firstMouse = true;
float g_lastX = (float)SCR_WIDTH / 2.f;
float g_lastY = (float)SCR_HEIGHT / 2.f;


int main()
{
	std::cout << "Hello World!\n";
	std::cout << "Bom dia !\n";
	std::cout << "Salut !\n";

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//---> Set a version OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//-/
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_DECORATED, true);
	glfwWindowHint(GLFW_ACCUM_RED_BITS, 8);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwSetGamma(glfwGetPrimaryMonitor(), 1.);//NICEEEEEEE

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// glfw window creation
	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MTL_ModernOpenGL", glfwGetPrimaryMonitor(), nullptr); //this->option genrete fullscreen

	//glfwSetMonitorCallback(monitor_callback);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MTL_ModernOpenGL", nullptr, nullptr);
	int xpos, ypos, workWidth, workHeight;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, &workWidth, &workHeight);

	//RAII
	{
	GLFWimage icons;
	Resources::TemporaryTexture test = Resources::TemporaryTexture::TemporaryTexture("resource/downhillduck.jpg", &icons.width, &icons.height, 0, 4);
	icons.pixels = test.m_pixels;
	if (icons.pixels)
	{
		glfwSetWindowIcon(window, 1, &icons);
	}
	else
	{
		std::cout << "Failed to load Window Icon" << std::endl;
	}
	}

	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowIconifyCallback(window, window_iconify_callback);
	glfwSetWindowMaximizeCallback(window, window_maximize_callback);

	// glad: load all OpenGL function pointers
	if (!InitLibGRAPH((Function)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	/*Init Lights*/
	//----------------------------------------------------------------------------------------------
	LowRenderer::LightManager::DirectionalLight({ -0.2f, -1.f, -0.3f }, { 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.f, 1.f, 1.f });
	LowRenderer::LightManager::PointLight({ 0.f, 0.f, 2.f }, { 1.f, 0.f, 0.f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f });
	LowRenderer::LightManager::SpotLight({ 0.5f, -6.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f });

	/*Get the Light to configure*/
	LowRenderer::Light Spotlight = *LowRenderer::LightManager::GetLight(1);
	Spotlight.SetCutOff(12.5f, 17.5);
	Spotlight.SetDirection({ 0.f, 0.f, -1.f });

	/* Set the Light in LightManager */
	LowRenderer::LightManager::SetLight(Spotlight, 1);
	//----------------------------------------------------------------------------------------------



	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//callback for the scroll
	glfwSetScrollCallback(window, scroll_callback);
	//callback for the mouse
	glfwSetCursorPosCallback(window, mouse_callback);
	//callback for the inputs
	glfwSetKeyCallback(window, key_callback);
	/* Loop until the user closes the window */
	Core::Maths::vec3 beforeCamPos{ 0.f, 0.f, 0.f };


											/*SCENE*/
	//----------------------------------------------------------------------------------------------
	g_scene.InitScene();
	//----------------------------------------------------------------------------------------------

	

	while (!glfwWindowShouldClose(window))
	{
		now = (float)glfwGetTime();
		delta = now - lastTime;
		lastTime = now;

		processInput(window);

		/*Poll for and process events*/
		glfwPollEvents();

		///*Wireframe Mode press <F>*/
		WireFrameGFX(wireframe);

		/*Scene Update Functions*/
		g_scene.RenderScene();
		g_scene.UpdateScene(delta);

		
											/*Set Material*/
		//----------------------------------------------------------------------------------------------
		//http://devernay.free.fr/cours/opengl/materials.html /*READ...:)(●'◡'●)*/
		g_scene.m_shader.SetVec3({ 1.f, 1.f, 1.f }, "material.ambient", g_scene.m_shader.GetID());

		g_scene.m_shader.SetVec3({ 1.0f, 0.5f, 0.31f }, "material.diffuse", g_scene.m_shader.GetID());

		g_scene.m_shader.SetVec3({ 0.5f, 0.5f, 0.5f }, "material.specular", g_scene.m_shader.GetID());

		g_scene.m_shader.SetFloat(32.0f, "material.shininess", g_scene.m_shader.GetID());
		//----------------------------------------------------------------------------------------------

		/*SpotLight*/ 
		//-----------------------------------------------------------------------------------------------
		/*flashlight configuration*/
		//Spotlight.SetPosition({ g_scene.m_camera.m_cameraPos.m_x,g_scene.m_camera.m_cameraPos.m_y,g_scene.m_camera.m_cameraPos.m_z, 1.f });
		//Spotlight.SetDirection(g_scene.m_camera.m_cameraFront);

		/*Light Color (Test color variadic)*/
		/*
		Core::Maths::vec3 lightColor{ 0.f, 0.f, 0.f };
		lightColor.m_x = sinf((float)glfwGetTime() * 2.0f);
		lightColor.m_y = sinf((float)glfwGetTime() * 0.7f);
		lightColor.m_z = sinf((float)glfwGetTime() * 1.3f);
		Core::Maths::vec3 diffuseColor = lightColor * Core::Maths::vec3(0.5f, 0.5f, 0.5f); // decrease the influence
		Core::Maths::vec3 ambientColor = diffuseColor * Core::Maths::vec3(0.2f, 0.2f, 0.2f); // low influence
		Spotlight.SetAmbientColor(ambientColor);
		Spotlight.SetDiffuseColor(diffuseColor);
		*/
		/*Static color Light*/
		//Spotlight.SetAmbientColor({ 0.1f, 0.1f, 0.1f });
		//Spotlight.SetDiffuseColor({ 0.8f, 0.8f, 0.8f });

		/* Set the Light in LightManager *///Careful to pass the good index XD
		//LowRenderer::LightManager::SetLight(Spotlight, 1);
		//----------------------------------------------------------------------------------------------

		/*Update light values in Shader*/
		LowRenderer::LightManager::UpdateShader(g_scene.m_shader.GetID());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	_CrtDumpMemoryLeaks();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* , int width, int height)
{
	// Size of Window for the rendering
	ResetViewPort(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		g_scene.m_camera.MoveCamera(LowRenderer::MovCamera::FORWARD, delta);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		g_scene.m_camera.MoveCamera(LowRenderer::MovCamera::BACKWARD, delta);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		g_scene.m_camera.MoveCamera(LowRenderer::MovCamera::LEFT, delta);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		g_scene.m_camera.MoveCamera(LowRenderer::MovCamera::RIGHT, delta);
	}
}

void mouse_callback(GLFWwindow* , double xpos, double ypos)
{
	if (g_active_mouse)
	{
		if (g_firstMouse)
		{
			g_lastX = (float)xpos;
			g_lastY = (float)ypos;
			g_firstMouse = false;
		}

		float xoffset = (float)xpos - g_lastX;
		float yoffset = g_lastY - (float)ypos;

		g_lastX = (float)xpos;
		g_lastY = (float)ypos;

		g_scene.m_camera.MouseMoveCamera(xoffset, yoffset);
	}
}

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	const float cameraSpeed = 0.5f * delta;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (wireframe == true)
		{
			wireframe = false;
		}
		else
		{
			wireframe = true;
		}
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		if (g_active_mouse == true)
		{
			g_active_mouse = false;
		}
		else
		{
			g_active_mouse = true;
		}
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (g_mouse_cursor == true)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			g_mouse_cursor = false;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			g_mouse_cursor = true;
		}
	}
}

void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
	if (g_scene.m_camera.m_fov > 1.0f && g_scene.m_camera.m_fov < 45.0f)
		g_scene.m_camera.m_fov -= (float)yoffset;
	else if (g_scene.m_camera.m_fov <= 1.0f)
		g_scene.m_camera.m_fov = 1.1f;
	else if (g_scene.m_camera.m_fov >= 45.0f)
		g_scene.m_camera.m_fov = 44.9f;
}

void window_iconify_callback(GLFWwindow* window, int iconified)
{
	if (iconified)
	{
		glfwIconifyWindow(window);
	}
	else
	{
		glfwRestoreWindow(window);
	}
}

void window_maximize_callback(GLFWwindow* window, int maximized)
{
	if (maximized)
	{
		glfwMaximizeWindow(window);
	}
	else
	{
		glfwRestoreWindow(window);
	}
}