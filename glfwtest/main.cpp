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
#include "Mesh.h"

#include <gl\gl.h>
#include <gl\glu.h>

#include <cal3d\cal3d.h>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//Gets key inputs
void move_camera();
//Moves the camera
void mouse_callback(GLFWwindow* window,double xpos,double ypos);
//Gets mouse movements
void scroll_callback(GLFWwindow* window,double xoffset,double yoffset);
//Sets mouse wheel actions
void mouseClick_callback(GLFWwindow* window, int button, int action, int mods);

void addStationaryCollision(glm::mat4 stationaryObject,glm::vec3 topRight,glm::vec3 bottomLeft);
//Adds objects

void stationaryCollision(glm::mat4 playerPosition,glm::vec3 playerTopRight,glm::vec3 playerBottomLeft,glm::vec3 movement);

//GLuint loadGLTexture(const char *filename);

//I'll probably need to make a moving-to-moving object, but that's a bit convoluted for now.
//I'm reworking collision. I think I'll create a global that holds the areas (A mat4, I'd guess), a function that adds to it, and a function
//That checks for it.

glm::vec3 stationaryUpperRight[99999];
//The upper right portion of any given stationary object
glm::vec3 stationaryBottomLeft[99999];
//The bottom left portion of any given stationary object
int stationarySize = 0;

glm::vec3 playerPos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraPos = playerPos;
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,0.0f,-1.0f);

bool dashForward = false;
bool dashBackward = false;
bool dashLeft = false;
bool dashRight = false;
bool lightAttack = false;
bool rollAttack = false;
bool heavyAttack = false;
bool currentAction = false;
bool canBuffer = false;
bool dashBuffer = false;
bool attackBuffer = false;
bool bufferEnabled = false;
int actionBuffer = 0;

glm::mat4 swordModel;

double dashElapsedTime = 0.0f;
double attackElapsedTime = 0.0f;

void addStationaryCollision(glm::mat4 stationaryObject,glm::vec3 topRight,glm::vec3 bottomLeft)
{
    //Adds the size of the object (Assumes it is rectangular) to the arrays holding the corners.
    //I think I'm rotating something wrong. Specifically, it rotates and has a -0.00 thing, so
    //it rotates to an extremely short degree


    stationaryUpperRight[stationarySize][0] = stationaryObject[3].x + topRight[0];
    stationaryUpperRight[stationarySize][1] = stationaryObject[3].y + topRight[1];
    stationaryUpperRight[stationarySize][2] = stationaryObject[3].z + topRight[2];

    stationaryBottomLeft[stationarySize][0] = stationaryObject[3].x + bottomLeft[0];
    stationaryBottomLeft[stationarySize][1] = stationaryObject[3].y + bottomLeft[1];
    stationaryBottomLeft[stationarySize][2] = stationaryObject[3].z + bottomLeft[2];

    std::cout << "Stationary Object's Matrix: " << glm::to_string(stationaryObject) << std::endl << "Upper Right: " << stationaryUpperRight[stationarySize][0] << std::endl
    << stationaryUpperRight[stationarySize][1] << std::endl << stationaryUpperRight[stationarySize][2] << std::endl << "Lower Left: " << stationaryBottomLeft[stationarySize][0] << std::endl
    << stationaryBottomLeft[stationarySize][1] << std::endl << stationaryBottomLeft[stationarySize][2] << std::endl;

    stationarySize++;
    //Note to self: After importing this, make the collision, then go learn about models.
    //Also something, something, cryptography.
}

