#include "ButtenGUI.h"


ButtenGUI::ButtenGUI(float posX, float posY, float sizeX, float sizeY, char * imagePath, int externalTexFormat){

	this->posX = posX;
	this->posY = posY;

	this->sizeX = sizeX;
	this->sizeY = sizeY;

	this->imagePath = imagePath;
	this->externalTexFormat = externalTexFormat;

	texID = loadTexture(imagePath, externalTexFormat);

	//could have changed to static because same across all buttons
	texCords.push_back(glm::vec2(0.0f,0.0f));
	texCords.push_back(glm::vec2(1.0f,0.0f));
	texCords.push_back(glm::vec2(1.0f,1.0f));
	texCords.push_back(glm::vec2(0.0f,1.0f));

	vertexList.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	vertexList.push_back(glm::vec3(sizeX, 0.0f, 1.0f));
	vertexList.push_back(glm::vec3(sizeX, sizeY, 1.0f));
	vertexList.push_back(glm::vec3(0.0f, sizeY, 1.0f));

}


//used for getting color of image for the color picer sircle, but not in use
Uint32 get_pixel32( SDL_Surface *surface, int x, int y ){
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

GLuint ButtenGUI::loadTexture(char * filename, int externalTexFormat){


	SDL_Surface* tempSurface = IMG_Load(filename);

	//on load error
	if (tempSurface == nullptr){
		std::cout << SDL_GetError() << std::endl;
		std::cin.get();
		return 0;
	}

	GLuint returnID = 0;

	//get ID
	glGenTextures(1, &returnID);
	glBindTexture(GL_TEXTURE_2D, returnID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempSurface->w, tempSurface->h, 0, externalTexFormat, GL_UNSIGNED_BYTE, tempSurface->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(tempSurface);

	return returnID;

}



ButtenGUI::~ButtenGUI(void){

	

}


//rendering button
void ButtenGUI::draw(int screenWidth, int screenHeight){

	glTranslatef(posX, posY, 0); 

	//set texture tu use
	glBindTexture(GL_TEXTURE_2D, texID);

	//reset to white, in order for texture to show
	glColor3f(1.0f,1.0f,1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertexList.data());
	glTexCoordPointer(2, GL_FLOAT, 0, texCords.data());
	glDrawArrays(GL_QUADS, 0, vertexList.size());
    
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//unset texture (0 = none)
	glBindTexture(GL_TEXTURE_2D, 0);

	glTranslatef(-posX, -posY, 0); 

}

//simple function for cheking if mouse is over button
bool ButtenGUI::hitTest(float x, float y){

	if(x >= this->posX && x <= this->posX + this->sizeX
	&& y >= this->posY && y <= this->posY + this->sizeY){
		return true;
	}

	return false;
}