//Collision notes:
//It's done. Collision has been been successfully implemented.
//
//I need to find the position of the cubes
//Then I need to camera position (should be cameraPos)
//Then I need to check if they collide, and if so, prevent movement.
//First off, I should just find the position of a cube.
//The last column is what I should look at, it appears the others are just defining the cube's dimensions.
//For future reference, a screenshot of the cube's matrices are in the folder.
///Maybe I should make a cube-object and make it have the same coordinates as the camera? Then bind the camera
///with the cube.
//OpenGL probably has a seperate value for the position and the vertex, so the vertexes doesn't get distorted
//when the object moves. Hence, what I need to do is find the location of the cubes, then add the vertexes, and make
//collision true (And maybe include the cube around the player for good measure)

//SO: model[3](.x/.y/.z) is where the center coordinate is. Now I just need to adjust for the cubes surrounding
//the point. How'll I do that?

//Matrix note:
//*/ is for resizing
//+- is for rotation and transformation
/** According to http://www.math.ucla.edu/~wittman/10c.1.11s/Lectures/Raids/Graphics3D.pdf,
For Columns:
X:        []X[]Y[]Z[]Point(1) or Vector(0)
Y:        []X[]Y[]Z[]Point(1) or Vector(0)
Z:        []X[]Y[]Z[]Point(1) or Vector(0)
Position: []X[]Y[]Z[]Point(1) or Vector(0)
Something like that?
Looks like everything is rotating a bit.
**/

//Note: All non-collision code was copied from learnopengl.com
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
#include <glm/ext.hpp>

#include "Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//Gets key inputs
void move_camera();
//Moves the camera
void mouse_callback(GLFWwindow* window,double xpos,double ypos);
//Gets mouse movements
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset);

float red = 1.0f;
float green = 1.0f;
float blue = 1.0f;

const GLuint WIDTH = 800, HEIGHT = 600;

GLfloat timeValue = glfwGetTime();