void stationaryCollision(glm::vec3 playerPosition,glm::vec3 playerTopRight,glm::vec3 playerBottomLeft,glm::vec3 movement)
{
    //Note to self: Add the movement/direction
    bool collidingX = false;
    bool collidingZ = false;
    int adjacentXPos;
    int adjacentZPos;

    //bool topRightbotLeft;
    //True for top right, false for bot left.

    for (int c=0;c<stationarySize;c++)
    {
        //Okay, so a few things:
        //One, I need it to NOT collide with the floor. This'll be a temporary fix, I guess.

        if ((playerPosition[1]/*+playerBottomLeft[1]*/ > stationaryBottomLeft[c][1] and playerPosition[1]/*+playerBottomLeft[1]*/ < stationaryUpperRight[c][1]))
            //or (playerPosition[1]+playerTopRight[1] > stationaryBottomLeft[c][1] and playerPosition[1]+playerTopRight[1] < stationaryUpperRight[c][1]))
        {
            //std::cout << "The Y collision is with " << c << std::endl;

            if (((playerPosition[0]+playerBottomLeft[0]+movement[0] > stationaryBottomLeft[c][0] and playerPosition[0]+playerBottomLeft[0]+movement[0] < stationaryUpperRight[c][0]) or
                 (playerPosition[0]+playerTopRight[0]+movement[0] > stationaryBottomLeft[c][0] and playerPosition[0]+playerTopRight[0]+movement[0] < stationaryUpperRight[c][0])) and
                (playerPosition[2]+playerBottomLeft[2] >= stationaryBottomLeft[c][2] and playerPosition[2]+playerBottomLeft[2] <= stationaryUpperRight[c][2]))
            {
                std::cout << "The X collision is with " << c << std::endl;
                collidingX = true;
                adjacentXPos = c;
            }

            if (((playerPosition[2]+playerBottomLeft[2]+movement[2] > stationaryBottomLeft[c][2] and playerPosition[2]+playerBottomLeft[2]+movement[2] < stationaryUpperRight[c][2]) or
                 (playerPosition[2]+playerTopRight[2]+movement[2] > stationaryBottomLeft[c][2] and playerPosition[2]+playerTopRight[2]+movement[2] < stationaryUpperRight[c][2])) and
                (playerPosition[2]+playerBottomLeft[2] >= stationaryBottomLeft[c][0] and playerPosition[2]+playerBottomLeft[2] <= stationaryUpperRight[c][0]))
            {
                std::cout << "The Z collision is with " << c << std::endl;
                collidingZ = true;
                adjacentZPos = c;
            }
        }
    }

    if (!collidingX and !collidingZ)
    {
        //std::cout << "No Collision" << std::endl;
        playerPos[0] += movement[0];
        playerPos[2] += movement[2];

    }
    else if (!collidingX)
    {
        std::cout << "X Collision" << std::endl;
        playerPos[2] += movement[2];

        if (movement[0] > 0)
        {
            playerPos[0] = stationaryBottomLeft[adjacentXPos][0] - playerTopRight[0];
        }
        else if (movement[0] < 0)
        {
            playerPos[0] = stationaryUpperRight[adjacentXPos][0] - playerBottomLeft[0];
        }
        collidingZ = false;

    }
    else if (!collidingZ)
    {
        std::cout << "Z Collision" << std::endl;
        playerPos[0] += movement[0];

        if (movement[2] > 0)
        {
            playerPos[2] = stationaryBottomLeft[adjacentZPos][2] - playerTopRight[2];
        }
        else if (movement[2] < 0)
        {
            playerPos[2] = stationaryUpperRight[adjacentZPos][2] - playerBottomLeft[2];
        }

        collidingX = false;
    }
    else
    {
        std::cout << "Double Collision" << std::endl;
        if (movement[0] > 0)
        {
            playerPos[0] = stationaryBottomLeft[adjacentXPos][0] - playerTopRight[0];
        }
        else if (movement[0] < 0)
        {
            playerPos[0] = stationaryUpperRight[adjacentXPos][0] - playerBottomLeft[0];
        }

        if (movement[2] > 0)
        {
            playerPos[2] = stationaryBottomLeft[adjacentZPos][2] - playerTopRight[2];
        }
        else if (movement[2] < 0)
        {
            playerPos[2] = stationaryUpperRight[adjacentZPos][2] - playerBottomLeft[2];
        }

        collidingX = false;
        collidingZ = false;
    }
}

/*
GLuint loadGLTexture(const char *filename)
{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program,filename),0);
}

GLuint loadGLTexture(const char *filename,int textureNum)
{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(ourShader.Program,filename),textureNum);

}
*/

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

