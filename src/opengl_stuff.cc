//	Sarvottamananda (shreesh)
//	2020-09-28
//	opengl_stuff.cc v0.0 (Simple OpenGL Code Snippets)
//
//	OpenGL utility functions

#include "opengl_stuff.h"

#include <iostream>

GLenum
check_glerror(const char *file, unsigned int line)
{
    GLenum errorCode = GL_NO_ERROR;

    // clang-format off
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
	std::string error = "unknown_error";
	switch (errorCode) {
	    case GL_INVALID_ENUM:       error = "invalid_enum"; break;
	    case GL_INVALID_VALUE:      error = "invalid_value"; break;
	    case GL_INVALID_OPERATION:  error = "invalid_operation"; break;
	    case GL_STACK_OVERFLOW:     error = "stack_overflow"; break;
	    case GL_STACK_UNDERFLOW:    error = "stack_underflow"; break;
	    case GL_OUT_OF_MEMORY:      error = "out_of_memory"; break;
	    case GL_INVALID_FRAMEBUFFER_OPERATION:  
                                        error = "invalid_framebuffer_operation"; break;
	    case GL_CONTEXT_LOST:       error = "context_lost"; break;
	    default: break;
	}
	std::cerr << "OpenGL error (" << file << ":" << line << ") : " << error << std::endl;
    }
    return errorCode;
    // clang-format on
}
