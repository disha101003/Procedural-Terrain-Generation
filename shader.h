#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<sstream>
#include<iostream>
#include<cerrno>
#include<string>
#include<fstream>

#include<glad/glad.h>


std::string get_file_contents(const char* filename);

class Shader{
public:
	GLuint ID;
	Shader(const char* vertexFile2, const char* fragmentFile2);
	void Activate();
	void Delete();
};
#endif
