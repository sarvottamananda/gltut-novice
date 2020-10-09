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
    try {
	//
	// I. glfw stuff
	//

	// initialize and configure glfw
	if (!glfwInit()) {
	    throw std::runtime_error("Failed to initialize glfw.");
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
