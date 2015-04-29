//Camera's goofed. Fix that crap.n
#include <iostream>
#include <math.h>
//Imports math for formula reasons

// GLEW
//#define GLEW_STATIC
//Removed because already defined in compiler settings.
#include <GL/glew.h>

// GLFW
#include <GL/glfw3.h>
#include <SOIL.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//Gets key inputs
void move_camera();
//Moves the camera
void mousee_callback(GLFWwindow* window,double xpos,double ypos);
//Gets mouse movements

float red = 1.0f;
float blue = 1.0f;
float green = 1.0f;
//Globals for screen colors

/*
GLfloat vertex1x = -0.5f;
GLfloat vertex1y = -0.5f;
GLfloat vertex1z = 0.0f;
GLfloat vertex2x = 0.5f;
GLfloat vertex2y = -0.5f;
GLfloat vertex2z = 0.0f;
GLfloat vertex3x = 0.0f;
GLfloat vertex3y = 0.5f;
GLfloat vertex3z = 0.0f;
*/
//Global points for the triangle. Is here so points can be modified.

GLfloat timeValue = glfwGetTime();

GLfloat vertices[] =
{
    /*
    0.5f, 0.5f,0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    0.5f,-0.5f,0.0f,  0.2f, 0.2f, 0.2f,  1.0f, 0.0f,
   -0.5f,-0.5f,0.0f,  0.2f, 0.2f, 0.2f,  0.0f, 0.0f,
   -0.5f, 0.5f,0.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f
   */
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
//Makes a single cube

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
//Makes multiple positions for cubes

GLuint indices[] =
{
    0,1,3,
    1,2,3
};

GLfloat texCoords[] =
{
    0.0f,0.0f,
    1.0f,0.0f,
    0.5f,1.0f
};

float textureBorderColor[] =
{
    1.0f,1.0f,0.0f,1.0f
};

//Press QWE to raise the color and ASD to lower it.
//Press RTY to raise the X of the triangle and FGH to lower it.
//Press UIO to raise the Y of the triangle and JKL to lower it.
//Press ZXC to raise the Z of the triangle and VBN to lower it.
//Note: none of the triangle adjusts have been implemented.

glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,1.0f,0.0f);
//Sets up position of the camera

bool keys[1024];
//Stores key input

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
//Establishes time

GLfloat lastX = 600;
GLfloat lastY = 300;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
bool firstMouse = true;
//Creates variables for

