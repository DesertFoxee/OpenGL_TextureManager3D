#ifndef TEXTUREMANAGER_H

#define TEXTUREMANAGER_H


#include <iostream>
#include <map>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>


#define NONE_SETTING -1

using namespace std;

/* 
	- s and t is coordinates texture from the lowerleft 
	- do without remove texture id destructor
*/
struct Image
{
	GLuint id;
	GLfloat width;
	GLfloat height;
};

struct TextureSetting
{
	
	GLint minmapping = GL_LINEAR_MIPMAP_LINEAR;
	GLint wrapping_s = NONE_SETTING;
	GLint wrapping_t = NONE_SETTING;
	bool anisotropic = true;
	GLfloat color_border[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
};

class TextureManager
{

private:
	TextureSetting setting;
	map<const char*, Image> textures;

public:
	TextureManager();
	TextureManager(TextureSetting _setting);
	~TextureManager();

	GLuint get(const char* name);

	void erase(const char* name);

	void loadTexture(const char* name, const char* path_tex);

	void loadTextureSourceFile(const char* file_path_src);

public:
	string removeWhiteSpace(string input);
};

#endif // !TEXTUREMANAGER_H



