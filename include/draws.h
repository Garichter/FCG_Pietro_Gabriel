#ifndef _DRAWS_H
#define _DRAWS_H

#include <cmath>

#include <glad/glad.h>   // Cria��o de contexto OpenGL 3.3
#include <GLFW/glfw3.h>  // Cria��o de janelas do sistema operacional
#include <glm/gtc/type_ptr.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <matrices.h>

void desenhaMapa(GLint);
void inicializaMapa();
GLuint BuildTrack();
GLuint BuildFloor();
GLuint BuildCube();
GLuint BuildTrackCircle();

#endif // _DRAWS_H
