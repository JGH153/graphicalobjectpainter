#pragma once

#include "Object.h"

using namespace std;

Object::Object() :
    mIsInitialised(false){

	std::vector<colorAction> actionsList;

}

Object::~Object()
{}




//this is a bit messy function
bool Object::loadTxtFile(const string &filename)
{
ifstream infile(filename);
	if (!infile.good()) {
		cout << "File load error\n";
		return false;
	}

	float x,y,z;
	float scale;
	int vertArrayPos;

	//.obj file variables
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec2> temp_uvs;

	numvertices = 0;
	numtriangles = 0;
	

	//If file extension is .obj. Does not work if the file uses serveral '.'s e.g. monk.ey.johannes.obj
	if (filename.substr(filename.find_last_of(".") + 1) == "obj") {
		std::cout << "Reading .obj file" << std::endl;
		while(infile.good()) {
			char lineCode[64];
			
			
			infile >> lineCode;

			if (strcmp(lineCode, "v") == 0 ) {
				numvertices ++;
				glm::vec3 vertex;
				infile >> vertex.x;
				infile >> vertex.y;
				infile >> vertex.z;
				
				vertices.push_back(vertex);

				temp_vertices.push_back(vertex); 
			} else if (strcmp(lineCode, "vt") == 0 ) {
				glm::vec2 uv;
				infile >> uv.x;
				infile >> uv.y;
				temp_uvs.push_back(uv);
			} else if (strcmp(lineCode, "vn") == 0 ) {
				glm::vec3 normal;
				infile >> normal.x;
				infile >> normal.y;
				infile >> normal.z;
				temp_normals.push_back(normal);
			} else if (strcmp(lineCode, "f") == 0 ) {
				numtriangles ++;
				unsigned int vertexIndex[3];
				unsigned int uvIndex[3];
				unsigned int normalIndex[3];

				infile >> vertexIndex[0];
				infile.ignore();
				infile >> uvIndex[0];
				infile.ignore();
				infile >> normalIndex[0];

				infile >> vertexIndex[1];
				infile.ignore();
				infile >> uvIndex[1];
				infile.ignore();
				infile >> normalIndex[1];

				infile >> vertexIndex[2];
				infile.ignore();
				infile >> uvIndex[2];
				infile.ignore();
				infile >> normalIndex[2];
				
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices    .push_back(uvIndex[0]);
				uvIndices    .push_back(uvIndex[1]);
				uvIndices    .push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

				fileTriangleList.push_back(glm::vec3(vertexIndex[0], vertexIndex[1], vertexIndex[2]));

			}
		}

		//For each vertex of each triangle
		for (int i = 0; i < vertexIndices.size(); i++) {
			glm::vec3 vertex = temp_vertices[vertexIndices[i]-1];
			renderVertices.push_back(vertex);
		}
		for (int i = 0; i < uvIndices.size(); i++) {
			glm::vec2 uv = temp_uvs[uvIndices[i]-1];
			uvs.push_back(uv);
		}
		for (int i = 0; i < normalIndices.size(); i++) {
			glm::vec3 normal = temp_normals[normalIndices[i]-1];
			normals.push_back(normal);
		}

	//Else if we load a txt file
	} else if (filename.substr(filename.find_last_of(".") + 1) == "txt" 
			|| filename.substr(filename.find_last_of(".") + 1) == "cxt" ) {
		std::cout << "Loading " << filename.substr(filename.find_last_of(".") + 1) << " file " << std::endl;
		infile >> numvertices;
		infile >> numtriangles;


		for (int i = 0; i < numvertices; i++) {
			infile >> x;
			infile >> y;
			infile >> z;
			vertices.push_back(glm::vec3(x, y, z));
		}
		


		//load triangle indexes
		std::vector<glm::vec3> tempVertexNormals;
		for (int i = 0; i < numtriangles*3; i++) {

			tempVertexNormals.push_back(glm::vec3(0.f, 0.f, 0.f));

		}


		//attempt at making vertex based normals...
		for (int i = 0; i < numtriangles*3; i+=3) {

			int vertex1, vertex2, vertex3;
			infile >> vertex1 >> vertex2 >> vertex3; 

			fileTriangleList.push_back(glm::vec3(vertex1, vertex2, vertex3));

			vertex1 --;
			vertex2 --;
			vertex3 --;

			renderVertices.push_back(vertices[vertex1]);
			renderVertices.push_back(vertices[vertex2]);
			renderVertices.push_back(vertices[vertex3]);

			glm::vec3 tempnormal = glm::normalize(glm::cross(renderVertices[i+2] - renderVertices[i], renderVertices[i+1] - renderVertices[i]))*-1.0f;

			tempVertexNormals[i] += tempnormal;
			tempVertexNormals[i+1] += tempnormal;
			tempVertexNormals[i+2] += tempnormal;


		}

		for (int i = 0; i < numtriangles*3; i++) {

			if(tempVertexNormals[i] != glm::vec3(0.f, 0.f, 0.f)){
				normals.push_back(glm::normalize(tempVertexNormals[i]));
			}
		}
		//but it didn't work =(


		//Cheking od colors is at the end of the file
		//if it's stupid and it works, it's not stupid <3
		int nextNum = -1;
		int currentPos = infile.tellg();
		//cheking next value, then reseting
		infile >> nextNum;
		infile.seekg(currentPos);

		//if a valid value, add colors
		if(nextNum >= 0){
			//colors!!!
			for (int i = 0; i < renderVertices.size()/3; i++) {

				int r;
				int g;
				int b;

				infile >> r >> g >> b;

				//convert from char -> float
				float fR = ((r) & 0xFF )/255.0f;
				float fG = ((g) & 0xFF )/255.0f;
				float fB = ((b) & 0xFF )/255.0f;

				//std::cout << r << " " << g << " " << b << endl;
				//std::cout << fR << " " << fG << " " << fB << endl;


				objectcolors.push_back(glm::vec3(fR, fG, fB));
				objectcolors.push_back(glm::vec3(fR, fG, fB));
				objectcolors.push_back(glm::vec3(fR, fG, fB));

				//the colors we set to with the reset button
				objectResetcolors.push_back(glm::vec3(fR, fG, fB));
				objectResetcolors.push_back(glm::vec3(fR, fG, fB));
				objectResetcolors.push_back(glm::vec3(fR, fG, fB));


			}

		}





	} else {
		std::cout << "Parser unable to load unknown filetype" << std::endl;
		return false;
	}

	//scaling!
	float closestPS = INT_MAX;
	float remotestPS = INT_MIN;
	//find the closest and the remotest vertex
	for each (glm::vec3 currentVertex in renderVertices){

		//cout << currentVertex.x << " " << currentVertex.y << " " << currentVertex.z << endl;;

		if(closestPS > currentVertex.x)
			closestPS = currentVertex.x;
		if(closestPS > currentVertex.y)
			closestPS = currentVertex.y;
		if(closestPS > currentVertex.z)
			closestPS = currentVertex.z;

		if(currentVertex.x > remotestPS)
			remotestPS = currentVertex.x;
		if(currentVertex.y > remotestPS)
			remotestPS = currentVertex.y;
		if(currentVertex.z > remotestPS)
			remotestPS = currentVertex.z;

	}

	//calculating
	float tot = glm::abs(closestPS) + glm::abs(remotestPS);
	float scaleNew = 3/tot;

	//applying new scale
	for(int i = 0; i < renderVertices.size(); i ++){
		renderVertices[i].x *= scaleNew;
		renderVertices[i].y *= scaleNew;
		renderVertices[i].z *= scaleNew;
	}



	//Make object color
	if(objectcolors.size() == 0){
		for (int i = 0; i < renderVertices.size(); i++) {

			//render normaly colors
			objectcolors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
			//the colors we set to with the reset button
			objectResetcolors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

		}
	}


	//Make object Select color
	for (int i = 0; i < renderVertices.size()/3; i++) {

		//A bit of magic to get uique colors for objectSelectcolors
		int r = (i & 0x000000FF) >>  0;
		int g = (i & 0x0000FF00) >>  8;
		int b = (i & 0x00FF0000) >> 16;

		//cout << r<< g<< b<<endl;

		objectSelectcolors.push_back(glm::vec3(r/255.0f, g/255.0f, b/255.0f));
		objectSelectcolors.push_back(glm::vec3(r/255.0f, g/255.0f, b/255.0f));
		objectSelectcolors.push_back(glm::vec3(r/255.0f, g/255.0f, b/255.0f));

	}



    mIsInitialised = true;
    return true;  //Return true if succusfuly loaded
}

