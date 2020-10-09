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

// Forward declarations, to be defined later, but used before. It is generally good practise to
// declare all static functions that a file defines, just before the function they are needed,
// nothing wrong if we declare them several times, as we can then move around functions, without
// worrying about things breaking.

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

	//
	// II. glew stuff
	//

	// glew is literal glue between opengl (glvnd and mesa) and glfw (glx and xcb), so the
	// others need to be initialized before glew is initialized.

	// all opengl functions need a context, glew needs to understand the context
	glfwMakeContextCurrent(win);

	// initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
	    glfwTerminate();
	    // throw error
	    throw std::runtime_error("Failed to initialize glew.");
	}

	// get version info to see if our hinting was successful
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);

	// what is the opengl driver
	std::cout << "Renderer: " << renderer << std::endl;
	// and the opengl version that it provides
	std::cout << "OpenGL version supported " << version << std::endl;

	//
	// V. Rendering
	//

	// black background, even the alpha is set to 0
	glClearColor(1.0f, 0.5f, 0.0f, 0.0f);

	// Value 0 is for no vsync, and 1 for vsync, it is integral value of required number of
	// display refreshes before we swap. Doesn't matter as we are not drawing realtime.
	glfwSwapInterval(0);

	// render loop
	while (!glfwWindowShouldClose(win)) {
	    // render

	    // foremost we clear the screen, otherwise it is tricky to redraw only the changed
	    // parts of the screen
	    glClear(GL_COLOR_BUFFER_BIT);

	    // swap buffers and poll IO events (keys pressed/released, mouse moved
	    // etc.)
	    glfwSwapBuffers(win);

	    // Either we poll for the events (immediately returns) or we wait for the events
	    // (waits), we are not doing realtime so we wait.

	    glfwWaitEvents();
	    // glfwPollEvents();

	    // process input
	    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);
	}

	// terminate glfw, clearing all previously allocated GLFW resources
	glfwTerminate();

	std::cout << "Window clear : success!!\n";
	return 0;
    }
    catch (std::exception &ex) {
	std::cerr << ex.what() << std::endl;
	return 1;
    }
}
