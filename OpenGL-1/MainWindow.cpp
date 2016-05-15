#include "MainWindow.h"
#include <GL/glew.h>
#include <GL/GL.h>
#include <SDL.h>
#include <iostream>
#include <string>

#include "Object.h"
#include "drawingFunctions.h"

using namespace std;

//Screen attributes
const int	SCREEN_WIDTH	= 800;
const int	SCREEN_HEIGHT	= 600;
const int	SCREEN_BPP		= 32;

SDL_Window*	window;
SDL_GLContext glContext;
Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

//The frame rate
const int FRAMES_PER_SECOND = 60;




MainWindow::MainWindow(std::vector<char *>& filenames) :
	filenames(filenames),
    mIsValid(false),
    running(false),
    mCameraAngleX(0.0f),
    mCameraAngleY(0.0f),
    mCameraZoom(8.0f),
	hideMouseInPlace(false),
	cameraPos(glm::vec3(0,0,0)){

		newColor[0] = 0.f;
		newColor[1] = 0.f;
		newColor[2] = 0.f;



    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
        return;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//set anti-alias:
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,16);
	

    //Create Window
	window		= SDL_CreateWindow("Assignment 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, flags);
	glContext	= SDL_GL_CreateContext(window);
	
    if( window == nullptr ){
        cout << "Couldn't set GL mode: " << SDL_GetError() << endl;
        return;
    }
    

    //Initialise the Glew extension library (this has the be done when we have a GL context)
    GLenum err = glewInit();
    if (GLEW_OK != err){

       // Problem: glewInit failed, something is seriously wrong.
       cout << "Error: " << glewGetErrorString(err) << endl;
       return;

    }

    //Initialize OpenGL
	resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);  //Sometimes the resize event doesn't seem to get called on window creation. So just do it
    if( initGL() == false ) {
        return;
    }

	//preload objects
    initObjects();
	initInterface();

    mIsValid = true; 

}

MainWindow::~MainWindow()
{

	//delete objects
	if(Objects.size()){
		for(int i = 0; i < Objects.size(); i++){

			delete Objects[i];

		}
	}

    SDL_Quit();
}

bool MainWindow::isValid()
{
    return mIsValid;
}

bool MainWindow::initGL()
{
    //Print out some OpenGL details
    //std::cout << ::glGetString(GL_VENDOR) << std::endl;
    //std::cout << ::glGetString(GL_RENDERER) << std::endl;
    //std::cout << ::glGetString(GL_VERSION) << std::endl;

	glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable/disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

    glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(1, 1, 1, 1);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    initLights();

	
	

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		cin.get();
        return false;
    }

	//all ok!
    return true;
}

void MainWindow::initLights() {

	glEnable(GL_LIGHTING);

    GLfloat lightAmbient[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightDiffuse[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightSpecular[] = {1, 1, 1, 1};          // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // position the light
    float lightPos[4] = {0, 3, 20, 1}; // positional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// enable the light
    glEnable(GL_LIGHT0); 
}


//preloading objects
void MainWindow::initObjects() {

	Object * tempObj;

	for(int i = 0; i < filenames.size(); i ++){

		tempObj = new Object();
		tempObj->loadTxtFile(filenames[i]);
		Objects.push_back(tempObj);

	}

	//set current object to the first
	currentObjectID = 0;

}

void MainWindow::initInterface(){

	currentGUI = new GUI(SCREEN_WIDTH, SCREEN_HEIGHT);

}

void MainWindow::exec(){
    running = true;

    Uint32 prevFrameStartTime = 0;

	//Wait for user exit
	while(running)
	{
        //Used to regulate the frame rate
        Uint32 curFrameStartTime = SDL_GetTicks();
        if(prevFrameStartTime == 0) {
            prevFrameStartTime = curFrameStartTime;
        }

        //Event handler
        SDL_Event sdlEvent;

        //While there are events to handle
		while( SDL_PollEvent( &sdlEvent ) ){

			if( sdlEvent.type == SDL_QUIT ){
                running = false;
            }
            else if( sdlEvent.type == SDL_KEYDOWN ){

                //Handle keypress with current mouse position
                int x = 0, y = 0;
                SDL_GetMouseState( &x, &y );
                handleKeys( (unsigned char)sdlEvent.key.keysym.sym, x, y );

            } else if( sdlEvent.type ==  SDL_WINDOWEVENT) {

				if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
					resizeWindow(sdlEvent.window.data1, sdlEvent.window.data2); 
				}

            } else {

				//cout << "GO " <<sdlEvent.motion.xrel << " " << lastRelativeMouseX << endl;

				//in order to skip second event triggerd by SDL_WarpMouseInWindow
				if(!(sdlEvent.motion.xrel == -lastRelativeMouseX
				&& sdlEvent.motion.yrel == -lastRelativeMouseY)){
					//all mouse related handling
					handeMouseEvent(sdlEvent, sdlEvent.button.x, sdlEvent.button.y);

				}

				lastRelativeMouseX = sdlEvent.motion.xrel;
				lastRelativeMouseY = sdlEvent.motion.yrel;

				//hiding of mouse when moving
				if(hideMouseInPlace){

					SDL_ShowCursor(false);

					//only when changed
					//if(!(lastMouseX == sdlEvent.button.x && lastMouseY == sdlEvent.button.y))
						SDL_WarpMouseInWindow(window, lastMouseX, lastMouseY);

				}else{

					SDL_ShowCursor(true);
					lastMouseX = sdlEvent.button.x;
					lastMouseY = sdlEvent.button.y;

				}



			}

			


		}

        //Run frame update
        update(double(curFrameStartTime - prevFrameStartTime) / 1000);

        //Render frame
        draw(false);

        //Regulate the frame rate
        Uint32 frameTime = SDL_GetTicks() - curFrameStartTime;
        if( frameTime < 1000 / FRAMES_PER_SECOND){

            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - frameTime );

        }

        prevFrameStartTime = curFrameStartTime;

	}
}


