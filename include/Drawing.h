/******************************************
* Official Name: Bruno Osorio Oliveira dos Santos
* Nickname: Bruno
* E-mail: bosorioo@syr.edu
* Assignment: Assignment 3
* Environment/Compiler:  Visual Studio 2012
*
* Possible inputs:
*
*  Up/Down arrows - adjust simulation speed
*  Left/Right arrows - rotate world for some cameras
*  '+' and '-' - scale the world
*  'C' - change ground rendering mode
*  'P' - turns on/off helicopter when it is on ground, lands on center if helicopter if flying.
*  'D' - toggle view of flight path
*  'F' - makes helicopter fly again over objects if it is turned on
*  'S' - toggle view of helicopter shadow
*  'Q' - toggle view of information box on bottom of screen
*  'W' - toggle infinite balloons mode
*  'A' / Mouse click on helicopter - shoots a balloon
*  'R' - reset most configurations to default
*  'Z' - changes camera one by one
*  '1' until '7' - sets current camera
*  '0' - sets current camera to 5 (overhead view)
*  'X' - toggle helicopter/balloons/paint batches wired mode
*
* The helicopter turns on when 'P' is pressed while it is on ground and turned off.
* It turns off when 'P' is pressed while it is on ground and turned on.
* It takes off when 'F' is pressed and it's engine speed is at 100%.
* If 'F' is pressed during flight, helicopter restarts it's path from beginning.
* If 'P' is pressed during flight, helicopter will fly back to center and land, but keep engines on.
*******************************************/

#ifndef _DRAWING_H
#define _DRAWING_H

#define PI 3.14159265f
#include <vector>

class Point2D
{
public:
	Point2D();
	Point2D(float x, float y);
	void setPoint(float x, float y);
	float x();
	float y();
private:
	float m_x, m_y;
};

class Color
{
public:
	Color();
	Color(int r, int g, int b);
	Color(float r, float g, float b);
	Color(const Color& c);
	void glColor();
	void set(int r, int g, int b);
	void set(float r, float g, float b);
	void set(Color c);
	float red();
	float green();
	float blue();
private:
	float m_r, m_g, m_b;
};

namespace Draw
{

	void drawpoints(std::vector<Point2D>& points, bool filled = true);

	void text(const char* text, float x, float y, float z, bool centered);
	void circle2d(float radius, int points = 60, bool filled = true);
	void square2d(float size, bool filled = true);
	void rectangle2d(float x, float y, bool filled = true);

	std::vector<Point2D> supershape2dpoints(float a, float b, float m, float n1, float n2, float n3, int points = 100, float ang_from = 0.f, float ang_to = 2.f * PI);
	void supershape2d(float a, float b, float m, float n1, float n2, float n3, bool filled = true, int points = 100, float ang_from = 0.f, float ang_to = 2.f * PI);
};

#endif
