#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits.h>

struct colorAction{

	float x;
	float y;
	float z;

	float r;
	float g;
	float b;

};


class Object
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> vertexIndices;
	std::vector<glm::vec3> renderVertices;
	std::vector<glm::vec3> fileTriangleList;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> objectcolors;
	std::vector<glm::vec3> objectResetcolors;
	//std::vector<unsigned char> objectcolorsByte;
	std::vector<glm::vec3> objectSelectcolors;
	//std::vector<unsigned char> objectSelectcolorsByte;


	//each post contans following syntax:
	//0-2: objectSelectcolors.x > z, 3-5: prev color
	std::vector<colorAction> actionsList;
	

	int numvertices;
	int numtriangles;
    bool mIsInitialised;


public:
    Object();
    ~Object();

    bool loadTxtFile(const std::string &filename);
    bool isInitialised() { return mIsInitialised; }
    void draw(bool inputToDetectObject);


	void undoLastAction();
	void resetColors();
	void saveYourself();


	void changeVertexColor(GLbyte pixelColor[4], float newColor[3]);


};
