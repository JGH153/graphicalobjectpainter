#pragma once

#include <GL/glew.h>
#include <GL/GL.h>
//#include <SDL.h>
//#include <SDL_opengl.h>
#include <vector>
//#include "Drawable.h"
#include "Object.h"
#include "GUI.h"
#include <windows.h>




//pre declarations
struct SDL_MouseMotionEvent;

class MainWindow
{
    bool mIsValid;
    bool running;
    float mCameraAngleX;
    float mCameraAngleY;
    float mCameraZoom;
	glm::vec3 cameraPos;

	std::vector<Object*> Objects;
	int currentObjectID;

	int drawMode;

	std::vector<char *> filenames;

	GUI * currentGUI;

	float newColor[3];

	int lastMouseX;
	int lastMouseY;

	int lastRelativeMouseX;
	int lastRelativeMouseY;

	bool hideMouseInPlace;

public:
    MainWindow(std::vector<char *>& filenames);
    ~MainWindow();

    bool isValid();
    void exec();


protected:
    bool initGL();
	void initLights();
    void initObjects();
	void initInterface();
    void handleKeys( unsigned char key, int x, int y );
	void handleMouseClick(int x, int y);
	void colorObject(int mouseX, int mouseY);
    void update(double deltaTime);
    void draw(bool inputToDetectObject);	
    void resizeWindow(int width, int height);
    void mouseMove(const SDL_MouseMotionEvent &motion, int x, int y);
	void handeMouseEvent(SDL_Event sdlEvent, int x, int y);
	void updateCurrentColor(int mouseX, int mouseY);
	void loadFileByBrowsing();

};