void MainWindow::handeMouseEvent(SDL_Event sdlEvent, int x, int y){

	if( sdlEvent.type == SDL_MOUSEMOTION) {
		//handfling of moving object and draggingcoloring
		mouseMove(sdlEvent.motion, x, y);

		

		if(sdlEvent.button.button == SDL_BUTTON_LEFT){

			if(currentGUI->palettButton->hitTest(x, y)){

				updateCurrentColor(x, y);

			}else{
			
				colorObject(x, y);

			}

		}

			
	}else if(sdlEvent.type == SDL_MOUSEWHEEL){
		//mouseweel zooming	
		mCameraZoom *= (float)exp(0.2 * -sdlEvent.wheel.y);
			
	}else if(sdlEvent.type == SDL_MOUSEBUTTONDOWN){
		//handling mouse click (left)
		if(sdlEvent.button.button == SDL_BUTTON_LEFT){

			handleMouseClick(x, y);

		}


	}else if(sdlEvent.type == SDL_MOUSEBUTTONUP){
		//resetting mouse to visible
		if(sdlEvent.button.button == SDL_BUTTON_RIGHT && hideMouseInPlace)
			hideMouseInPlace = false;


	}

	


}

//color object based on selectcolors
void MainWindow::colorObject(int mouseX, int mouseY){

	//draw to buffer, colorSelect values
	draw(true);

	//retriving a bit more data than needed
	GLbyte color[4];
	GLfloat depth;
	GLuint index;
	glReadPixels(mouseX, SCREEN_HEIGHT - mouseY - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glReadPixels(mouseX, SCREEN_HEIGHT - mouseY - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	glReadPixels(mouseX, SCREEN_HEIGHT - mouseY - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

	bool hitButton = false;
	for each (ButtenGUI * curtrentButton in currentGUI->buttons){

		if(curtrentButton->hitTest(mouseX, mouseY))
			hitButton = true;

	}

	//if no button is pressed, color away!
	if(!hitButton && depth != 1)
		Objects[currentObjectID]->changeVertexColor(color, newColor);


}

//pressing left mouse button, buttons and coloring
void MainWindow::handleMouseClick(int mouseX, int mouseY){

	//draw to buffer, colorSelect values
	draw(true);

	//GLbyte color[4];
	//GLfloat depth;
	//GLuint index;
	//glReadPixels(mouseX, SCREEN_HEIGHT - mouseY - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	//glReadPixels(mouseX, SCREEN_HEIGHT - mouseY - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	//glReadPixels(mouseX, SCREEN_HEIGHT - mouseY - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

	//printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n", mouseX, mouseY, color[0], color[1], color[2], color[3], depth, index);				
	//printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx\n", mouseX, mouseY, color[0], color[1], color[2], color[3]);

	//finging pressed button 
	if(currentGUI->saveButton->hitTest(mouseX, mouseY)){

		Objects[currentObjectID]->saveYourself();

	}else if(currentGUI->undoButton->hitTest(mouseX, mouseY)){

		Objects[currentObjectID]->undoLastAction();

	}else if(currentGUI->palettButton->hitTest(mouseX, mouseY)){

		updateCurrentColor(mouseX, mouseY);

	}else if(currentGUI->resetButton->hitTest(mouseX, mouseY)){

		Objects[currentObjectID]->resetColors();


	}else if(currentGUI->deleteButton->hitTest(mouseX, mouseY)){
		
		//delete current object, if none is left; quit
		delete(Objects[currentObjectID]);

		Objects.erase(Objects.begin()+currentObjectID);

		if(Objects.size() == 0){
			running = false;
		}else{//set current to first if any is left
			currentObjectID = 0;
		}


	}else if(currentGUI->exitButton->hitTest(mouseX, mouseY)){
			
		running = false;

	}else if(currentGUI->loadButton->hitTest(mouseX, mouseY)){
			

		loadFileByBrowsing();


	}else{
		//remembert, if color is white, no object is colored
		colorObject(mouseX, mouseY);

	}


}

//opens a windows filebrowser and loads the selected file 
void MainWindow::loadFileByBrowsing(){
	//needs VS settings->general->caracter set->Not Set

	//this codeblock is stolen, i take no credit
	OPENFILENAME ofn ;
	char filename[255] ;
	ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.hwndOwner = NULL ;
	ofn.lpstrFile = filename ;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof( filename );
	ofn.lpstrFilter = "Text Files (*.txt;*.cxt;*.obj)\0*.txt;*.cxt;*.obj\0\0";//OK file formats
	ofn.nFilterIndex =1;
	ofn.lpstrFileTitle = NULL ;
	ofn.nMaxFileTitle = 0 ;
	ofn.lpstrInitialDir=NULL ;
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
	GetOpenFileName(&ofn);
	//END of steal

	//cout << "Filename selected: " << filename << endl;

	Object * tempObj;

	//loading selected file
	tempObj = new Object();
	tempObj->loadTxtFile(filename);
	Objects.push_back(tempObj);
	currentObjectID = Objects.size()-1;

}

//setting color for the quad displaying current color
void MainWindow::updateCurrentColor(int mouseX, int mouseY){

	GLbyte color[4];
	glReadPixels(mouseX, SCREEN_HEIGHT - mouseY - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	//setting color for the quad displaying current color
	newColor[0] = ( ((int)color[0]) & 0xFF )/255.0f;
	newColor[1] = ( ((int)color[1]) & 0xFF )/255.0f;
	newColor[2] = ( ((int)color[2]) & 0xFF )/255.0f;


}


void MainWindow::resizeWindow(int width, int height)
{
    glViewport(0, 0, width, height);

    //Setup the perspective projection matrix
    glMatrixMode(GL_PROJECTION);                // Select The Projection Matrix
    glLoadIdentity();                           // Reset The Projection Matrix
    gluPerspective(60.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);  // Calculate The Aspect Ratio Of The Window
 
}

void MainWindow::handleKeys( unsigned char key, int x, int y ){
	
    if( key == ' ' ) { //Spacebar pressed

		//switching object
		if(currentObjectID >= Objects.size()-1)
			currentObjectID = 0;
		else 
			currentObjectID ++;
	
	}else if(toupper (key) == 'Z' && (SDL_GetModState() & KMOD_CTRL)){

		//ctrl z
		Objects[currentObjectID]->undoLastAction();

		
	} else if(key == 27) { //Escape key
        running = false;
    } 
}


void MainWindow::mouseMove(const SDL_MouseMotionEvent &motion, int x, int y){

	if(motion.state & SDL_BUTTON_RMASK) { //if left mouse button is currently pressed down
		const float scale = 0.2f;
		mCameraAngleX += (float)motion.xrel * scale;
		mCameraAngleY += (float)motion.yrel * scale;

		hideMouseInPlace = true;
		
	}



}

//update is called once per frame
//deltaTime is the total time of the last frame
void MainWindow::update(double deltaTime)
{
	//Get current camera position, its magic (Yup)
	GLdouble mdl[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, mdl);
    cameraPos.x = -(mdl[0] * mdl[12] + mdl[1] * mdl[13] + mdl[2] * mdl[14]);
    cameraPos.y = -(mdl[4] * mdl[12] + mdl[5] * mdl[13] + mdl[6] * mdl[14]);
    cameraPos.z = -(mdl[8] * mdl[12] + mdl[9] * mdl[13] + mdl[10] * mdl[14]);
}

void MainWindow::draw(bool useSelectColors){

	if(useSelectColors){
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0); 
	}
	

	glClearColor(1, 1, 1, 1);                   // background color


	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();


	glViewport(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(60.0f, (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT, 0.1f, 500.0f);  

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


    //Set the camera view matrix - Note we need to use the inverse when setting the camera
    //As moving the camera backward by x is like moving the world forward by x, etc
    glTranslatef(0, 0, -mCameraZoom);    //this puts the camera at (0, 0, x)
    glRotatef(mCameraAngleY, 1, 0, 0);
    glRotatef(mCameraAngleX, 0, 1, 0);

    //After finishing the camera movement - this is effectively world space here
    //Any more transformations will be into object space
    //However, we can use push and pop to return to this matrix state

    //drawAxis();

    glPushMatrix();

	//if vector contains objects, start drawing
	if(Objects.size()) {

		Objects[currentObjectID]->draw(useSelectColors);

    }

    glPopMatrix();



	//no light in GUI
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0); 
	
	currentGUI->draw(newColor);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	

    //Update screen, but only of we are rendering normaly. 
	if(!useSelectColors)
		SDL_GL_SwapWindow(window);
}