int main()
{
    //Array to draw the vertexes.
    GLuint VBO;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLuint VAO;
    GLuint EBO;
    GLuint texture1,texture2;
    glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
    glm::mat4 trans;

    //glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
    //glm::vec3 cameraTarget = glm::vec3(0.0f,0.0f,0.0f);
    //glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    //glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
    //glm::vec3 cameraRight = glm::normalize(glm::cross(up,cameraDirection));
    //glm::vec3 cameraUp = glm::vec3(0.0f,0.0f,-1.0f);
    //glm::vec3 cameraFront = glm::vec3(0.0f,1.0f,0.0f);

    //glm::mat4 view;
    //view = glm::lookAt(glm::vec3(0.0f,0.0f,3.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));


    //trans = glm::translate(trans, glm::vec3(0.0f,0.0f,0.0f));
    //Changes where the object is with respect to the center, tutorial says 1.0,0.0,0.0, but that pushes it
    //All the way to the right
    vec = trans*vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;

    int width,height;
    unsigned char* image = SOIL_load_image("container.jpg",&width,&height,0,SOIL_LOAD_RGB);

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
    Shader ourShader("shaders/vShader.vs","shaders/fShader.frag");
    //Location of this is important. It crashes if this is at the start.
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);


    glfwSetKeyCallback(window,key_callback);
    //Sets what function manages key presses.
    /*
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();

    //glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    //glShaderSource(fragmentShader, 1,&fragmentShaderSource,NULL);

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
    */

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);

    glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,textureBorderColor);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    //Generates and binds the vertex

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices, GL_STATIC_DRAW);

    //Position
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    //Color
    //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);
    //Texture
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER,0);



    //glBindVertexArray(0);

    //Generates and binds the textures
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB, GL_UNSIGNED_BYTE,image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D,0);

    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    image = SOIL_load_image("awesomeface.png",&width,&height,0,SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);

    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D,0);

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


    trans = glm::rotate(trans,90.0f,glm::vec3(0.0f,0.0f,1.0f));
    //trans = glm::scale(trans,glm::vec3(0.5f,0.5f,0.5f));


    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //Sets the polygons as unfilled

        //glBindVertexArray(VAO);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

        glfwPollEvents();
        move_camera();

        glClearColor(red,green,blue,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Clears the screen


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program,"ourTexture1"),0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.Program,"ourTexture2"),1);
        //Does not support png's transparency.

        //trans = glm::translate(trans,glm::vec3(0.5f,-0.5f,0.0f));
        //Again, pushes the square too far off the screen.
        glm::mat4 trans;
        //Not generating here makes the square spin extremely quickly
        trans = glm::rotate(trans,(GLfloat)glfwGetTime()*50.0f,glm::vec3(0.0f,0.0f,1.0f));

        GLuint transformLoc = glGetUniformLocation(ourShader.Program,"transform");
        glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));

        ourShader.Use();

        //Draws the triangle
        //glUseProgram(shaderProgram);

        /*
        timeValue = glfwGetTime();
        greenValue = (sin(timeValue)/2+.5);
        redValue = (cos(timeValue)/2+.5);
        blueValue = (sin(timeValue)/2+.5);
        vertexColorLocation = glGetUniformLocation(shaderProgram,"importColor");
        glUseProgram(shaderProgram);
        glUniform3f(vertexColorLocation,redValue,greenValue,blueValue);

        vertices[3] = redValue;
        vertices[11] = greenValue;
        vertices[17] = blueValue
        */

        /*GLfloat radius = 10.0f;
        GLfloat camX = sin(glfwGetTime())*radius;
        GLfloat camZ = cos(glfwGetTime())*radius;*/
        glm::mat4 view;
        view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);


        //Color
        glm::mat4 model;
        glm::mat4 projection;
        model = glm::rotate(model,(GLfloat)glfwGetTime() * 50.0f,glm::vec3(0.5f,1.0f,0.0f));
        //view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
        projection = glm::perspective(45.0f,(GLfloat)800/(GLfloat)600,.1f,100.0f);

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(projection));


        //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,36*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
        //glEnableVertexAttribArray(1);

        //glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
        for (GLuint i = 0;i<10;i++)
        {
            //Draws the cubes
            glm::mat4 model;
            model = glm::translate(model,cubePositions[i]);
            GLfloat angle = 20.0f*(i+1);
            model = glm::rotate(model,(GLfloat)glfwGetTime() * angle,glm::vec3(1.0f,0.3f,0.5f));
            glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES,0,36);
        }

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
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        if (red < 1)
        {
            red += .1f;
        }
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        if (green < 1)
        {
            green += .1f;
        }
    }
    if (key == GLFW_KEY_Y && action == GLFW_PRESS)
    {
        if (blue < 1)
        {
            blue += .1f;
        }
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (red > 0)
        {
            red -= .1f;
        }
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        if (green > 0)
        {
            green -= .1f;
        }
    }
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        if (blue > 0)
        {
            blue -= .1f;
        }
    }
    if (action == GLFW_PRESS)
    {
        //Allows action to occur so long as the key is held
        //Also allows multiple inputs
        keys[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        keys[key] = false;
    }
}

void move_camera()
{
    //Moves the camera
    GLfloat camSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W])
    {
        cameraPos += camSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_S])
    {
        cameraPos -= camSpeed * cameraFront;
    }
    if (keys[GLFW_KEY_A])
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront,cameraUp))*camSpeed;
    }
    if (keys[GLFW_KEY_D])
    {
        cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp))*camSpeed;
    }
}

void mousee_callback(GLFWwindow* window,double xpos,double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    //Finds the difference between frames

    GLfloat sensitivity = .05f;
    //Sets the sensitivity of the mouse

    xoffset *= sensitivity;
    yoffset *= sensitivity;
    //offsets the offset by the sensitivity

    yaw += xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }
    //Sets caps for the Y values

    glm::vec3 frontCam;
    frontCam.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontCam.y = sin(glm::radians(pitch));
    frontCam.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(frontCam);

}
