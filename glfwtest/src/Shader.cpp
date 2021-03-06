#include "Shader.h"

Shader::Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath)
{
    //ctor
    std::string vertexCode;
    std::string fragmentCode;

    try
    {
        std::ifstream vShaderFile(vertexSourcePath);
        std::ifstream fShaderFile(fragmentSourcePath);
        std::stringstream vShaderStream,fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::exception e)
    {
        std::cout << "Error, shader file unsuccessfully read." << std::endl;
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    GLuint vertex,fragment;
    GLint success;
    GLchar infolog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1,&vShaderCode,NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex,512,NULL,infolog);
        std::cout << "Error: Vertex Shader compilation failed \n" << infolog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1,&fShaderCode,NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment,512,NULL,infolog);
        std::cout << "Error: Fragment Shader compilation failed \n" << infolog << std::endl;
    }

    this->Program = glCreateProgram();
    glAttachShader(this->Program,vertex);
    glAttachShader(this->Program,fragment);
    glLinkProgram(this->Program);

    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->Program, 512, NULL, infolog);
        std::cout << "Error: Shader program could not link. \n" << infolog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
{
    glUseProgram(this->Program);
}
