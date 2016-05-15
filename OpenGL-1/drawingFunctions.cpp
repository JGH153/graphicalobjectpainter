#include <GL/glew.h>
#include <GL/GL.h>
#include <SDL.h>
#include "drawingFunctions.h"
#include <vector>

using namespace std;

//Draw the x, y and z axis in red, green and blue. They are length 1
void drawAxis()
{

    //Set up the 3 line segments
    static GLfloat vertices[] = { 0.0f, 0.0f, 0.0f,  //x-axis
                           1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f,  //y-axis
                           0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f,  //z-axis
                           0.0f, 0.0f, 1.0f };

    static GLfloat colors[] = { 1.0f, 0.0f, 0.0f,  //red
                         1.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f,  //green
                         0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 1.0f,  //blue
                         0.0f, 0.0f, 1.0f };





    //draw the lines
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_LINES, 0, 6);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