GLfloat roomVertices[] =
{    //Vertex Coordinate  //Textures
     //Front
    -20.0f, -20.0f, -20.0f,  0.0f, 0.0f,
     20.0f, -20.0f, -20.0f,  1.0f, 0.0f,
     20.0f,  20.0f, -20.0f,  1.0f, 1.0f,
     20.0f,  20.0f, -20.0f,  1.0f, 1.0f,
    -20.0f,  20.0f, -20.0f,  0.0f, 1.0f,
    -20.0f, -20.0f, -20.0f,  0.0f, 0.0f,

     //Back
    -20.0f, -20.0f,  20.0f,  0.0f, 0.0f,
     20.0f, -20.0f,  20.0f,  1.0f, 0.0f,
     20.0f,  20.0f,  20.0f,  1.0f, 1.0f,
     20.0f,  20.0f,  20.0f,  1.0f, 1.0f,
    -20.0f,  20.0f,  20.0f,  0.0f, 1.0f,
    -20.0f, -20.0f,  20.0f,  0.0f, 0.0f,

     //Left
    -20.0f,  20.0f,  20.0f,  1.0f, 0.0f,
    -20.0f,  20.0f, -20.0f,  1.0f, 1.0f,
    -20.0f, -20.0f, -20.0f,  0.0f, 1.0f,
    -20.0f, -20.0f, -20.0f,  0.0f, 1.0f,
    -20.0f, -20.0f,  20.0f,  0.0f, 0.0f,
    -20.0f,  20.0f,  20.0f,  1.0f, 0.0f,

     //Right
     20.0f,  20.0f,  20.0f,  1.0f, 0.0f,
     20.0f,  20.0f, -20.0f,  1.0f, 1.0f,
     20.0f, -20.0f, -20.0f,  0.0f, 1.0f,
     20.0f, -20.0f, -20.0f,  0.0f, 1.0f,
     20.0f, -20.0f,  20.0f,  0.0f, 0.0f,
     20.0f,  20.0f,  20.0f,  1.0f, 0.0f,

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

GLfloat cubeSword[] =
{    //Vertex Coordinate  //Textures
     //Front
    -0.02f,  0.00f, -0.01f,  0.0f, 0.0f,
     0.02f,  0.00f, -0.01f,  1.0f, 0.0f,
     0.02f,  0.35f, -0.01f,  1.0f, 1.0f,
     0.02f,  0.35f, -0.01f,  1.0f, 1.0f,
    -0.02f,  0.35f, -0.01f,  0.0f, 1.0f,
    -0.02f,  0.00f, -0.01f,  0.0f, 0.0f,

     //Back
    -0.02f,  0.00f,  0.01f,  0.0f, 0.0f,
     0.02f,  0.00f,  0.01f,  1.0f, 0.0f,
     0.02f,  0.35f,  0.01f,  1.0f, 1.0f,
     0.02f,  0.35f,  0.01f,  1.0f, 1.0f,
    -0.02f,  0.35f,  0.01f,  0.0f, 1.0f,
    -0.02f,  0.00f,  0.01f,  0.0f, 0.0f,

     //Left
    -0.02f,  0.35f,  0.01f,  1.0f, 0.0f,
    -0.02f,  0.35f, -0.01f,  1.0f, 1.0f,
    -0.02f,  0.00f, -0.01f,  0.0f, 1.0f,
    -0.02f,  0.00f, -0.01f,  0.0f, 1.0f,
    -0.02f,  0.00f,  0.01f,  0.0f, 0.0f,
    -0.02f,  0.35f,  0.01f,  1.0f, 0.0f,

     //Right
     0.02f,  0.35f,  0.01f,  1.0f, 0.0f,
     0.02f,  0.35f, -0.01f,  1.0f, 1.0f,
     0.02f,  0.00f, -0.01f,  0.0f, 1.0f,
     0.02f,  0.00f, -0.01f,  0.0f, 1.0f,
     0.02f,  0.00f,  0.01f,  0.0f, 0.0f,
     0.02f,  0.35f,  0.01f,  1.0f, 0.0f,

     //Bottom
    -0.02f,  0.00f, -0.01f,  0.0f, 1.0f,
     0.02f,  0.00f, -0.01f,  1.0f, 1.0f,
     0.02f,  0.00f,  0.01f,  1.0f, 0.0f,
     0.02f,  0.00f,  0.01f,  1.0f, 0.0f,
    -0.02f,  0.00f,  0.01f,  0.0f, 0.0f,
    -0.02f,  0.00f, -0.01f,  0.0f, 1.0f,

     //Top
    -0.02f,  0.35f, -0.01f,  0.0f, 1.0f,
     0.02f,  0.35f, -0.01f,  1.0f, 1.0f,
     0.02f,  0.35f,  0.01f,  1.0f, 0.0f,
     0.02f,  0.35f,  0.01f,  1.0f, 0.0f,
    -0.02f,  0.35f,  0.01f,  0.0f, 0.0f,
    -0.02f,  0.35f, -0.01f,  0.0f, 1.0f
};
//For a floor
//Well, I got a room. Not really a floor, but still. Good enough, I guess.
//Perhaps this method only works for cubes, and not rectangles?
//Nope! Turns out I just need to adjust for positions.

GLfloat wallVertices[] =
{    //Vertex Coordinate  //Textures
     //Front
    -20.0f,  -0.05f, -20.0f,  0.0f, 0.0f,
     20.0f,  -0.05f, -20.0f,  1.0f, 0.0f,
     20.0f,   0.05f, -20.0f,  1.0f, 1.0f,
     20.0f,   0.05f, -20.0f,  1.0f, 1.0f,
    -20.0f,   0.05f, -20.0f,  0.0f, 1.0f,
    -20.0f,  -0.05f, -20.0f,  0.0f, 0.0f,

     //Back
    -20.0f,  -0.05f,  20.0f,  0.0f, 0.0f,
     20.0f,  -0.05f,  20.0f,  1.0f, 0.0f,
     20.0f,   0.05f,  20.0f,  1.0f, 1.0f,
     20.0f,   0.05f,  20.0f,  1.0f, 1.0f,
    -20.0f,   0.05f,  20.0f,  0.0f, 1.0f,
    -20.0f,  -0.05f,  20.0f,  0.0f, 0.0f,

     //Left
    -20.0f,   0.05f,  20.0f,  1.0f, 0.0f,
    -20.0f,   0.05f, -20.0f,  1.0f, 1.0f,
    -20.0f,  -0.05f, -20.0f,  0.0f, 1.0f,
    -20.0f,  -0.05f, -20.0f,  0.0f, 1.0f,
    -20.0f,  -0.05f,  20.0f,  0.0f, 0.0f,
    -20.0f,   0.05f,  20.0f,  1.0f, 0.0f,

     //Right
     20.0f,   0.05f,  20.0f,  1.0f, 0.0f,
     20.0f,   0.05f, -20.0f,  1.0f, 1.0f,
     20.0f,  -0.05f, -20.0f,  0.0f, 1.0f,
     20.0f,  -0.05f, -20.0f,  0.0f, 1.0f,
     20.0f,  -0.05f,  20.0f,  0.0f, 0.0f,
     20.0f,   0.05f,  20.0f,  1.0f, 0.0f,

     //Bottom
    -20.0f, -0.05f, -20.0f,  0.0f, 1.0f,
     20.0f, -0.05f, -20.0f,  1.0f, 1.0f,
     20.0f, -0.05f,  20.0f,  1.0f, 0.0f,
     20.0f, -0.05f,  20.0f,  1.0f, 0.0f,
    -20.0f, -0.05f,  20.0f,  0.0f, 0.0f,
    -20.0f, -0.05f, -20.0f,  0.0f, 1.0f,

     //Top
    -20.0f,  0.05f, -20.0f,  0.0f, 1.0f,
     20.0f,  0.05f, -20.0f,  1.0f, 1.0f,
     20.0f,  0.05f,  20.0f,  1.0f, 0.0f,
     20.0f,  0.05f,  20.0f,  1.0f, 0.0f,
    -20.0f,  0.05f,  20.0f,  0.0f, 0.0f,
    -20.0f,  0.05f, -20.0f,  0.0f, 1.0f
};

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

//GLfloat deltaTime = 0.0f;
//GLfloat lastFrame = 0.0f;
//Establishes time
bool collision;

//For now unused, to be used for directional collision
bool forwardCollision = false;
bool backwardCollision = false;
bool leftCollision = false;
bool rightCollision = false;
bool topCollision = false;
bool bottomCollision = false;
bool gravityEnabled = true;

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

    glm::vec3 pCubePos = playerPos;

    const std::string modelPointer = "test";
    //I need a constant address of a string
    //Creates the model
    //Kind of? I have no frickin' clue what I'm doing here
    //Based on the fact this library has NO FRIGGIN' USABLE DOCUMENTATION OR EXAMPLES
    //Note to self: Create/Destroy were removed and replaced with Constructors/Destructors. The documentation is outdated.
    //Son of a...
    //Okay, I THINK and this is PURE speculation, the &variant of the model is purely for inheriting previous models.
    //So I need to get a &string, which means the address of the string

    Mesh monkey;
    std::vector<glm::vec3> vertexData;

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
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
    //Sets what function manages scrolling the mouse wheel
    glfwSetMouseButtonCallback(window,mouseClick_callback);
    //Sets what function manages mouse clicks.

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

    glm::mat4 modelFloor;
    glm::mat4 modelCeiling;
    glm::mat4 modelLeftWall;
    glm::mat4 modelRightWall;
    glm::mat4 modelFrontWall;
    glm::mat4 modelBackWall;
    GLfloat ninetyFlip = 90.0f;

    modelFloor = glm::translate(modelFloor,glm::vec3(0.0f,-20.0f,0.0f));
    modelFloor = modelFloor,glm::vec3(0.0f,0.0f,0.0f);
    std::cout << "Floor Matrix : " << glm::to_string(modelFloor) << std::endl;
    addStationaryCollision(modelFloor,glm::vec3(20.0f,0.05f,20.0f),glm::vec3(-20.0f,-0.05f,-20.0f));

    modelCeiling = glm::translate(modelCeiling,glm::vec3(0.0f,20.0f,0.0f));
    modelCeiling = modelCeiling,glm::vec3(0.0f,0.0f,0.0f);
    std::cout << "Ceiling Matrix : " << glm::to_string(modelCeiling) << std::endl;
    addStationaryCollision(modelCeiling,glm::vec3(20.0f,0.05f,20.0f),glm::vec3(-20.0f,-0.05f,-20.0f));

    modelLeftWall = glm::translate(modelLeftWall,glm::vec3(-20.0f,0.0f,0.0f));
    modelLeftWall = modelLeftWall,glm::vec3(0.0f,0.0f,0.0f);
    modelLeftWall = glm::rotate(modelLeftWall,ninetyFlip,glm::vec3(0.0f,0.0f,1.0f));
    std::cout << "Left Wall Matrix : " << glm::to_string(modelLeftWall) << std::endl;
    addStationaryCollision(modelLeftWall,glm::vec3(0.05f,20.0f,20.0f),glm::vec3(-0.05f,-20.0f,-20.0f));

    modelRightWall = glm::translate(modelRightWall,glm::vec3(20.0f,0.0f,0.0f));
    modelRightWall = modelRightWall,glm::vec3(0.0f,0.0f,0.0f);
    modelRightWall = glm::rotate(modelRightWall,ninetyFlip,glm::vec3(0.0f,0.0f,1.0f));
    std::cout << "Right Wall Matrix : " << glm::to_string(modelRightWall) << std::endl;
    addStationaryCollision(modelRightWall,glm::vec3(0.05f,20.0f,20.0f),glm::vec3(-0.05f,-20.0f,-20.0f));

    modelFrontWall = glm::translate(modelFrontWall,glm::vec3(0.0f,0.0f,-20.0f));
    modelFrontWall = modelFrontWall,glm::vec3(0.0f,0.0f,0.0f);
    modelFrontWall = glm::rotate(modelFrontWall,ninetyFlip,glm::vec3(1.0f,0.0f,0.0f));
    std::cout << "Front Wall Matrix : " << glm::to_string(modelFrontWall) << std::endl;
    addStationaryCollision(modelFrontWall,glm::vec3(20.0f,20.0f,0.05f),glm::vec3(-20.0f,-20.0f,-0.05f));

    modelBackWall = glm::translate(modelBackWall,glm::vec3(0.0f,0.0f,20.0f));
    modelBackWall = modelBackWall,glm::vec3(0.0f,0.0f,0.0f);
    modelBackWall = glm::rotate(modelBackWall,ninetyFlip,glm::vec3(1.0f,0.0f,0.0f));
    std::cout << "Back Wall Matrix : " << glm::to_string(modelBackWall) << std::endl;
    addStationaryCollision(modelBackWall,glm::vec3(20.0f,20.0f,0.05f),glm::vec3(-20.0f,-20.0f,-0.05f));

    monkey.LoadObjModel("Data/monkey.obj");
    vertexData = monkey.returnMesh();

    /*
    for (int v=0;v<vertexData.size()*5;v++)
    {
        for (int g=0;g<5;g++)
        {
            if (g < 3)
            {
                vertexDataGL[(v*5)+g] = vertexData.at(g)[v];
            }
            else
            {
                vertexDataGL[(v*5)+g] = 0.0f;
            }
        }
    }*/


    glm::mat4 monkeyModelMatrix;

    monkeyModelMatrix = glm::mat4(1.0f);
    monkeyModelMatrix = glm::translate(monkeyModelMatrix,glm::vec3(0.0f,-18.0f,0.0f));
    monkeyModelMatrix = monkeyModelMatrix,glm::vec3(0.0f,0.0f,0.0f);
    //monkeyModelMatrix = glm::scale(monkeyModelMatrix,glm::vec3(1.0,1.0,1.0));
    std::cout << "Monkey Matrix : " << glm::to_string(monkeyModelMatrix) << std::endl;


    swordModel = glm::translate(swordModel,glm::vec3(0.0f,-20.0f,3.0f));
    swordModel = swordModel,glm::vec3(0.0f,0.0f,0.0f);
    swordModel = glm::rotate(swordModel,GLfloat(60),glm::vec3(-1.0f,0.0f,0.0f));
    swordModel = glm::rotate(swordModel,GLfloat(90),glm::vec3(0.0f,1.0f,0.0f));

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        //GLfloat currentFrame = glfwGetTime();
        //deltaTime = currentFrame - lastFrame;
        //lastFrame = currentFrame;

        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //Sets the polygons as unfilled

        //glBindVertexArray(VAO);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_DYNAMIC_DRAW);

        glfwPollEvents();
        move_camera();

        cameraPos = playerPos;
        swordModel[3].x = playerPos[0]+.05+(.2*cameraFront.x);
        swordModel[3].y = playerPos[1]-.15;
        swordModel[3].z = playerPos[2]+(.2*cameraFront.z);
        ///So they now follow your cursor, but does NOT rotate. Mm. Could be worse.
        //This needs to be loaded once. Unfortunately, because this loads every loop, it does an awkward perma-roll.
        //Sooooooo, I need to stuff this rotation in to the mouse-movement, but that would make this a global. Hm.
        ///Okay, maybe I need to rotate it by the change, not the rotation itself.

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


        //glm::mat4 trans;
        //Not generating here makes the square spin extremely quickly
        //trans = glm::rotate(trans,(GLfloat)glfwGetTime()*50.0f,glm::vec3(0.0f,0.0f,1.0f));

        //GLuint transformLoc = glGetUniformLocation(ourShader.Program,"transform");
        //glUniformMatrix4fv(transformLoc,1,GL_FALSE,glm::value_ptr(trans));

        ourShader.Use();

        //Draws the triangle
        //glUseProgram(shaderProgram);

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


        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        //glBindVertexArray(VAO);
        //glBufferData(GL_ARRAY_BUFFER, monkey.vertexNum,monkey.vertexCoords,GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(glm::vec3),vertexData.data(),GL_STATIC_DRAW);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER,vertexData.size()*3,vertexData,GL_STATIC_DRAW);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER,monkey.indexNum,monkey.vertexIndices,GL_STATIC_DRAW);


        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
        glEnableVertexAttribArray(0);

        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(monkeyModelMatrix));
        glDrawArrays(GL_TRIANGLES,0,vertexData.size());

        //glDrawElements(GL_TRIANGLES,monkey.indexNum,GL_UNSIGNED_INT,0);
        ///NOTE: You are drawing with the indices, given the vertices. Calculate it or something.


        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)0);
        glEnableVertexAttribArray(0);
        //Color
        //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
        //glEnableVertexAttribArray(1);
        //Texture
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),(GLvoid*)(3* sizeof(GLfloat)));
        glEnableVertexAttribArray(2);




        /**
        The 10th cube goes around the camera, probably because it has no values and defaults there.
        cameraPos has the same effect, but because it makes it take the camera as a position.
        **/

        glBufferData(GL_ARRAY_BUFFER, sizeof(smallVertices),smallVertices,GL_STATIC_DRAW);
        glBindVertexArray(VAO);
        //Causes the player's box to be smaller

        glm::mat4 modelp;

        modelp = glm::translate(modelp,pCubePos);
        //This is it's own thing, so the camera will freemove forever, but the cube will not.
        modelp = modelp,glm::vec3(0.0f,0.0f,0.0f);
        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelp));
        //glDrawArrays(GL_TRIANGLES,0,30);
        //Temporarily disabled so I can actually look down

        glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices),wallVertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);

        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelFloor));
        glDrawArrays(GL_TRIANGLES,0,36);
        //Draws the floor

        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelCeiling));
        glDrawArrays(GL_TRIANGLES,0,36);
        //Draws the ceiling

        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelLeftWall));
        glDrawArrays(GL_TRIANGLES,0,36);

        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelRightWall));
        glDrawArrays(GL_TRIANGLES,0,36);

        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelFrontWall));
        glDrawArrays(GL_TRIANGLES,0,36);

        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(modelBackWall));
        glDrawArrays(GL_TRIANGLES,0,36);


        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeSword),cubeSword,GL_STATIC_DRAW);
        glBindVertexArray(VAO);

        glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(swordModel));
        glDrawArrays(GL_TRIANGLES,0,36);



        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindVertexArray(VAO);

        //This is a fairly ad-hoc gravity.
        if (gravityEnabled == true)
        {
            playerPos.y -= .1;
        }


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
                                playerPos.z = model[3].z+.65;
                            }
                            else
                            {
                                playerPos.z = model[3].z-.65;

                            }
                        }
                    if (std::abs(modelp[3].x - model[3].x) > std::abs(modelp[3].z - model[3].z) and
                        std::abs(modelp[3].x - model[3].x) > std::abs(modelp[3].y - model[3].y))
                        {
                            std::cout << "Most displacement: X" << std::endl;
                            if (modelp[3].x > model[3].x)
                            {
                                playerPos.x = model[3].x+.65;
                            }
                            else
                            {
                                playerPos.x = model[3].x-.65;

                            }

                        }
                    if (std::abs(modelp[3].y - model[3].y) > std::abs(modelp[3].z - model[3].z) and
                        std::abs(modelp[3].y - model[3].y) > std::abs(modelp[3].x - model[3].x))
                        {
                            std::cout << "Most displacement: Y" << std::endl;
                            if (modelp[3].y > model[3].y)
                            {
                                playerPos.y = model[3].y+.65;
                            }
                            else
                            {
                                playerPos.y = model[3].y-.65;

                            }
                        }
                }
        }




        if (modelp[3].y-.15 <= -20)
        {
            playerPos.y = -20+.15;
            gravityEnabled = false;
            bottomCollision = true;
            //std::cout << "Collision with Floor" << std::endl;
        }
        else
        {
            bottomCollision = false;
            gravityEnabled = true;
        }


        if (dashForward)
        {
            dashElapsedTime = glfwGetTime();
            if (dashElapsedTime <= .25)
            {
                stationaryCollision(playerPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3((.4)*cameraFront.x,0,(.4)*cameraFront.z));
            }
            if (dashElapsedTime >= .15)
            {
                canBuffer = true;
            }
            if (dashElapsedTime >= .35)
            {
                dashForward = false;
                currentAction = false;
            }
        }
        else if (dashBackward)
        {
            dashElapsedTime = glfwGetTime();

            if (dashElapsedTime <= .25)
            {
                stationaryCollision(playerPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3(-(.4*cameraFront.x),0,-(.4*cameraFront.z)));
            }
            if (dashElapsedTime >= .15)
            {
                canBuffer = true;
            }
            if (dashElapsedTime >= .35)
            {
                dashBackward = false;
                currentAction = false;
            }
        }
        else if (dashLeft)
        {
            dashElapsedTime = glfwGetTime();
            if (dashElapsedTime <= .25)
            {
                stationaryCollision(playerPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3(-(glm::normalize(glm::cross(cameraFront,cameraUp))*GLfloat(.4))));
            }
            if (dashElapsedTime >= .15)
            {
                canBuffer = true;
            }
            if (dashElapsedTime >= .35)
            {
                dashLeft = false;
                currentAction = false;
            }
        }
        else if (dashRight)
        {
            dashElapsedTime = glfwGetTime();

            if (dashElapsedTime <= .25)
            {
                stationaryCollision(playerPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3((glm::normalize(glm::cross(cameraFront,cameraUp))*GLfloat(.4))));
            }
            if (dashElapsedTime >= .15)
            {
                canBuffer = true;
            }
            if (dashElapsedTime >= .35)
            {
                dashRight = false;
                currentAction = false;
            }
        }
        else if (lightAttack)
        {

        }
        else if (heavyAttack)
        {
            attackElapsedTime = glfwGetTime();
            if (attackElapsedTime <= .25)
            {
                ///Rightclicking breaks it for now.
                //Pull back time
                std::cout << "Rotating" << std::endl;
                swordModel = glm::rotate(swordModel,GLfloat(1),glm::vec3(-1.0f,0.0f,0.0f));
            }
            if (attackElapsedTime > .25 and attackElapsedTime <= .5)
            {
                //The actual thrust
            }
            if (attackElapsedTime > .5 and attackElapsedTime < .75 and !bufferEnabled)
            {
                //Pull back time, also the time to let a buffer in.
                canBuffer = true;
                currentAction = false;

            }
            //Basically, pull back, make a thrust. Should have some time after for cancelling/addition input

        }
        else if (rollAttack)
        {

        }

        if (canBuffer == true and currentAction == false)
        {
            //Okay, works. It feels a bit rough though.
            if (actionBuffer == 1)
            {
                std::cout << "Buffered Frontdash" << std::endl;
                glfwSetTime(0);
                dashForward = true;
                currentAction = true;
            }
            else if (actionBuffer == 2)
            {
                std::cout << "Buffered Backdash" << std::endl;
                glfwSetTime(0);
                dashBackward = true;
                currentAction = true;
            }
            else if (actionBuffer == 3)
            {
                std::cout << "Buffered Leftdash" << std::endl;
                glfwSetTime(0);
                dashLeft = true;
                currentAction = true;
            }
            else if (actionBuffer == 4)
            {
                std::cout << "Buffered Rightdash" << std::endl;
                glfwSetTime(0);
                dashRight = true;
                currentAction = true;
            }
            else if (actionBuffer == 5)
            {
                std::cout << "You leftclicked (Buffer Variant)" << std::endl;
                currentAction = true;
                rollAttack = true;
                ///Do a special dash attack?
                currentAction = false;

            }
            else if (actionBuffer == 6)
            {
                std::cout << "You rightclicked (Buffer Variant)" << std::endl;
                currentAction = true;
                heavyAttack = true;
                ///Note: Should do a heavy attack regardless
                currentAction = false;
            }
            canBuffer = false;
            actionBuffer = 0;
        }

        //This DOES draw it, BUT, it only draws the top/bottom (I can't tell which).
        //Causes the player's box to be smaller

        glm::mat4 modelf;
        glDrawArrays(GL_TRIANGLES,0,36);



        glBindVertexArray(0);

        glfwSwapBuffers(window);

        //std::cout << "Camera Position: " << cameraPos[0] << ", " << cameraPos[1] << ", " << cameraPos[2] << std::endl;

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
    GLfloat camSpeed = 0.2f;
    //How fast the camera moves
    //double dashTime = 1.0f;
    //How long a dash should last. Might end up making it deal with physics instead of a fixed time, but for now, this'll do.
    //How long a dash has lasted.
    if (collision == false and currentAction == false /*and dashForward == false and dashBackward == false and dashLeft == false and dashRight == false*/)
    {
        //Okay! So there is one bump, but no more. There's also the problem that if you block the while facing left, you block the forward.
        //Maybe I need to adjust for what direction the player is facing? Or maybe just check if the collision is going to happen if you move a direction in general?
        //Yeah, that'll probably need to happen.
        if (keys[GLFW_KEY_W])
        {
            if (keys[GLFW_KEY_SPACE])
            {
                glfwSetTime(0);
                dashForward = true;
                currentAction = true;
                /*
                //A time loop for ~ a second where you move fast but can't walk. Maybe allow queuing up attacks or something.
                stationaryCollision(cameraPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3(camSpeed*cameraFront.x,0,camSpeed*cameraFront.z));
                glfwSetTime(1);
                while (dashElapsedTime < 2)
                {
                    stationaryCollision(cameraPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3(camSpeed*cameraFront.x,0,camSpeed*cameraFront.z));
                    //stationaryCollision(cameraPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3((camSpeed*3)*cameraFront.x,0,(camSpeed*3)*cameraFront.z));
                    std::cout << "Test" << std::endl;
                    dashElapsedTime = glfwGetTime();
                }
                dashElapsedTime = 0;
                //Maybe try putting this in the main while loop, so that it won't hold the game still for a few seconds? That might be the problem.
                */
            }
            else
            {
                stationaryCollision(playerPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3(camSpeed*cameraFront.x,0,camSpeed*cameraFront.z));
            }
            //if (!forwardCollision)
            //{
                //cameraPos.z += camSpeed*cameraFront.z;
                //cameraPos.x += camSpeed*cameraFront.x;

            //}

            //cameraPos.z += cameraFront.z;
        }
        if (keys[GLFW_KEY_S])
        {
            if (keys[GLFW_KEY_SPACE])
            {
                glfwSetTime(0);
                dashBackward = true;
                currentAction = true;

            }
            else
            {
                stationaryCollision(playerPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3(-(camSpeed*cameraFront.x),0,-(camSpeed*cameraFront.z)));
            }
            //cameraPos -= (camSpeed*cameraFront);
            /*
            if (!backwardCollision)
            {
                //cameraPos.z -= camSpeed*cameraFront.z;
                //cameraPos.x -= camSpeed*cameraFront.x;
            }*/
            //cameraPos.z += 1.0f;
        }
        if (keys[GLFW_KEY_A])
        {
            if (keys[GLFW_KEY_SPACE])
            {
                glfwSetTime(0);
                dashLeft = true;
                currentAction = true;

            }
            else
            {
                stationaryCollision(playerPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3(-(glm::normalize(glm::cross(cameraFront,cameraUp))*camSpeed)));
            }
            /*
            if (!leftCollision)
            {
                //cameraPos -= glm::normalize(glm::cross(cameraFront,cameraUp))*camSpeed;
                //cameraPos.x -= camSpeed*cameraFront.x;
            }*/
            //cameraPos.x -= (glm::normalize(glm::cross(cameraFront.x,cameraUp))*camSpeed);

            //cameraPos.x -= 1.0f;
        }
        if (keys[GLFW_KEY_D])
        {
            if (keys[GLFW_KEY_SPACE])
            {
                glfwSetTime(0);
                dashRight = true;
                currentAction = true;

            }
            else
            {
                stationaryCollision(playerPos,glm::vec3(.15,.15,.15),glm::vec3(-.15,-.15,-.15),glm::vec3((glm::normalize(glm::cross(cameraFront,cameraUp))*camSpeed)));
            }
            /*if (!rightCollision)
            {
                //cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp))*camSpeed;;
            }*/
            //cameraPos.x += 1.0f;
        }
    }
    else if (canBuffer == true and actionBuffer == 0)
    {
        if (keys[GLFW_KEY_W] and keys[GLFW_KEY_SPACE])
        {
            actionBuffer = 1;
        }
        if (keys[GLFW_KEY_S] and keys[GLFW_KEY_SPACE])
        {
            actionBuffer = 2;
        }
        if (keys[GLFW_KEY_A] and keys[GLFW_KEY_SPACE])
        {
            actionBuffer = 3;
        }
        if (keys[GLFW_KEY_D] and keys[GLFW_KEY_SPACE])
        {
            actionBuffer = 4;
        }

    }
}

