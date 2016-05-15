#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/GL.h>
#include "SDL.h" 
#include <SDL_image.h>

class ButtenGUI{

private:
	float posX;
	float posY;

	std::vector<glm::vec2> texCords;
	std::vector<glm::vec3> vertexList;

	GLuint texID;
	char * imagePath;

	SDL_Surface* myTex;
	GLuint myTexID;
	int externalTexFormat;

	GLuint loadTexture(char * filename, int externalTexFormat);

public:

	float sizeX;
	float sizeY;

	ButtenGUI(float posX, float posY, float sizeX, float sizeY, char * imagePaht, int externalTexFormat);

	~ButtenGUI(void);
	void draw(int screenWidth, int screenHeight);
	bool hitTest(float x, float y);
};

