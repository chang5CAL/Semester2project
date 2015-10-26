#include "Mesh.h"

Mesh::Mesh()
{
    //ctor
}

Mesh::~Mesh()
{
    //dtor
}

void Mesh::LoadObjModel(const char *filename)
{
    //I might just throw this in the constructor. Or call it in there, who knows.

    std::ifstream in(filename, std::ios::in);
    if (!in)
    {
        std::cerr << "Can't open " << filename << ". Try checking if it exists, or if you misspelled it." << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(in,line))
    {
        //std::cout << line << std::endl;
        //Outputs the lines of the file.

        if (line.substr(0,2) == "v ")
        {
            //std::cout << line << std::endl;
            std::istringstream v(line.substr(2));
            glm::vec3 vert;
            double x,y,z;
            v>>x;
            v>>y;
            v>>z;
            vert = glm::vec3(x,y,z);
            vertices.push_back(vert);



        }
        else if (line.substr(0,2) == "f ")
        {
            std::istringstream v(line.substr(2));
            GLuint a,b,c;
            v>>a;
            v>>b;
            v>>c;
            a--;
            b--;
            c--;
            faceIndex.push_back(a);
            faceIndex.push_back(b);
            faceIndex.push_back(c);


        }
    }
    for (unsigned int i=0;i<faceIndex.size();i++)
    {
        glm::vec3 meshData;
        meshData = glm::vec3(vertices[faceIndex[i]].x,vertices[faceIndex[i]].y,vertices[faceIndex[i]].z);
        meshVertices.push_back(meshData);
    }

    /*
    for (unsigned int v=0;v<((faceIndex.size()/3)*5);v+=5)
    {
        vertexCoords[v] = vertices[faceIndex[v]].x;
        vertexCoords[v+1] = vertices[faceIndex[v]].y;
        vertexCoords[v+2] = vertices[faceIndex[v]].z;
        vertexCoords[v+3] = 1.0f;
        vertexCoords[v+4] = 0.0f;
    }
    */

}

std::vector<glm::vec3> Mesh::returnMesh()
{
    return meshVertices;
}
