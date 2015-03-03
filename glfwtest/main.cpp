#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//Something broke in Glew, apparently, so now it won't let me compile.

// GLFW
#include <GL/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

float red = 1.0f;
float blue = 1.0f;
float green = 1.0f;
//Globals for screen colors

float vertex1x = -0.5f;
float vertex1y = -0.5f;
float vertex1z = 0.0f;
float vertex2x = 0.5f;
float vertex2y = -0.5f;
float vertex2z = 0.0f;
float vertex3x = 0.0f;
float vertex3y = 0.5f;
float vertex3z = 0.0f;
//Global points for the triangle. Is here so points can be modified.

//Press QWE to raise the color and ASD to lower it.
//Press RTY to raise the X of the triangle and FGH to lower it.
//Press UIO to raise the Y of the triangle and JKL to lower it.
//Press ZXC to raise the Z of the triangle and VBN to lower it.
//Note: none of the triangle adjusts have been implemented.

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

int main()
{
    float vertices[8];
    //Array to draw the vertexes.
    GLuint VBO;
    GLuint vertexShader;
    GLint success;
    GLchar infolog[512];

    glfwInit();
    glewInit();
    //Initiates the libraries

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    //Sets OpenGL Version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Ensures OpenGL will only run using new functionality.
    glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
    //Sets if the window is resizeable.

    GLFWwindow* window = glfwCreateWindow(800,600,"Window Test", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (window == NULL)
    {
        std::cout << "Failed to create window.";
        glfwTerminate();
        return -1;
    }
    //Checks if Window is successfully created
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initiate GLEW.";
        return -1;
    }
    //Checks if GLEW successfully initiated.
    glViewport(0,0,800,600);
    //Allows OpenGL to know the initial coordinate and the size of the window.

    glfwSetKeyCallback(window,key_callback);
    //Sets what function manages key presses.

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        std::cout << "Shader failed to compile\n" << infolog << std::endl;
    }


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(red,green,blue,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //Clears the screen

        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window,GL_TRUE);
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        if (red < 1)
        {
            red += .1f;
        }
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        if (green < 1)
        {
            green += .1f;
        }
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        if (blue < 1)
        {
            blue += .1f;
        }
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        if (red > 0)
        {
            red -= .1f;
        }
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        if (green > 0)
        {
            green -= .1f;
        }
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        if (blue > 0)
        {
            blue -= .1f;
        }
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        if (vertex1x < 1.0f)
        {
            vertex1x += .1f;
        }
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        if (vertex2x < 1.0f)
        {
            vertex2x += .1f;
        }
    }
    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        if (vertex3x < 1.0f)
        {
            vertex3x += .1f;
        }
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (vertex1x > -1.0f)
        {
            vertex1x -= .1f;
        }
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        if (vertex2x > -1.0f)
        {
            vertex2x -= .1f;
        }
    }
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        if (vertex3x > -1.0f)
        {
            vertex3x -= .1f;
        }
    }
    if (key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        if (vertex1y < 1.0f)
        {
            vertex1y += .1f;
        }
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        if (vertex2y < 1.0f)
        {
            vertex2y += .1f;
        }
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        if (vertex3y < 1.0f)
        {
            vertex3y += .1f;
        }
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        if (vertex1y > -1.0f)
        {
            vertex1y -= .1f;
        }
    }
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        if (vertex2y > -1.0f)
        {
            vertex2y -= .1f;
        }
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (vertex3y > -1.0f)
        {
            vertex3y -= .1f;
        }
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        if (vertex1z < 1.0f)
        {
            vertex1z += .1f;
        }
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        if (vertex2z < 1.0f)
        {
            vertex2z += .1f;
        }
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        if (vertex3z < 1.0f)
        {
            vertex3z += .1f;
        }
    }
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        if (vertex1z > -1.0f)
        {
            vertex1z -= .1f;
        }
    }
    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        if (vertex2z > -1.0f)
        {
            vertex2z -= .1f;
        }
    }
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        if (vertex3z > -1.0f)
        {
            vertex3z -= .1f;
        }
    }
}
