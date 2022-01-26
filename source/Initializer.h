#pragma once
#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H  

#define Initializer InitializationHandler::Instance()

using glm::Vector2;
using glm::Vector3;
using glm::Vector4;
using std::cout;
using std::endl;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character 
{
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

class InitializationHandler final
{
private:
	InitializationHandler() {}
	~InitializationHandler() {}
	InitializationHandler(const InitializationHandler&) = delete;
	InitializationHandler& operator = (const InitializationHandler&) = delete;
public:
	GLFWwindow* window;
	const Vector2 windowSize = Vector2(1600, 800);

	static InitializationHandler& Instance()
	{
		static InitializationHandler object; 
		return object;
	}

	constexpr float GetAspectRatio()
	{
		return windowSize.x / windowSize.y;
	}

	bool Init()
	{
		if (!glfwInit())
		{
			cout << "glfwInit failed" << endl;
			return false;
		}
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		window = glfwCreateWindow(windowSize.x, windowSize.y, "Game", nullptr, nullptr);
		if (!window)
		{
			glfwTerminate();
			cout << "glfwCreateWindow failed" << endl;
			return false;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGL())
		{
			cout << "gladLoadGL failed" << endl;
			return false;
		}

		cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
		cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return -1;
		}

		FT_Face face;
		if (FT_New_Face(ft, "../fonts/montserrat.ttf", 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			return -1;
		}
		else
		{
			// set size to load glyphs as
			FT_Set_Pixel_Sizes(face, 0, 32);

			// disable byte-alignment restriction
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			// load first 128 characters of ASCII set
			for (unsigned char c = 0; c < 128; c++)
			{
				// Load character glyph 
				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
					continue;
				}
				// generate texture
				unsigned int texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				// set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				// now store character for later use
				Character character = 
				{
					texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};
				Characters.insert(std::pair<char, Character>(c, character));
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return true;
	}

	void Quit()	{ glfwTerminate(); }
};