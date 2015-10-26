#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <GL/glew.h>

class Mesh
{
    public:
        Mesh();
        virtual ~Mesh();
        void LoadObjModel(const char *filename); //Load object models
        std::vector<glm::vec3> returnMesh(); //Returns the vertices of mesh data
    protected:
    private:
        std::vector<glm::vec3> vertices; //Store v vertex information
        std::vector<glm::vec3> meshVertices; //All 3D model face vertices
        std::vector<int> faceIndex; //Number of face indexes started with f
};

#endif // MESH_H
