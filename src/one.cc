/*
 * Sarvottamananda (shreesh)
 * 2020-10-09
 * orig.cc v0.0 (OpenGL Code Snippets)
 *
 * Simple OpenGL code snippet using GLEW, GLFW3 and glm
 *
 * comments style : short comments are lowercase, long comments have semi-proper grammar.
 */

// include glew.h before glfw.h, as glfw will know that it has to prepare for glew (and not
// vulkan)

// opengl extension wrangler : for opengl functions
// clang-format off
#include <GL/glew.h>
// clang-format on

// graphics library framework : for window functions
#include <GLFW/glfw3.h>

// C++ standard headers
#include <iostream>
#include <stdexcept>
#include <string>

/*
 * main() : This is a beginner's snippet, so in order to highlight important parts of the code,
 * we write everything in one behemoth main function.
 */

int
main(int argc, char *argv[])
{
    // settings for the window that will provide us with an opengl context

    // requested width, mostly honoured, unless too big or too small
    const unsigned int width = 800;
    // requested height, mostly honoured, unless too big or too small
    const unsigned int height = 600;
    // requested title, mostly honoured, unless too big
    const std::string title = "Simple OpenGL Code Snippet";

    // Settings for opengl version, since we are not using any new opengl functions we settle
    // for bare minimum with good shader support. I did not check, but we must be using some 3.2
    // feature, as window  is not created if use opengl 3.1, probably we can create opengl 3.1
    // window if we use compatibility mode.

    // requebst opengl 3.2, honoured only if device supports it, we need to check after creating
    // window, what we have got
    const int major_version = 3;
    const int minor_version = 2;

    try {
	//
	// I. glfw stuff
	//

	// initialize and configure glfw
	if (!glfwInit()) {
	    throw std::runtime_error("Failed to initialize glfw.");
	}

	// Now we specify what kind of window we want

	// require opengl version 3.2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);

	// Warning: this may make our programs stop running in future, if some function that we
	// are using is deprecated. Usual practise is to develop programs with strict
	// conditions, but when we give the program to others we use compatibility profile and
	// allow deprecated features by requesting
	// GLFW_OPENGL_PROFILE:GLFW_OPENGL_COMPATIBILITY_PROFILE and
	// GLFW_OPENGL_FORWARD_COMPAT:GL_FALSE

	// require opengl core profile, not the compatibility profile, will disable older
	// versions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// will disable deprecated features in the requested core profile, that are not in the
	// newer versions
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// require multisampling anti-aliasing (MSAA) 4x, otherwise we have jagged edges
	// glfwWindowHint(GLFW_SAMPLES, 4);

	// glfw window creation
	GLFWwindow *win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!win) {
	    glfwTerminate();
	    // throw error
	    throw std::runtime_error("Failed to create glfw window.");
	}
	else {
	    std::cout << "Window creation : success !!\n";
	}

	// terminate glfw, clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
    }
    catch (std::exception &ex) {
	std::cerr << ex.what() << std::endl;
	return 1;
    }
}
