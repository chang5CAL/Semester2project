#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader
{
    public:
        Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);
        GLuint Program;
        void Use();
    protected:
    private:
};

#endif // SHADER_H
