#pragma once

#include "Common.h"
#include "Mat4x4f.h"

using namespace std;

class Shader
{
public:
	std::string fileName;
	std::string src;
	int shaderID;
	unsigned int shaderType;

	Shader(std::string fileName);
	void readFile(std::string fileName);
	void clean();
	void compile();
	void getErrors();
};

class Program
{
public:
	unsigned int programID = -1;
	std::map<std::string, Shader*> shaderList;
	std::map<std::string, unsigned int> varList;
	std::map<std::string, std::string> shaderDescriptor;
   
	Program() {};
	void addShader(std::string fileName);
	void link();
	void clean();
	void getErrors();
	void readVarList();
	
	std::map<std::string, std::string> readShaderDescriptor();

	int getAttributeLocation(std::string name);
	int getUniformLocation(std::string name);

	void setMVP(libMath::mat4x4f mvp);
	void setMatrix(string name, libMath::mat4x4f mvp);
	void setInteger(string name, int data);
	void setFloat(string name, float data);
	void setVec4f(string name, libMath::vector4f data);

	void setVertexAttribute(std::string name, int ncomp, int type, size_t stride, void* offset);
	void use();
};