void Object::changeVertexColor(GLbyte pixelColor[4], float newColor[3]){

	bool fistFound = false;
	
	//looping ALL vertexes and applyin g new color if selectcolor maches (remember 3 vertexes pr triangle)
	for (int i = 0; i < renderVertices.size(); i++) {

		//see if the buffer color matches the current unique color
		if(		(float)( ((int)pixelColor[0]) & 0xFF )/255.0f == objectSelectcolors[i].x
			&&	(float)( ((int)pixelColor[1]) & 0xFF )/255.0f == objectSelectcolors[i].y
			&&	(float)( ((int)pixelColor[2]) & 0xFF )/255.0f == objectSelectcolors[i].z){
			
			//undo action only needs the first of the three
			if(!fistFound){
				
				fistFound = true;

				//if no relevant change (vertex already  having that color)
				if(!(objectcolors[i].x == newColor[0]
				&& objectcolors[i].y == newColor[1]
				&& objectcolors[i].z == newColor[2])){

					//push an action for undo
					colorAction actionPush;
					//first is the objectSelectcolors
					actionPush.x = (((int)pixelColor[0]) & 0xFF )/255.0f;
					actionPush.y = (((int)pixelColor[1]) & 0xFF )/255.0f;
					actionPush.z = (((int)pixelColor[2]) & 0xFF )/255.0f,	
								
					//then the old value
					actionPush.r = objectcolors[i].x,
					actionPush.g = objectcolors[i].y,
					actionPush.b = objectcolors[i].z;

					actionsList.push_back(actionPush);

				}
			}

			//finaly setting new colors
			objectcolors[i].x = newColor[0];
			objectcolors[i].y = newColor[1];
			objectcolors[i].z = newColor[2];

		}



	}


}

