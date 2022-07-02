// -- LOCAL

// .. REFERENCES

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "main_panel.h"
#include "graphics_helper.h"
#include <stdio.h>
#include <iostream>


#pragma comment (lib, "PhysBox2D.lib")

int main(int argc, char** argv) {

	auto& main_panel = GRAPHICS::init_main_panel<MAIN_PANEL>();

	return GRAPHICS::main_graphics(main_panel, (int)g_panel_width, (int)g_panel_height, "A* Search", false);

}



