#pragma once

#include <iostream>
#include <map>
#include "Component.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

class TextHandler final
{
public:
	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character
	{
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2 Size;      // Size of glyph
		glm::ivec2 Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};

	static std::map<char, Character> characters;

	static int InitializeCharacters();
};

class TextRendererComponent : public Component
{
	COMPONENT(TextRendererComponent)
private:
public:
};