GLfloat vertices[] =
{    //Vertex Coordinate  //Textures
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

GLfloat smallVertices[] =
{    //Vertex Coordinate  //Textures
    -0.15f, -0.15f, -0.15f,  0.0f, 0.0f,
     0.15f, -0.15f, -0.15f,  1.0f, 0.0f,
     0.15f,  0.15f, -0.15f,  1.0f, 1.0f,
     0.15f,  0.15f, -0.15f,  1.0f, 1.0f,
    -0.15f,  0.15f, -0.15f,  0.0f, 1.0f,
    -0.15f, -0.15f, -0.15f,  0.0f, 0.0f,

    -0.15f, -0.15f,  0.15f,  0.0f, 0.0f,
     0.15f, -0.15f,  0.15f,  1.0f, 0.0f,
     0.15f,  0.15f,  0.15f,  1.0f, 1.0f,
     0.15f,  0.15f,  0.15f,  1.0f, 1.0f,
    -0.15f,  0.15f,  0.15f,  0.0f, 1.0f,
    -0.15f, -0.15f,  0.15f,  0.0f, 0.0f,

    -0.15f,  0.15f,  0.15f,  1.0f, 0.0f,
    -0.15f,  0.15f, -0.15f,  1.0f, 1.0f,
    -0.15f, -0.15f, -0.15f,  0.0f, 1.0f,
    -0.15f, -0.15f, -0.15f,  0.0f, 1.0f,
    -0.15f, -0.15f,  0.15f,  0.0f, 0.0f,
    -0.15f,  0.15f,  0.15f,  1.0f, 0.0f,

     0.15f,  0.15f,  0.15f,  1.0f, 0.0f,
     0.15f,  0.15f, -0.15f,  1.0f, 1.0f,
     0.15f, -0.15f, -0.15f,  0.0f, 1.0f,
     0.15f, -0.15f, -0.15f,  0.0f, 1.0f,
     0.15f, -0.15f,  0.15f,  0.0f, 0.0f,
     0.15f,  0.15f,  0.15f,  1.0f, 0.0f,

    -0.15f, -0.15f, -0.15f,  0.0f, 1.0f,
     0.15f, -0.15f, -0.15f,  1.0f, 1.0f,
     0.15f, -0.15f,  0.15f,  1.0f, 0.0f,
     0.15f, -0.15f,  0.15f,  1.0f, 0.0f,
    -0.15f, -0.15f,  0.15f,  0.0f, 0.0f,
    -0.15f, -0.15f, -0.15f,  0.0f, 1.0f,

    -0.15f,  0.15f, -0.15f,  0.0f, 1.0f,
     0.15f,  0.15f, -0.15f,  1.0f, 1.0f,
     0.15f,  0.15f,  0.15f,  1.0f, 0.0f,
     0.15f,  0.15f,  0.15f,  1.0f, 0.0f,
    -0.15f,  0.15f,  0.15f,  0.0f, 0.0f,
    -0.15f,  0.15f, -0.15f,  0.0f, 1.0f
};
//For the player cube's coordinates.

GLfloat floorVertices[] =
{    //Vertex Coordinate  //Textures
     //Front
    -0.05f, -0.05f, -0.05f,  0.0f, 0.0f,
     0.05f, -0.05f, -0.05f,  1.0f, 0.0f,
     0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
     0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
    -0.05f,  0.05f, -0.05f,  0.0f, 1.0f,
    -0.05f, -0.05f, -0.05f,  0.0f, 0.0f,

     //Back
    -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
     0.05f, -0.05f,  0.05f,  1.0f, 0.0f,
     0.05f,  0.05f,  0.05f,  1.0f, 1.0f,
     0.05f,  0.05f,  0.05f,  1.0f, 1.0f,
    -0.05f,  0.05f,  0.05f,  0.0f, 1.0f,
    -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,

     //Left
    -0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
    -0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
    -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
    -0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
    -0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
    -0.05f,  0.05f,  0.05f,  1.0f, 0.0f,

     //Right
     0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
     0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
     0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
     0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
     0.05f, -0.05f,  0.05f,  0.0f, 0.0f,
     0.05f,  0.05f,  0.05f,  1.0f, 0.0f,

     //Bottom
    -20.0f, -20.0f, -20.0f,  0.0f, 1.0f,
     20.0f, -20.0f, -20.0f,  1.0f, 1.0f,
     20.0f, -20.0f,  20.0f,  1.0f, 0.0f,
     20.0f, -20.0f,  20.0f,  1.0f, 0.0f,
    -20.0f, -20.0f,  20.0f,  0.0f, 0.0f,
    -20.0f, -20.0f, -20.0f,  0.0f, 1.0f,

     //Top
    -20.0f,  20.0f, -20.0f,  0.0f, 1.0f,
     20.0f,  20.0f, -20.0f,  1.0f, 1.0f,
     20.0f,  20.0f,  20.0f,  1.0f, 0.0f,
     20.0f,  20.0f,  20.0f,  1.0f, 0.0f,
    -20.0f,  20.0f,  20.0f,  0.0f, 0.0f,
    -20.0f,  20.0f, -20.0f,  0.0f, 1.0f
};
//For a floor

glm::vec3 cubePositions[] =
{
    //I am convinced that deleting one
    //makes a cube around the camera.
    //Maybe that's the default position? To be at around the camera?
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f),
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

glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);
//Sets up position of the camera
//Found the error: Front and Up had the reversed values.

GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
//Sets direction of where camera is pointed.

GLfloat lastX = WIDTH/2;
GLfloat lastY = HEIGHT/2;
//Creates variables for Camera movements

GLfloat aspect = 45.0f;
//Sets FOV

bool keys[1024];
//Stores key input

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
//Establishes time
bool collision;

//For now unused, to be used for directional collision
bool forwardCollision;
bool backwardCollision;
bool leftCollision;
bool rightCollision;
bool topCollision;
bool bottomCollision;

