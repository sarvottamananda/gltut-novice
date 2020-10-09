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
	// III. shader stuff
	//

	// build and compile our shader program

	// Basically there are two steps in building programs, 1) we compile all the program
	// individually using glCompileShader into shader objects and 2) we link all the related
	// compiled shader objects in a program object.

	// Our beginner opengl program uses no transformations, so  we pass on the
	// coordinates and the colour of the vertices to the rasterizer, both of which we need
	// to specify to draw the triangles. The fragment shader then takes the colour that it
	// gets from the rasterizer and passes it on to the display hardware.

	// glsl language program for vertex shader

	// pass the input from app to rasterizer
	const char *vertex_shader_src =
	    "#version 330 core\n"
	    "layout (location = 0) in vec3 vPos;\n"
	    "layout (location = 1) in vec3 vCol;\n"
	    "out vec4 fCol;\n"
	    "void main()\n"
	    "{\n"
	    "   gl_Position = vec4(vPos.x, vPos.y, vPos.z, 1.0);\n"
	    "   fCol = vec4(vCol.r, vCol.g, vCol.b, 1.0);\n"
	    "}\0";

	// glsl language program for fragment shader

	// pass the input from rasterizer to display
	const char *fragment_shader_src =
	    "#version 330 core\n"
	    "in vec4 fCol;\n"
	    "out vec4 FragColor;\n"
	    "void main()\n"
	    "{\n"
	    "   FragColor = vec4(fCol);\n"
	    "}\n\0";

	// result of various compilaltion steps
	GLint result = 1;

	// vertex shader compilation
	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	// check if shader object is created
	if (!vertex_shader) {
	    // throw error
	    throw std::runtime_error("creation of vertex shader object failed.");
	}

	// specify the shader source and compile
	glShaderSource(vertex_shader, 1, &vertex_shader_src, nullptr);
	glCompileShader(vertex_shader);

	// check for shader compile errors
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
	    // get compile log length
	    GLsizei log_sz = 0;
	    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_sz);

	    // get compile log
	    char *info_log = new char[log_sz + 1];
	    glGetShaderInfoLog(vertex_shader, log_sz, &log_sz, info_log);
	    std::cerr << "shader compiler :\n" << info_log << std::endl;

	    // throw error
	    throw std::runtime_error("vertex shader compilation failed.");
	}

	// fragment shader
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragment_shader) {
	    // throw error
	    throw std::runtime_error("creation of fragment shader object failed.");
	}
	glShaderSource(fragment_shader, 1, &fragment_shader_src, nullptr);
	glCompileShader(fragment_shader);

	// check for shader compile errors

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
	    // get compile log length
	    GLsizei log_sz = 0;
	    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_sz);

	    // get compile log
	    char *info_log = new char[log_sz + 1];
	    glGetShaderInfoLog(fragment_shader, log_sz, &log_sz, info_log);
	    std::cerr << "shader compiler :\n" << info_log << std::endl;

	    // throw error
	    throw std::runtime_error("fragment shader compilation failed.");
	}

	// link shaders
	int shader_program = glCreateProgram();
	if (!shader_program) {
	    // throw error
	    throw std::runtime_error("creation  of shader program object failed");
	}

	// specify the shader objects
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	// link the shader objects
	glLinkProgram(shader_program);

	// detach and delete shader objects
	glDetachShader(shader_program, vertex_shader);
	glDetachShader(shader_program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// check for linking errors
	glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
	    // get link log length
	    GLsizei log_sz = 0;
	    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_sz);

	    // get link log
	    char *info_log = new char[log_sz + 1];
	    glGetShaderInfoLog(shader_program, log_sz, &log_sz, info_log);
	    std::cerr << "shader compiler :\n" << info_log << std::endl;

	    // throw error
	    throw std::runtime_error("shader program linking failed.");
	}

	//
	// IV. Data to be drawn
	//

	// set up vertex data (and buffer(s)) and configure vertex attributes

	// add a new set of vertices to form a second triangle (a total of 6
	// vertices); the vertex attribute configuration remains the same (still one
	// 3-float position vector per vertex)

	// hack: position and color are different types, we should use structs/class instead,
	// though it works because underlying types are same

	const int num_triangles = 4;

	GLfloat vertices[] = {

	    // first triangle

	    0.0f, 0.0f, 0.0f,  // left
	    0.5f, 0.0f, 0.0f,  // dark orange
	    1.0f, 0.0f, 0.0f,  // right
	    0.5f, 0.0f, 0.0f,  // dark orange
	    0.0f, 1.0f, 0.0f,  // top
	    1.0f, 0.0f, 0.0f,  // orange

	    // second triangle

	    0.0f, 0.0f, 0.0f,	// left
	    0.25f, 0.0f, 0.4f,	// dark purple
	    0.0f, -1.0f, 0.0f,	// bottom
	    0.25f, 0.0f, 0.4f,	// dark purple
	    1.0f, 0.0f, 0.0f,	// right
	    0.5f, 0.0f, 0.8f,	// purple

	    // third triangle

	    0.0f, 0.0f, 0.0f,	  // right
	    0.25f, 0.45f, 0.25f,  // dark gray
	    -1.0f, 0.0f, 0.0f,	  // left
	    0.25f, 0.45f, 0.25f,  // dark gray
	    0.0f, -1.0f, 0.0f,	  // bottom
	    0.4f, 0.9f, 0.4f,	  // gray

	    // fourth triangle

	    0.0f, 0.0f, 0.0f,	// left
	    0.0f, 0.25f, 0.4f,	// dark purple
	    0.0f, 1.0f, 0.0f,	// bottom
	    0.0f, 0.25f, 0.4f,	// dark purple
	    -1.0f, 0.0f, 0.0f,	// right
	    0.0f, 0.5f, 0.8f,	// purple
	};

	GLuint vbo, vao;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s),
	// and then configure vertex attributes(s).
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			      (void *)(0 * sizeof(GLfloat)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			      (void *)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered
	// VBO as the vertex attribute's bound vertex buffer object so afterwards we
	// can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally
	// modify this VAO, but this rarely happens. Modifying other

	// VAOs requires a call to glBindVertexArray anyways so we generally don't
	// unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//
	// V. Rendering
	//

	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// sap green background
	glClearColor(0.2f, 0.1f, 0.0f, 0.0f);

	// Value 0 is for no vsync, and 1 for vsync, it is integral value of required number of
	// display refreshes before we swap. Doesn't matter as we are not drawing realtime.
	glfwSwapInterval(0);

	// render loop
	while (!glfwWindowShouldClose(win)) {
	    // render

	    // foremost we clear the screen, otherwise it is tricky to redraw only the changed
	    // parts of the screen
	    glClear(GL_COLOR_BUFFER_BIT);

	    // specify the program to draw the triangle
	    glUseProgram(shader_program);

	    // seeing as we only have a single VAO (vertex array object) there's no need to bind
	    // it every time, but we'll do so to keep things a bit more organized
	    glBindVertexArray(vao);

	    // draw our triangles

	    // set the count to 12 since we're drawing 12 vertices now (4 triangles);
	    // not 4! it reads that array contains triangles and 12 vertices starting from 0
	    glDrawArrays(GL_TRIANGLES, 0, num_triangles * 3);

	    // no need to unuse program everytime
	    // glUseProgram(0);

	    // no need to unbind it every time
	    // glBindVertexArray(0);

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

	// good practice: de-allocate all resources once they've outlived their purposei,
	// shaders are deleted beforehand
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shader_program);

	// terminate glfw, clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
    }
    catch (std::exception &ex) {
	std::cerr << ex.what() << std::endl;
	return 1;
    }
}
