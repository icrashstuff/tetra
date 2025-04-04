/* SPDX-License-Identifier: MIT
 *
 * SPDX-FileCopyrightText: Copyright (c) 2025 Ian Hangartner <icrashstuff at outlook dot com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Basic example program:
 * main.cpp
 * ========
 * #include "tetra/gui/imgui.h"
 * #include "tetra/tetra_core.h"
 * #include "tetra/tetra_gl.h"
 * int main(const int argc, const char** argv)
 * {
 *     tetra::init("icrashstuff", "Tetra example", "config_prefix", argc, argv);
 *
 *     tetra::set_render_api(tetra::RENDER_API_GL_CORE, 3, 0);
 *
 *     tetra::init_gui("Hello World");
 *
 *     int done = 0;
 *     while (!done)
 *     {
 *         done = !tetra::start_frame();
 *
 *         ImGui::Begin("Hello");
 *         ImGui::Text("Hello world from tetra!");
 *         ImGui::End();
 *
 *         tetra::end_frame();
 *     }
 *
 *     tetra::deinit_gui();
 *     tetra::deinit();
 * }
 *
 * CMakeLists.txt
 * ==============
 * cmake_minimum_required(VERSION 3.27)
 *
 * # Make option() honor normal variables
 * cmake_policy(SET CMP0077 NEW)
 *
 * project(tetra_example LANGUAGES CXX C)
 *
 * set(CMAKE_INCLUDE_CURRENT_DIR ON)
 * set(tetra_example_SRC
 *     main.cpp
 * )
 *
 * set(TETRA_BUILD_GL_COMPONENT ON)
 *
 * # Tetra must be in the "tetra" subfolder
 * add_subdirectory(tetra/)
 *
 * add_executable(tetra_example ${tetra_example_SRC})
 *
 * target_link_libraries(tetra_example tetra::core)
 * target_link_libraries(tetra_example tetra::gl)
 */

#ifndef TETRA_TETRA_GL_H_INCLUDED
#define TETRA_TETRA_GL_H_INCLUDED

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

namespace tetra
{

enum render_api_t
{
    RENDER_API_GL_CORE,
    RENDER_API_GL_COMPATIBILITY,
    RENDER_API_GL_ES,
};

/**
 * Set render api and version for tetra to use
 *
 * Must be called before tetra::init_gui()
 *
 * NOTE: No checks are made for invalid variables
 */
void set_render_api(render_api_t api, int major, int minor);

/**
 * Returns 0 on successful init, some non-zero value on failure
 */
int init_gui(const char* window_title);

/**
 * Deinit gui, call this before tetra::deinit()
 */
void deinit_gui();

/**
 * Returns -1 on failure, 0 for application should exit, 1 for application should continue
 *
 * @param event_loop Handle SDL events inside start_frame()
 */
int start_frame(bool event_loop = true);

/**
 * Feed events to imgui
 *
 * Returns true if application should exit, false otherwise
 */
bool process_event(SDL_Event event);

/**
 * Change visibility of main imgui context
 *
 * This works by not feeding the context any events and discarding all render data
 *
 * NOTE: If the dev console is shown it will take priority over values set through here
 * NOTE: gui_registrar::render_menus() is still called
 */
void show_imgui_ctx_main(bool shown);

/**
 * @returns True if either the main imgui context is shown or the dev console is forcing it, False otherwise
 */
bool imgui_ctx_main_wants_input();

/**
 * Change visibility of overlay imgui context
 *
 * This works by discarding all render data
 *
 * NOTE: gui_registrar::render_overlays() is still called
 */
void show_imgui_ctx_overlay(bool shown);

/**
 * Renders the frame, and optionally limits the frame rate if gui_fps_limiter is set
 *
 * @param clear_frame Clear OpenGL color buffer
 * @param cb_screenshot Callback to be called immediately before SDL_GL_SwapWindow()
 */
void end_frame(bool clear_frame = true, void (*cb_screenshot)(void) = NULL);

/**
 * Wrapper around glObjectLabel()
 *
 * NOTE: If the OpenGL context version is below 4.3 or r_debug_gl is not set then this function is a no-op
 *
 * NOTE: This is only valid for an OpenGL context created by tetra::init_gui()
 */
void gl_obj_label(GLenum identifier, GLuint name, const GLchar* fmt, ...) SDL_PRINTF_VARARG_FUNCV(3);

/**
 * Window created by tetra::init_gui()
 */
extern SDL_Window* window;

/**
 * OpenGL Context created by tetra::init_gui()
 */
extern SDL_GLContext gl_context;
}

#endif
