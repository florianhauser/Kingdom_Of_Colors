#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

#include <ft2build.h>
#include <opencv2/core/mat.hpp>
#include <algorithm>
#include "Structs.h"
#include FT_FREETYPE_H
using namespace cv;

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};


class FontUtilities
{
private: 

	static GLuint shader;
	static FT_Library ft;
	static FT_Face face;
	static std::map<GLchar, Character> Characters;
	static GLuint buffer;


public:

	static GLuint CompileShaders( const char* vs_path, const char* fs_path);


	static void init(int width, int height);
	static void render_text(std::string text, GLfloat xOffset, GLfloat yOffset, GLfloat scale, color color);

};