int main()
{
    //Array to draw the vertexes.
    GLuint VBO;
    //GLuint vertexShader;
    //GLuint fragmentShader;
    //GLuint shaderProgram;
    GLuint VAO;
    GLuint EBO;
    GLuint texture1,texture2;
    glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
    glm::mat4 trans;

    glm::vec3 pCubePos = cameraPos;

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
    //std::cout << vec.x << vec.y << vec.z << std::endl;

    int width,height;
    unsigned char* image = SOIL_load_image("container.jpg",&width,&height,0,SOIL_LOAD_RGB);

    //GLfloat timeValue,greenValue,redValue,blueValue;
    //GLint vertexColorLocation;

    //GLint success;
    //GLchar infolog[512];


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
    glViewport(0,0,WIDTH,HEIGHT);
    //Allows OpenGL to know the initial coordinate and the size of the window.

    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"Window Test", NULL, NULL);
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
    //Location of this is important. It crashes if this is at the start.


    glfwSetKeyCallback(window,key_callback);
    //Sets what function manages key presses.
    glfwSetCursorPosCallback(window, mouse_callback);
    //Sets what function manages mouse movement.
    glfwSetScrollCallback(window,scroll_callback);

    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    Shader ourShader("shaders/vShader.vs","shaders/fShader.frag");
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
    //glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices, GL_STATIC_DRAW);

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

    image = SOIL_load_image("Cube Coordinates.png",&width,&height,0,SOIL_LOAD_RGB);
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


    //trans = glm::rotate(trans,90.0f,glm::vec3(0.0f,0.0f,1.0f));
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
        //glm::mat4 trans;
        //Not generating here makes the square spin extremely quickly
        //trans = glm::rotate(trans,(GLfloat)glfwGetTime()*50.0f,glm::vec3(0.0f,0.0f,1.0f));

        //GLuint transformLoc = glGetUniformLocation(ourShader.Program,"transform");
        //glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));

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

        //For applying to all objects in the cubePositions array
        if (collision == false)
        {
            //If area is clear
            view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
            pCubePos = cameraPos;
            /*std::cout << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << std::endl;
            std::cout << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z << std::endl;
            std::cout << cameraUp.x << ", " << cameraUp.y << ", " << cameraUp.z << std::endl;*/
        }
        else
        {
            //If colliding, stop, figure out where it collided at, then put you at the border.
            //view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
            view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
            std::cout << "Colliding." << std::endl;

            //Maybe I should make a variable to hold previous values of
            //cameraPos and cameraUp, so you can always move the camera.
            //It kind of needs to know where it should go, though.
        }
        //Color
        //glm::mat4 model;
        glm::mat4 projection;
        //model = glm::rotate(model,(GLfloat)glfwGetTime() * 50.0f,glm::vec3(0.5f,1.0f,0.0f));
        //view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
        projection = glm::perspective(aspect,(GLfloat)WIDTH/(GLfloat)HEIGHT,.1f,100.0f);

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        //Sends the position to the shader

        //glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(projection));

        //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,36*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
        //glEnableVertexAttribArray(1);

        //glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);



        /**
        The 10th cube goes around the camera, probably because it has no values and defaults there.
        cameraPos has the same effect, but because it makes it take the camera as a position.
        **/

        glBufferData(GL_ARRAY_BUFFER, sizeof(smallVertices),smallVertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        //Causes the player's box to be smaller

        glm::mat4 modelp;

        modelp = glm::translate(modelp,pCubePos);
        //This is it's own thing, so the camera will freemove forever, but the cube will not.
        modelp = modelp,glm::vec3(0.0f,0.0f,0.0f);
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelp));
        glDrawArrays(GL_TRIANGLES,6,36);
        //6 to 36 draws a block without the front.
        //I wanna make this one a bit smaller so I can have more collisions.


        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindVertexArray(VAO);

        for (GLuint i=0;i<10;i++)
        {
            //Draws the cubes
            glm::mat4 model;
            model = glm::translate(model,cubePositions[i]);
            //Sets the positions of the cubes

            //GLfloat angle = 20.0f*(i+1);
            //Sets the angle for rotation

            //model = glm::rotate(model,(GLfloat)glfwGetTime() * angle,glm::vec3(1.0f,0.3f,0.5f));
            //Draws the cubes to rotate
            //model = model,glm::vec3(1.0f,0.3f,0.5f);
            //model[0] = glm::vec4(1,0,0,0);
            //model[1] = glm::vec4(0,1,0,0);
            //model[2] = glm::vec4(0,0,1,0);
            //model[3] = glm::vec4(0,0,0,1);
            //Manually sets the matrices for SCIENCE!!!!!!!!

            glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
            //Binds the location of the cubes
            //I think. It sets everything in camera position without this.

            glDrawArrays(GL_TRIANGLES,0,36);
            //Actually draws the cubes
            //std::cout << "Cube Matrix " << i << " : " << glm::to_string(model) << std::endl;


            //Okay, good news, bad news: Got collision working, but it warps you because multiple
            //collisions act at once.
            if (((modelp[3].z+.15 < model[3].z+.5 and modelp[3].z+.15 > model[3].z-.5)
                or (modelp[3].z-.15 < model[3].z+.5 and model[3].z-.15 > model[3].z-.5)) and

                ((modelp[3].x-.15 > model[3].x-.5 and modelp[3].x-.15 < model[3].x+.5)
                or (modelp[3].x+.15 > model[3].x-.5 and modelp[3].x+.15 < model[3].x+.5)) and

                ((modelp[3].y-.15 > model[3].y-.5 and modelp[3].y-.15 < model[3].y+.5)
                or (modelp[3].y+.15 > model[3].y-.5 and modelp[3].y+.15 < model[3].y+.5)))
                {
                    //This basically just checks if the cube is inside of the block or not
                    std::cout << "Collision is with cube number " << i << std::endl;
                    //What I want to do is make it so that once you collide from an area, it puts you at the
                    //nearest point.
                    if (std::abs(modelp[3].z - model[3].z) > std::abs(modelp[3].x - model[3].x) and
                        std::abs(modelp[3].z - model[3].z) > std::abs(modelp[3].y - model[3].y))
                        {
                            std::cout << "Most displacement: Z" << std::endl;

                            if (modelp[3].z > model[3].z)
                            {
                                cameraPos.z = model[3].z+.65;
                            }
                            else
                            {
                                cameraPos.z = model[3].z-.65;

                            }
                        }
                    if (std::abs(modelp[3].x - model[3].x) > std::abs(modelp[3].z - model[3].z) and
                        std::abs(modelp[3].x - model[3].x) > std::abs(modelp[3].y - model[3].y))
                        {
                            std::cout << "Most displacement: X" << std::endl;
                            if (modelp[3].x > model[3].x)
                            {
                                cameraPos.x = model[3].x+.65;
                            }
                            else
                            {
                                cameraPos.x = model[3].x-.65;

                            }

                        }
                    if (std::abs(modelp[3].y - model[3].y) > std::abs(modelp[3].z - model[3].z) and
                        std::abs(modelp[3].y - model[3].y) > std::abs(modelp[3].x - model[3].x))
                        {
                            std::cout << "Most displacement: Y" << std::endl;
                            if (modelp[3].y > model[3].y)
                            {
                                cameraPos.y = model[3].y+.65;
                            }
                            else
                            {
                                cameraPos.y = model[3].y-.65;

                            }
                        }
                }

            /*if ((modelp[3].z-.15 < model[3].z+.5 and modelp[3].z-.15 > model[3].z-.5)and

                ((modelp[3].x-.15 >= model[3].x-.5 and modelp[3].x-.15 <= model[3].x+.5)
                or (modelp[3].x+.15 >= model[3].x-.5 and modelp[3].x+.15 <= model[3].x+.5)) and

                ((modelp[3].y-.15 >= model[3].y-.5 and modelp[3].y-.15 <= model[3].y+.5)
                or (modelp[3].y+.15 >= model[3].y-.5 and modelp[3].y+.15 <= model[3].y+.5)))
            {
                //Needa make it only apply to blocks.
                //Then make sure it doesn't let you keep going there.
                //std::cout << glm::to_string(modelp) << std::endl << std::endl;
                //Okay, so! This is checking if it's intersecting EVERYWHERE, not just one point.
                //Try just one row/column?

                //collision = true;

                forwardCollision = true;

                std::cout << "Forwards Collision is with cube number " << i << std::endl;

                cameraPos.z = model[3].z+.65;
            }
            /*
                if ((modelp[3].z+.15 < model[3].z+.5 and modelp[3].z+.15 > model[3].z-.5)and

                ((modelp[3].x-.15 >= model[3].x-.5 and modelp[3].x-.15 <= model[3].x+.5)
                or (modelp[3].x+.15 >= model[3].x-.5 and modelp[3].x+.15 <= model[3].x+.5)) and

                ((modelp[3].y-.15 >= model[3].y-.5 and modelp[3].y-.15 <= model[3].y+.5)
                or (modelp[3].y+.15 >= model[3].y-.5 and modelp[3].y+.15 <= model[3].y+.5)))
            {
                //Backwards collision
                //collision = true;

                backwardCollision = true;

                std::cout << "Backwards Collision is with cube number " << i << std::endl;

                cameraPos.z = model[3].z-.65;
            }
            if (((modelp[3].z+.15 <= model[3].z+.5 and modelp[3].z+.15 >= model[3].z-.5)
                or (modelp[3].z-.15 <= model[3].z+.5 and model[3].z-.15 >= model[3].z+.5)) and

                ((modelp[3].x-.15 > model[3].x-.5 and modelp[3].x-.15 < model[3].x+.5)) and

                ((modelp[3].y-.15 >= model[3].y-.5 and modelp[3].y-.15 <= model[3].y+.5)
                or (modelp[3].y+.15 >= model[3].y-.5 and modelp[3].y+.15 <= model[3].y+.5)))
            {
                //Left Collision
                //collision = true;

                leftCollision = true;

                std::cout << "Left Collision is with cube number " << i << std::endl;

                cameraPos.x = model[3].x+.65;
            }
            if (((modelp[3].z+.15 <= model[3].z+.5 and modelp[3].z+.15 >= model[3].z-.5)
                or (modelp[3].z-.15 <= model[3].z+.5 and model[3].z-.15 >= model[3].z+.5)) and

                ((modelp[3].x+.15 > model[3].x-.5 and modelp[3].x+.15 < model[3].x+.5)) and

                ((modelp[3].y-.15 >= model[3].y-.5 and modelp[3].y-.15 <= model[3].y+.5)
                or (modelp[3].y+.15 >= model[3].y-.5 and modelp[3].y+.15 <= model[3].y+.5)))
            {
                //Right Collision
                //collision = true;

                rightCollision = true;

                std::cout << "Right Collision is with cube number " << i << std::endl;

                cameraPos.x = model[3].x-.65;
            }
            if (((modelp[3].z+.15 <= model[3].z+.5 and modelp[3].z+.15 >= model[3].z-.5)
                or (modelp[3].z-.15 <= model[3].z+.5 and model[3].z-.15 >= model[3].z+.5)) and

                ((modelp[3].x-.15 >= model[3].x-.5 and modelp[3].x-.15 <= model[3].x+.5)
                or (modelp[3].x+.15 >= model[3].x-.5 and modelp[3].x+.15 <= model[3].x+.5)) and

                (modelp[3].y+.15 > model[3].y-.5 and modelp[3].y+.15 < model[3].y+.5))
            {
                //Top Collision
                //collision = true;

                topCollision = true;

                std::cout << "Top Collision is with cube number " << i << std::endl;

                cameraPos.y = model[3].y-.65;
            }
            if (((modelp[3].z+.15 <= model[3].z+.5 and modelp[3].z+.15 >= model[3].z-.5)
                or (modelp[3].z-.15 <= model[3].z+.5 and model[3].z-.15 >= model[3].z+.5)) and

                ((modelp[3].x-.15 >= model[3].x-.5 and modelp[3].x-.15 <= model[3].x+.5)
                or (modelp[3].x+.15 >= model[3].x-.5 and modelp[3].x+.15 <= model[3].x+.5)) and

                (modelp[3].y-.15 > model[3].y-.5 and modelp[3].y-.15 < model[3].y+.5))
            {
                //Top Collision
                //collision = true;

                bottomCollision = true;

                std::cout << "Bottom Collision is with cube number " << i << std::endl;

                cameraPos.y = model[3].y+.65;
            }*/
            /*else
            {
                //std::cout << "Player position: " << glm::to_string(modelp) << std::endl;
                //Wait, is modelp the MOUSE position?
                //std::cout << "Cube position: " << model << std::endl;
            }*/
        }


        /* Floor (Under Construction)s
        glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices),floorVertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        //Causes the player's box to be smaller

        glm::mat4 modelf;

        modelf = glm::translate(modelp,glm::vec3(0.0f,0.0f,0.0f));
        //This is it's own thing, so the camera will freemove forever, but the cube will not.
        modelf = modelf,glm::vec3(0.0f,0.0f,0.0f);
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelf));
        glDrawArrays(GL_TRIANGLES,0,36);
        */



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
    if (key >= 0 && key < 1024)
    {
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
}

void move_camera()
{
    //Moves the camera
    GLfloat camSpeed = 5.0f * deltaTime;
    if (collision == false)
    {
        if (keys[GLFW_KEY_W])
        {
            cameraPos += camSpeed*cameraFront;
        }
        if (keys[GLFW_KEY_S])
        {
            cameraPos += -(camSpeed*cameraFront);
        }
        if (keys[GLFW_KEY_A])
        {
            cameraPos += -(glm::normalize(glm::cross(cameraFront,cameraUp))*camSpeed);
        }
        if (keys[GLFW_KEY_D])
        {
            cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp))*camSpeed;
        }
    }
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window,double xpos,double ypos)
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

    GLfloat sensitivity = 0.05f;
    //Sets the sensitivity of the mouse

    xoffset *= sensitivity;
    yoffset *= sensitivity;
    //offsets the offset by the sensitivity

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }
    //Sets caps for the Y values

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window,double xoffset,double yoffset)
{
    if (aspect >= 1.0f && aspect <= 45.0f)
    {
        aspect -= yoffset;
    }
    if (aspect <= 1.0f)
    {
        aspect = 1.0f;
    }
    if (aspect > 45.0f)
    {
        aspect = 45.0f;
    }
}
