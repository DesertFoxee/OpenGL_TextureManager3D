#include "TextureManager.h"


TextureManager::TextureManager(){
}

TextureManager::TextureManager(TextureSetting _setting)
: setting(_setting){

}

TextureManager::~TextureManager(){
}

GLuint TextureManager::get(const char* name){
	return textures.at(name).id;
}

void TextureManager::erase(const char* name){
	auto find_result = textures.find(name);
	if (find_result != textures.end()){
		glDeleteTextures(0, &find_result->second.id);
	}
}

void TextureManager::loadTexture(const char* name, const char* path_tex){
	Image image;

	// other name is unique 
	if (textures.find(name) != textures.end()){
		cout << "Name : " << name << "texture already exist! ";
		return;
	}

	// gen texture setup texture
	GLuint texture_id = SOIL_load_OGL_texture(path_tex, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (setting.minmapping != NONE_SETTING){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, setting.minmapping);
	}

	if (setting.wrapping_t != NONE_SETTING){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, setting.wrapping_t);
		if (setting.wrapping_s == GL_CLAMP_TO_BORDER){
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, setting.color_border);
		}
	}
	if (setting.wrapping_s != NONE_SETTING){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, setting.wrapping_s);
		if (setting.wrapping_s == GL_CLAMP_TO_BORDER){
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, setting.color_border);
		}
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	// load infor image;
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &(image.width));
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &(image.height));


	// if also anisotropic filtering
	if (setting.anisotropic == true && glewIsSupported("GL_EXT_texture_filter_anisotropic")){
		GLfloat anisoSetting = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
	}

	if (texture_id == 0){
		cout << "Can't load texture file: " << path_tex << endl;
		return;
	}
	image.id = texture_id;

	textures.insert(std::make_pair(name, image));
}

void TextureManager::loadTextureSourceFile(const char* file_path_src){
	ifstream file(file_path_src);
	string stuff , name , path;
	int line = 0;
	if (file.is_open()){
		while (getline(file, stuff, '\n')){
			stuff = removeWhiteSpace(stuff);
			line++;
			auto find_result = stuff.find('\t');
			if (find_result != string::npos){
				name = stuff.substr(0, find_result);
				path = stuff.substr(find_result + 1, stuff.length());
			}
			else{
				cout << "\t>" << line << " : Wrong syntax source file texture" << endl;
			}
		}
		file.close();
	}
	else{
		cout << "Can't open source file texture :" << file_path_src << endl;
		return;
	}
}

string TextureManager::removeWhiteSpace(string input){
	const char* in_str = input.c_str();
	int inputIndex = 0;
	string out_str = "";
	while (in_str[inputIndex] != '\0'){
		out_str += in_str[inputIndex];

		if (in_str[inputIndex] == ' '){
			while (in_str[inputIndex + 1] == ' '){
				inputIndex++;
			}
		}
		inputIndex++;
	}
	if (out_str.length() >= 1){
		if (out_str[0] == ' ')
			out_str.erase(out_str.begin());
	}
	if (out_str.length() >= 1){
		if (out_str[out_str.length() - 1] == ' ')
			out_str.erase(out_str.end() - 1);
	}
	return out_str;
}
