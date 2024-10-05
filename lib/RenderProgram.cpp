#include "RenderProgram.h"

Shader::Shader(std::string fileName) {
	this->fileName = fileName;

	if (fileName.ends_with(".vert")) {
		shaderType = GL_VERTEX_SHADER;
	}
	else if (fileName.ends_with(".frag")) {
		shaderType = GL_FRAGMENT_SHADER;
	}

	readFile(fileName);
	compile();
	getErrors();
}

void Shader::readFile(std::string fileName) {
	std::ifstream t(fileName);

	if (t.is_open()) {
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		src = str;
	}
	else {
		std::cout << "Unable to open file " << fileName << std::endl;
	}
}

void Shader::clean() {
	glDeleteShader(shaderID);
}

void Shader::compile() {
	shaderID = glCreateShader(shaderType);
	const char* source = src.c_str();
	glShaderSource(shaderID, 1, &source, nullptr);
	glCompileShader(shaderID);
}

void Shader::getErrors() {
	GLint retCode;
	char errorLog[1024];
	GLint fragment_compiled;
	glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &fragment_compiled);
	
	if (fragment_compiled != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(this->shaderID, 1024, &log_length, message);
		std::cout << "ERROR " << fileName << "\n" << message << "\n\n";
	}
}

void Program::addShader(std::string fileName) {
	shaderList[fileName] = new Shader(fileName);
}

void Program::link() {
	programID = glCreateProgram();

	for (auto& s : shaderList) {
		glAttachShader(programID, s.second->shaderID);
	}

	glLinkProgram(programID);
	getErrors();
	readVarList();
}

void Program::clean() {
	for (auto& s : shaderList) {
		glDeleteShader(s.second->shaderID);
	}
}

void Program::getErrors() {
	GLint program_linked;
	glGetProgramiv(this->programID, GL_LINK_STATUS, &program_linked);
	
	if (program_linked != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetProgramInfoLog(this->programID, 1024, &log_length, message);
		std::cout << "ERROR: " << message << "\n\n";
	}
}

void Program::readVarList() {
	int count = 0;
	
	std::map<std::string, std::string> shaderDescriptor = readShaderDescriptor();
	//for (const auto& pair : shaderDescriptor) std::cout << pair.first << " : " << pair.second << std::endl;
	
	glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &count);
	//std::cout << "Active Attributes: " << count << std::endl;
	for (int i = 0; i < count; i++) {
		GLuint idx = i;
		GLsizei buffSize, length;
		GLint size;
		GLenum type;
		char name[30];
		buffSize = 30;
		unsigned int location = 0;

		glGetActiveAttrib(programID, (GLuint) idx, buffSize, &length, &size, &type, name);
		location = glGetAttribLocation(programID, name);
		//std::cout << "Attribute: " << name << std::endl;
       
		for (const auto& pair : shaderDescriptor) {
			if (pair.second == name) {
				//std::cout << "Found: " << pair.first << std::endl;
				strncpy_s(name, sizeof(name), pair.first.c_str(), _TRUNCATE);
				break;
			}
		}
	
		//std::cout << "Attribute: " << name << " Location: " << location << std::endl;
		varList[name] = location;
	}

	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; i++) {
		GLuint idx = i;
		GLsizei buffSize, length;
		GLint size;
		GLenum type;
		char name[30];
		buffSize = 30;
		unsigned int location = 0;

		glGetActiveUniform(programID, (GLuint)idx, buffSize, &length, &size, &type, name);
		location = glGetUniformLocation(programID, name);
		//std::cout << "Uniform: " << name << std::endl;
		
		for (const auto& pair : shaderDescriptor) {
			if (pair.second == name) {
				//std::cout << "Found: " << pair.first << std::endl;
				strncpy_s(name, sizeof(name), pair.first.c_str(), _TRUNCATE);		
				break;
			}
		}
		
		//std::cout << "Uniform: " << name << " Location: " << location << std::endl;
		varList[name] = location;
	}
}

std::map<std::string, std::string> Program::readShaderDescriptor() {
	std::map<std::string, std::string> shader_map;

	std::ifstream f("./data/ShaderDescriptor.txt", std::ios_base::in);
	if (!f.is_open()) {
		std::cout << "Unable to open file ShaderDescriptor.txt" << std::endl;
		return shader_map;
	}

	std::string line;
	while (std::getline(f, line, '\n')) {
		//std::cout << line << std::endl;
		if (line.empty() || line[0] == '#') {
			continue;
		}

		size_t pos = line.find('=');
		if (pos != std::string::npos) {
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			shader_map[key] = value;
		}
	}

	return shader_map;
}

int Program::getAttributeLocation(std::string name) {
    if (varList.find(name) != varList.end()) {
        return varList[name];
    }
    else {
        std::cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << std::endl;
    }
    return 0;
}

int Program::getUniformLocation(std::string name) {
    if (varList.find(name) != varList.end()) {
        return varList[name];
    }
    else {
        std::cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << std::endl;
    }
}

void Program::setMVP(libMath::mat4x4f mvp) {
	if (varList.find("uniformMVP") != varList.end()) {
		glUniformMatrix4fv(varList["uniformMVP"], 1, GL_FALSE, &mvp.data1D[0]);
	} else {
		std::cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << std::endl;
	}
}

void Program::setMatrix(string name, libMath::mat4x4f mvp) {
	if (varList.find(name) != varList.end()) {
		glUniformMatrix4fv(varList[name], 1, GL_FALSE, &mvp.data1D[0]);
	}
	else {
		std::cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << std::endl;
	}
}

void Program::setInteger(string name, int data) {
	if (varList.find(name) != varList.end()) 
		glUniform1i(varList[name], data);
}

void Program::setFloat(string name, float data) {
	if (varList.find(name) != varList.end()) glUniform1f(varList[name], data);
}

void Program::setVec4f(string name, libMath::vector4f data) {
	if (varList.find(name) != varList.end()) glUniform4f(varList[name], data.x, data.y, data.z, data.w);
}

void Program::setVertexAttribute(std::string name, int ncomp, int type, size_t stride, void* offset) {
	//std::cout << name << " " << ncomp << " " << type << " " << stride << " " << std::endl;
	if (varList.find(name) != varList.end()) {
		unsigned int location = varList[name];
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, ncomp, type, GL_FALSE, stride, offset);
	} else {
		std::cout << "ERROR: " << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << std::endl;
	}
}

void Program::use() {
	glUseProgram(programID);
}