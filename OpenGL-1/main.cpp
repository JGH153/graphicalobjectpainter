/*
Exam 2013
Jan Greger Hemb 				121224



List of functionality
- Supports editing of several objects at the same time, switch between with spacebar
- Undo by button or ctrl+z
- Loading the basic fileformat(.txt), the advanced fileformat(.cxt) and .obj files
- Saving in your advanced fileformat by button(and by accident making a .obj to the advanced fileformat convertor)
- Loadin files runtime using Windows file API
- Scaling to fit screen upon loading
- Coloring by left mousebutton, and dragging
- Moving object by right mouse button
- Zooming by mouseweel
- Rendering backfaces in order to allow you to zoom into the object and color it from the inside
- Selecting color fluidly by palette
- Displaying current color in a quad
- Loading .png files
- Hiding and trapping mouse when rotating
- Deleting current object (and exiting if none is left)
- Exit button (exit program)
- NOT using selection indicator on the palette, just made it harder to select a color, got in the way.
- Reusable and well commented code

*/

#pragma once

#include <iostream>
#include "MainWindow.h"


//I love std <3
using namespace std;

int main( int numArgs, char *arguments[] )
{
	//supports multible files, separated by space
	vector<char *> filenames;

	if(numArgs > 1){
		//add each element
		for(int i = 1; i < numArgs; i ++){
			filenames.push_back(arguments[i]);
		}
	}else{//default file
		filenames.push_back("cube.txt");
	}

	//filenames.push_back("cube.txt");

	cout << "Files added to stack:" << endl;
	for(int i = 0; i < filenames.size(); i ++)
			cout << filenames[i] << endl;
	cout << "Starting program..." << endl;

    MainWindow mainWindow(filenames);

    if(!mainWindow.isValid()) {
        cout << "Error creating main window" << endl;
		cin.get();
        exit(0);
    }

    mainWindow.exec();
	
	return 1337;
}
