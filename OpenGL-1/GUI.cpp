#include "GUI.h"



GUI::GUI(int screenWidth, int screenHeight){

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	selectedColorBoxSizeX = 50;
	selectedColorBoxSizeY = 50;

	//palettButton = new ButtenGUI(	0,					 0,		275.f, 260.f,	palettTexID);
	//in normal size, it was taking too much space
	palettButton = new ButtenGUI(	0,					 0.f,		200.f, 190.f,	"images/palette.png", GL_RGB);

	int buttonSize = 75.f;

	exitButton = new ButtenGUI(		screenWidth - buttonSize, 0.f,				buttonSize, buttonSize,	"images/exit.png", GL_RGBA);
	deleteButton = new ButtenGUI(	screenWidth - buttonSize, buttonSize * 1,	buttonSize, buttonSize,	"images/delete.png", GL_RGBA);
	saveButton = new ButtenGUI(		screenWidth - buttonSize, buttonSize * 2,	buttonSize, buttonSize,	"images/save.png", GL_RGBA);
	loadButton = new ButtenGUI(		screenWidth - buttonSize, buttonSize * 3,	buttonSize, buttonSize,	"images/load.png", GL_RGBA);
	resetButton = new ButtenGUI(	screenWidth - buttonSize, buttonSize * 4,	buttonSize, buttonSize,	"images/reset2.png", GL_RGBA);
	undoButton = new ButtenGUI(		screenWidth - buttonSize, buttonSize * 5,	buttonSize, buttonSize,	"images/undo.png", GL_RGBA);
	

	buttons.push_back(palettButton);
	buttons.push_back(resetButton);
	buttons.push_back(saveButton);
	buttons.push_back(loadButton);
	buttons.push_back(undoButton);
	buttons.push_back(deleteButton);
	buttons.push_back(exitButton);

}


GUI::~GUI(void){

}


void GUI::preRender(){

	// Prepare for GUI rendering:
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);
	glScalef(1, -1, 1);		// Reverse Y axis
	glTranslatef(0, -screenHeight, 0);
	glMatrixMode(GL_MODELVIEW);

	//glTranslatef(0,0,0);

}

void GUI::postRender(){

	// Disable GUI rendering:
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

}

//rendering the GUI
void GUI::draw(float drawColor[3]){

	//setup for GUI render
	preRender();

	//render each button
	for each (ButtenGUI * currentButton in buttons){

		currentButton->draw(screenWidth, screenHeight);

	}



	//render the current color quad
	glTranslatef(palettButton->sizeX, 0, 0); 

	glBegin(GL_QUADS);
		glColor3f(drawColor[0], drawColor[1], drawColor[2]);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(selectedColorBoxSizeX, 0.0f, 1.0f);
		glVertex3f(selectedColorBoxSizeX, selectedColorBoxSizeY, 1.0f);
		glVertex3f(0.0f, selectedColorBoxSizeY, 1.0f);

	glEnd();

	glTranslatef(-palettButton->sizeX, 0, 0);

	//restet back
	postRender();



}