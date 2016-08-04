/*
 * File: SierpinskiTriangle.cpp
 * ----------------------------
 * Name: Francisco Lumbreras
 * This file is the starter code for the Sierpinski Triangle problem
 * fron Assignment #3.
 * 
 */

#include <iostream>
#include "graphics.h"
#include "simpio.h"
using namespace std;

void drawFractalTriangle(GPoint p0, double l,int order);

/* Main program */

int main() {
   initGraphics(1000,600);

   //Initialise edge length and fractal order to ensure while loop accessed at first call/
   double edgelen=-1;
   int order=-1;

   //Request user input, if sentinel -> exit/
   while(edgelen<0)
   edgelen=getReal("Please enter length of triange edge  [Typically 200-500] (0 to exit)\n");

   while(order<0&&edgelen!=0)
	   order=getInteger("Please enter fractal order \n");

   if(edgelen==0) return -1;
   

   double wid=getWindowWidth();
   double height=getWindowHeight();
   //Define the initial point in order to draw first triangle centred/
   GPoint P0=GPoint(wid/2,height/2-edgelen/2);

   //Call recursive function/
   drawFractalTriangle(P0,edgelen,order);

   return 0;
}

void drawFractalTriangle(GPoint p0, double l,int order)
{
	//Exit condition, all drawing finished if order==0 or edge lenght excessively small
	// and may lead to resolution problems./
	if(order==0||l<1) return;

	//Recursion part 1 - Draw parent triangle
	GPoint p1=drawPolarLine(p0,l,-60);
	GPoint p2=drawPolarLine(p1,l,180);
	p0=drawPolarLine(p2,l,60);


	/*Recursion part 2 - Generate 3 children points (upper vertex of children triangles) and
	call the parent drawer using half the edge length and one fractal order less. Note how 
	the points used for the children are returned after drawing polar lines to place them */

	drawFractalTriangle(p0,l/2,order-1);
	drawFractalTriangle(drawPolarLine(p0,l/2,-60),l/2,order-1);
	drawFractalTriangle(drawPolarLine(p2,l/2,60),l/2,order-1);


	return;


}
