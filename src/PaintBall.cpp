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

#include "PaintBall.h"
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PAINT_BALL_RADIUS .75f

PaintBall::PaintBall() : m_color(1.f, 1.f, 1.f)
{
	m_filled = true;
	m_removed = false;
	m_x = m_y = m_z = 0.f;
	m_speed_x = m_speed_y = m_speed_z = 0.f;
}

void PaintBall::draw()
{
	m_color.glColor();
	glTranslatef(m_x, m_y, m_z);

	if (m_filled)
		glutSolidSphere(PAINT_BALL_RADIUS, 10, 10);
	else
		glutWireSphere(PAINT_BALL_RADIUS, 10, 10);

	glTranslatef(-m_x, -m_y, -m_z);
}

void PaintBall::update(float interval)
{
	float current_height = m_y;

	m_speed_y += PAINT_BALL_GRAVITY;

	m_x += m_speed_x * interval;
	m_y += m_speed_y * interval;
	m_z += m_speed_z * interval;

	if (m_y <= PAINT_BALL_RADIUS / 2.f)
	{
		float ratio = 1.f - current_height / (current_height + std::abs(m_y));
		m_x -= m_speed_x * interval * ratio;
		m_z -= m_speed_z * interval * ratio;
		m_speed_x = m_speed_y = m_speed_z = m_y = 0.f;
	}
}

void PaintBall::toggleFill()
{
	m_filled = !m_filled;
}

void PaintBall::remove()
{
	m_removed = true;
}

void PaintBall::setPosition(float x, float y, float z)
{
	m_x = x; m_y = y; m_z = z;
}

void PaintBall::setSpeed(float x, float y, float z)
{
	m_speed_x = x; m_speed_y = y; m_speed_z = z;
}

Color& PaintBall::getColor()
{
	return m_color;
}

bool PaintBall::isRemoved()
{
	return m_removed;
}

float PaintBall::x()
{
	return m_x;
}

float PaintBall::y()
{
	return m_y;
}

float PaintBall::z()
{
	return m_z;
}

float PaintBall::radius()
{
	return PAINT_BALL_RADIUS;
}
