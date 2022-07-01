#pragma once

#include "GLFW/glfw3.h"
#include "implot.h"
#include "graphics_colour.h"
#include "graphics_utility.h"
#include "math_vector_2d.h"
#include "physics_entity.h"
#include <vector>
#include <memory>
#include <string>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glfw3.lib")

namespace GRAPHICS
{
	std::shared_ptr< PANEL_INTERFACE > main_panel;

	void clear()
	{
		glClearColor(114.0f / 255.0f, 0, 138.0f / 255.0f, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void setup2DView(int windowWidth, int windowHeight)
	{
		glViewport(0, 0, windowWidth, windowHeight);
		clear();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-windowWidth / 2, windowWidth / 2, -windowHeight / 2, windowHeight / 2, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_POINT_SMOOTH);
	}

	void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		main_panel->controls(window, key, scancode, action, mods);
	}


	void mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {
		main_panel->mouse_click_callback(window, button, action, mods);
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		main_panel->mouse_callback(window, xpos, ypos);
	}

	GLFWwindow* create_window(const int resX, const int resY, const std::string& name)
	{
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			return NULL;
		}
		glfwWindowHint(GLFW_SAMPLES, 4);

		GLFWwindow* window = glfwCreateWindow(resX, resY, name.c_str(), NULL, NULL);

		if (window == NULL)
		{
			fprintf(stderr, "Failed to open GLFW window.\n");
			glfwTerminate();
			return NULL;
		}

		glfwMakeContextCurrent(window);

		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

		glfwSetKeyCallback(window, controls);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_click_callback);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		return window;
	}


	GLFWwindow* initWindow(const int resX, const int resY, const std::string& name)
	{
		return GRAPHICS::create_window(resX, resY, name);
	}

	template< class T>
	const std::shared_ptr< PANEL_INTERFACE >& init_main_panel() {
		GRAPHICS::main_panel = std::make_shared<T>();
		GRAPHICS::main_panel->init();

		return GRAPHICS::main_panel;
	}

	int main_graphics(const std::shared_ptr< PANEL_INTERFACE >& main_panel, const int width, const int height, const std::string& name, bool server_based_update = false)
	{
		GLFWwindow* window = GRAPHICS::initWindow(width, height, name);

		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		GRAPHICS::main_panel = main_panel;

		GLint  windowWidth, windowHeight;

		glfwMakeContextCurrent(window);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL2_Init();

		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			ImGui_ImplOpenGL2_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);

			if (!server_based_update) {
				GRAPHICS::main_panel->update();
			}

			glfwGetWindowSize(window, &windowWidth, &windowHeight);

			GRAPHICS::setup2DView(windowWidth, windowHeight);

			GRAPHICS::main_panel->draw();

			ImGui::Render();
			ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
			glfwMakeContextCurrent(window);
			glfwSwapBuffers(window);
		}

		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
		return 0;
	}
};
