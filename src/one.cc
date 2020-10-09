// Include standard headers
//#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

// Include GLEW
// OpenGL Extension Wrangler
// clang-format off
#include <GL/glew.h>
// clang-format on

// Include GLFW
#include <GLFW/glfw3.h>


static void framebuffer_size_callback(GLFWwindow * window, int width, int height);

int main(int argc, char *argv[]) 
{
    // settings
    const unsigned int win_width = 800;
    const unsigned int win_height = 600;
    const std::string win_title = "Simple OpenGL Code Snippet";

    const int major_version = 3;
    const int minor_version = 2;

    try {
	// I. glfw stuff

	// initialize and configure glfw
	// ------------------------------
	if (!glfwInit()) {
	    throw std::runtime_error("Failed to initialize glfw.");
	}
	// require multisampling anti-aliasing (MSAA) 4x
	glfwWindowHint(GLFW_SAMPLES, 4);

	// require opengl version 3.2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);

	// require opengl core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// glfw window creation
	// --------------------
	GLFWwindow *window =
	    glfwCreateWindow(win_width, win_height, win_title.c_str(), NULL, NULL);
	if (!window) {
	    glfwTerminate();
	    throw std::runtime_error("Failed to create glfw window.");
	}

        // all opengl functions need a context
	glfwMakeContextCurrent(window);

        // callback uses glViewport, so it can only by set after the context has
        // been created
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// II. glew stuff

	// initialize glew
        glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
	    glfwTerminate();
	    throw std::runtime_error("Failed to initialize glew.");
	}

	// get version info
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);

        std::cout << "Renderer: " << renderer << std::endl;
        std::cout << "OpenGL version supported " << version << std::endl;


        //
        // III. shader stuff
        //

	// build and compile our shader program
	// ------------------------------------

	// GLSL language

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

	const char *fragment_shader_src =
	    "#version 330 core\n"
	    "in vec4 fCol;\n"
	    "out vec4 FragColor;\n"
	    "void main()\n"
	    "{\n"
	    "   FragColor = vec4(fCol);\n"
	    "}\n\0";

	// vertex shader
	GLint result = 1;

	int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if (!vertex_shader) {
            throw std::runtime_error("creation of vertex shader object failed.");
	}

	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);

	// check for shader compile errors
        
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
            GLsizei log_sz = 0;
            glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_sz);

	    char *info_log = new char[log_sz + 1];

	    glGetShaderInfoLog(vertex_shader, log_sz, &log_sz, info_log);
            std::cerr << "shader compiler :\n" << info_log << std::endl;
	    throw std::runtime_error("vertex shader compilation failed.");
	}

	// fragment shader
	int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragment_shader) {
            throw std::runtime_error("creation of fragment shader object failed.");
	}
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);

	// check for shader compile errors
        
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
            GLsizei log_sz = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_sz);

	    char *info_log = new char[log_sz + 1];

	    glGetShaderInfoLog(fragment_shader, log_sz, &log_sz, info_log);
            std::cerr << "shader compiler :\n" << info_log << std::endl;
	    throw std::runtime_error("fragment shader compilation failed.");
	}

	// link shaders
	int shader_program = glCreateProgram();
	if (!shader_program) {
	    throw std::runtime_error("creation  of shader program object failed");
        }

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);

	// check for linking errors
	glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
            GLsizei log_sz = 0;
            glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_sz);

	    char *info_log = new char[log_sz + 1];

	    glGetShaderInfoLog(shader_program, log_sz, &log_sz, info_log);
            std::cerr << "shader compiler :\n" << info_log << std::endl;
	    throw std::runtime_error("shader program linking failed.");
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);


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

	    0.0f, 0.0f, 0.0f,	 // left
	    0.5f, 0.0f, 0.0f,	 // dark orange
	    1.0f, 0.0f, 0.0f,	 // right
	    0.5f, 0.0f, 0.0f,	 // dark orange
	    0.0f, 1.0f, 0.0f,	 // top
	    1.0f, 0.0f, 0.0f,	 // orange

	    // second triangle

	    0.0f, 0.0f, 0.0f,	 // left
	    0.25f, 0.0f, 0.4f,	// dark purple
	    0.0f, -1.0f, 0.0f,	 // bottom
	    0.25f, 0.0f, 0.4f,	// dark purple
	    1.0f, 0.0f, 0.0f,	 // right
	    0.5f, 0.0f, 0.8f,	// purple

	    // third triangle

	    0.0f, 0.0f, 0.0f,	 // right
	    0.25f, 0.45f, 0.25f,  // dark gray
	    -1.0f, 0.0f, 0.0f,	 // left
	    0.25f, 0.45f, 0.25f,  // dark gray
	    0.0f, -1.0f, 0.0f,	 // bottom
	    0.4f, 0.9f, 0.4f,	  // gray

	    // fourth triangle

	    0.0f, 0.0f, 0.0f,	 // left
	    0.0f, 0.25f, 0.4f,	// dark purple
	    0.0f, 1.0f, 0.0f,	 // bottom
	    0.0f, 0.25f, 0.4f,	// dark purple
	    -1.0f, 0.0f, 0.0f,	 // right
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
	glClearColor(0.0f, 0.0f, 0.07f, 0.0f);

	// no vsync, 1 for vsync
	glfwSwapInterval(0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
	    // input
	    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	    // render
	    glClear(GL_COLOR_BUFFER_BIT);

	    // seeing as we only have a single VAO there's no need to bind it every
	    // time, but we'll do so to keep things a bit more organized

	    // set the count to 6 since we're drawing 9 vertices now (3 triangles);
	    // not 3!

	    glBindVertexArray(vao);

	    // draw our triangles
	    glUseProgram(shader_program);

	    glDrawArrays(GL_TRIANGLES, 0, num_triangles * 3);

            // no need to unuse program everytime
	    // glUseProgram(0);
            
	    // no need to unbind it every time
	    // glBindVertexArray(0);

	    // swap buffers and poll IO events (keys pressed/released, mouse moved
	    // etc.)
	    glfwSwapBuffers(window);

            // Either we poll for the events  or we wait for the events

	    // glfwPollEvents();
            glfwWaitEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shader_program);

	// terminate glfw, clearing all previously allocated GLFW resources.s
	glfwTerminate();
	return 0;
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}

// whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) 
{
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}
