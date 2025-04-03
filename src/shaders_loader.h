#pragma once
#include <string>
#include <GLES2/gl2.h>    // Äëÿ OpenGL ES 2.0

std::string loadShaderSource(const std::string& path);
GLuint compileShader(GLenum type, const std::string& source);
GLuint createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
