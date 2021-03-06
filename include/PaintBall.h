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

#ifndef _PAINTBALL_H
#define _PAINTBALL_H

#include "Drawing.h"

#define PAINT_BALL_GRAVITY -3.f

class PaintBall
{
public:
	PaintBall();

	void draw();
	void update(float interval);

	void toggleFill();
	void remove();
	void setPosition(float x, float y, float z);
	void setSpeed(float x, float y, float z);
	Color& getColor();
	bool isRemoved();
	float x();
	float y();
	float z();
	float radius();

private:
	float m_speed_x, m_speed_y, m_speed_z;
	float m_x, m_y, m_z;
	bool m_filled;
	bool m_removed;
	Color m_color;
};

#endif
