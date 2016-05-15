#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/GL.h>
#include "SDL.h" 
#include <SDL_image.h>
#include "ButtenGUI.h"

const int lol = 5;

class GUI{

private:

	void preRender();
	void postRender();

	int screenWidth; 
	int screenHeight;

	

public:

	std::vector<ButtenGUI *> buttons;

	ButtenGUI * palettButton;
	ButtenGUI * resetButton;
	ButtenGUI * saveButton;
	ButtenGUI * loadButton;
	ButtenGUI * undoButton;
	ButtenGUI * deleteButton;
	ButtenGUI * exitButton;

	float palettSizeX;
	float palettSizeY;


	float selectedColorBoxSizeX;
	float selectedColorBoxSizeY;


	GUI(int screenWidth, int screenHeight);
	~GUI(void);





	void draw(float drawColor[3]);
};