void Object::undoLastAction(){

	//if no actions
	if(actionsList.size() < 1)
		return;

	colorAction lastAction = actionsList[actionsList.size() - 1];

	//applying prev color
	for (int i = 0; i < renderVertices.size(); i++) {

		if(	lastAction.x == objectSelectcolors[i].x
		&&	lastAction.y == objectSelectcolors[i].y
		&&	lastAction.z == objectSelectcolors[i].z){
			
			objectcolors[i].x = lastAction.r;
			objectcolors[i].y = lastAction.g;
			objectcolors[i].z = lastAction.b;

		}


	}

	//and removing
	actionsList.pop_back();

	cout << "Action undoned" << endl;

}

//setting colors back to default, and clearing undo list
void Object::resetColors(){

	//the beauty of vectors
	objectcolors = objectResetcolors;

	actionsList.clear();

}

//This migth sound like a horrible function...
//Athter writing this function i came to realise i've just written
//an obj -> (your basic file format) convertor by accident ^^
void Object::saveYourself(){

	//straightforward, save a .cxt file in the advanced file format

	ofstream outFile;
	outFile.open ("object.cxt");
	outFile << numvertices << " " << numtriangles << endl;

	for (int i = 0; i < numvertices; i++) {
		outFile << vertices[i].x << " ";
		outFile << vertices[i].y << " ";
		outFile << vertices[i].z;
		outFile << endl;
	}

	for each (glm::vec3 currentTiangle in fileTriangleList){

		outFile << currentTiangle.x << " ";
		outFile << currentTiangle.y << " ";
		outFile << currentTiangle.z;
		outFile << endl;

	}

	//color is given pr vertex, but we need pr triangle:
	for(int i = 0; i < objectcolors.size(); i +=3){

		int r = objectcolors[i].x*255;
		int g = objectcolors[i].y*255;
		int b = objectcolors[i].z*255;

		//std:: cout << (int)r << " " << (int)g << " " << (int)b << endl;
		//cin.get();

		outFile << r << " ";
		outFile << g << " ";
		outFile << b;
		outFile << endl;

	}

	outFile.close();

	cout << "File saved as object.cxt" << endl;

}




//drawing object
void Object::draw(bool useSelectColors){

    if(!mIsInitialised) { return; }

	//drawing object
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, renderVertices.data());

	//use normal colors or the selectcolors
	if(useSelectColors)
		glColorPointer(3, GL_FLOAT, 0, objectSelectcolors.data());
	else
		glColorPointer(3, GL_FLOAT, 0, objectcolors.data());



	glNormalPointer(GL_FLOAT, 0, normals.data());

    glDrawArrays(GL_TRIANGLES, 0, renderVertices.size());

	
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);






}