void mouseClick_callback(GLFWwindow* window, int button, int action, int mods)
{

    if (currentAction == false)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_PRESS)
        {
            std::cout << "You leftclicked" << std::endl;
            lightAttack = true;
            currentAction = true;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT and action == GLFW_PRESS)
        {
            std::cout << "You rightclicked" << std::endl;
            heavyAttack = true;
            currentAction = true;
        }
    }
    else if (canBuffer == false and actionBuffer == 0)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT and action == GLFW_PRESS)
        {
            actionBuffer = 5;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT and action == GLFW_PRESS)
        {
            actionBuffer = 6;
        }

    }
    //Just here so clicking doesn't break anything.
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
    //swordModel = glm::rotate(swordModel,pitch,glm::vec3(1.0f,0.0f,0.0f));
    ///Okay, 2 things: 1) Rotates too much.
    ///2) Rotates downward for whatever reason.
    //swordModel = glm::rotate(swordModel,yoffset,glm::vec3(0.0f,0.0f,1.0f));
    //swordModel = glm::rotate(swordModel,GLfloat(front.z - cameraFront[2]),glm::vec3(0.0f,0.0f,1.0f));
    cameraFront = glm::normalize(front);
    //swordModel[0].x = cameraFront[0];
    //swordModel[1].x = cameraFront[0];

    //swordModel[2].x = cameraFront[0];
    //swordModel = glm::rotate(swordModel,GLfloat(cameraFront[0]),glm::vec3(1.0f,0.0f,0.0f));
    //swordModel = glm::rotate(swordModel,GLfloat(cameraFront[1]),glm::vec3(1.0f,0.0f,0.0f));
    //swordModel = glm::rotate(swordModel,GLfloat(cameraFront[2]),glm::vec3(1.0f,0.0f,0.0f));
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
