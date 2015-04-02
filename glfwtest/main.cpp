#include <iostream>
#include <math.h>
//Imports math for formula reasons

// GLEW
//#define GLEW_STATIC
//Removed because already defined in compiler settings.
#include <GL/glew.h>

// GLFW
#include <GL/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

float red = 1.0f;
float blue = 1.0f;
float green = 1.0f;
//Globals for screen colors

GLfloat vertex1x = -0.5f;
GLfloat vertex1y = -0.5f;
GLfloat vertex1z = 0.0f;
GLfloat vertex2x = 0.5f;
GLfloat vertex2y = -0.5f;
GLfloat vertex2z = 0.0f;
GLfloat vertex3x = 0.0f;
GLfloat vertex3y = 0.5f;
GLfloat vertex3z = 0.0f;
//Global points for the triangle. Is here so points can be modified.

GLfloat timeValue = glfwGetTime();

GLfloat vertices[] =
{
    vertex1x,vertex1y,vertex1z, 1.0f, 0.0f, 0.0f,
    vertex2x,vertex2y,vertex2z, 0.0f, 1.0f, 0.0f,
    vertex3x,vertex3y,vertex3z, 0.0f, 0.0f, 1.0f
};

GLuint indices[] =
{
    0,1,2,
    0,1,3
};


//Press QWE to raise the color and ASD to lower it.
//Press RTY to raise the X of the triangle and FGH to lower it.
//Press UIO to raise the Y of the triangle and JKL to lower it.
//Press ZXC to raise the Z of the triangle and VBN to lower it.
//Note: none of the triangle adjusts have been implemented.

/*Disabled to test external shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(position,1.0);\n"
"   vertexColor = color;\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 330 core \n"
"in vec3 vertexColor;\n"
"out vec4 exportColor;\n"
"uniform vec3 importColor;\n"
"void main()\n"
"{\n"
"   exportColor = vec4(vertexColor,1.0f);\n"
"}\0";
*/

int main()
{
    //Array to draw the vertexes.
    GLuint VBO;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLuint VAO;
    GLuint EBO;
    GLfloat timeValue,greenValue,redValue,blueValue;
    GLint vertexColorLocation;

    GLint success;
    GLchar infolog[512];

    glfwInit();
    glewInit();
    //Initiates the libraries
    glewExperimental = GL_TRUE;
    //So turns out this is absolutely neccessary and the program would crash without it

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    //Sets OpenGL Version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Ensures OpenGL will only run using new functionality.
    glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
    //Sets if the window is resizeable.
    glViewport(0,0,800,600);
    //Allows OpenGL to know the initial coordinate and the size of the window.

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

    glfwSetKeyCallback(window,key_callback);
    //Sets what function manages key presses.

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();

    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glShaderSource(fragmentShader, 1,&fragmentShaderSource,NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);


    //Tests the shader and shader program to see if they work.
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        std::cout << "Vertex Shader failed to compile\n" << infolog << std::endl;
    }

    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,infolog);
        std::cout << "Shader program to compile\n" << infolog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //Deletes these shaders, since they are now unneccessary.

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices, GL_STATIC_DRAW);

    //Position
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    //Color
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);


    glBindVertexArray(0);

    /////////////////////////////////////////////////////////

    /*
    glUseProgram(shaderProgram);
    //Uses the Shader Program

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);

    //You know, this is like the same thing as above.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,3);
    glBindVertexArray(0);
    */

    /*timeValue = glfwGetTime();
    greenValue = (sin(timeValue)/2+.5);
    redValue = (cos(timeValue)/2+.5);
    blueValue = (tan(timeValue)/2+.5);
    vertexColorLocation = glGetUniformLocation(shaderProgram,"importColor");
    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation,redValue,greenValue,blueValue,1.0f);*/


    while (!glfwWindowShouldClose(window))
    {

        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //Sets the polygons as unfilled

        //glBindVertexArray(VAO);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

        glfwPollEvents();

        glClearColor(red,green,blue,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //Clears the screen

        //Draws the triangle
        glUseProgram(shaderProgram);

        timeValue = glfwGetTime();
        greenValue = (sin(timeValue)/2+.5);
        redValue = (cos(timeValue)/2+.5);
        blueValue = (sin(timeValue)/2+.5);
        vertexColorLocation = glGetUniformLocation(shaderProgram,"importColor");
        glUseProgram(shaderProgram);
        glUniform3f(vertexColorLocation,redValue,greenValue,blueValue);

        vertices[3] = redValue;
        vertices[11] = greenValue;
        vertices[17] = blueValue;

        //Color
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);

    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);

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
        if (vertices[0] < 1.0f)
        {
            vertices[0] += .1f;
        }
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        if (vertices[3] < 1.0f)
        {
            vertices[3] += .1f;
        }
    }
    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        if (vertices[6] < 1.0f)
        {
            vertices[6] += .1f;
        }
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (vertices[0] > -1.0f)
        {
            vertices[0] -= .1f;
        }
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        if (vertices[3] > -1.0f)
        {
            vertices[3] -= .1f;
        }
    }
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        if (vertices[6] > -1.0f)
        {
            vertices[6] -= .1f;
        }
    }
    if (key == GLFW_KEY_U && action == GLFW_PRESS)
    {
        if (vertices[1] < 1.0f)
        {
            vertices[1] += .1f;
        }
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        if (vertices[4] < 1.0f)
        {
            vertices[4] += .1f;
        }
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        if (vertices[7] < 1.0f)
        {
            vertices[7] += .1f;
        }
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS)
    {
        if (vertices[1] > -1.0f)
        {
            vertices[1] -= .1f;
        }
    }
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        if (vertices[4] > -1.0f)
        {
            vertices[4] -= .1f;
        }
    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (vertices[7] > -1.0f)
        {
            vertices[7] -= .1f;
        }
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        if (vertices[2] < 1.0f)
        {
            vertices[2] += .1f;
        }
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        if (vertices[5] < 1.0f)
        {
            vertices[5] += .1f;
        }
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        if (vertices[8] < 1.0f)
        {
            vertices[8] += .1f;
        }
    }
    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        if (vertices[2] > -1.0f)
        {
            vertices[2] -= .1f;
        }
    }
    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        if (vertices[5] > -1.0f)
        {
            vertices[5] -= .1f;
        }
    }
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        if (vertices[8] > -1.0f)
        {
            vertices[8] -= .1f;

        }
    }
}
