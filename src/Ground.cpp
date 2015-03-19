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

#include "Ground.h"
#include "Drawing.h"
#include "Utils.h"
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

GroundPainting::GroundPainting(float x, float y, float r, float g, float b, std::vector<Point2D>& points) :
	m_points(points)
{
	m_x = x; m_y = y;
	m_r = r; m_g = g; m_b = b;
	m_rotation = Utils::getRandomFloat(0.f, 360.f);
	m_filled = true;
}

void GroundPainting::draw()
{
	glColor3f(m_r, m_g, m_b);
	glPushMatrix();
		glTranslatef(m_x, 0.f, m_y);
		glRotatef(m_rotation, 0.f, 1.f, 0.f);
		Draw::drawpoints(m_points, m_filled);
	glPopMatrix();
}

void Ground::reset()
{
	m_paint_drops.clear();
}

void GroundPainting::setFill(bool b)
{
	m_filled = b;
}

void GroundPainting::toggleFill()
{
	m_filled = !m_filled;
}

Ground::Ground()
{
	m_inner_radius = 10.f;
	m_radius_increase = 20.f;
	m_quad_size = 16.f;
	m_size = 336.f;
	m_gl_list = -1;
	m_ground_height = 0.f;
	m_update_gl_list = true;
	m_quad_mode = false;
	m_paintdrop_shape = Draw::supershape2dpoints(2.2f, 1.8f, 9.f, 2.f, 7.5f, 2.5f);
}

Ground::~Ground()
{
	//if (m_gl_list > 0)
	//	glDeleteLists(m_gl_list, 1);
}

void Ground::draw(bool draw_drops)
{
	if (m_update_gl_list)
	{
		m_update_gl_list = false;
		create_gl_list();
	}

	if (m_gl_list > 0)
	{
		glCallList(m_gl_list);
		draw_paint_drops();
		return;
	}

	if (m_quad_mode)
		draw_quads();
	else
		draw_circles();

	if (draw_drops)
		draw_paint_drops();
}

void Ground::toggleMode()
{
	m_quad_mode = !m_quad_mode;
	m_update_gl_list = true;
}

void Ground::toggleFill()
{
	for (std::vector<GroundPainting>::iterator it = m_paint_drops.begin(); it != m_paint_drops.end(); it++)
		it->toggleFill();

}

void Ground::addPaintDrop(float x, float y, float r, float g, float b, bool filled)
{
	m_paint_drops.push_back(GroundPainting(x, y, r, g, b, m_paintdrop_shape));
	m_paint_drops.back().setFill(filled);
}

void Ground::draw_quads()
{
	glPushMatrix();
	glColor3f(0.545f, 0.27f, 0.07f);
	Draw::square2d(m_size);
	glColor3f(0.f, 0.f, 0.f);
	glTranslatef(-m_size / 2.f + m_quad_size / 2.f, 0.01f, -m_size / 2.f + m_quad_size / 2.f);
	m_ground_height = 0.02f;
	for (unsigned i = 0; i < m_size / m_quad_size; i++)
	{
		for (unsigned j = 0; j < m_size / m_quad_size; j++)
		{
			//Draw::rectangle2d(m_quad_size, false);
			glTranslatef(0.f, 0.f, m_quad_size);
		}
		glTranslatef(m_quad_size, 0.f, -m_size);
	}
	glPopMatrix();
}

void Ground::draw_circles()
{
	float biggestRadius = m_inner_radius;
	float increaseRadius = m_radius_increase;
	unsigned circles = 1;

	while (biggestRadius + increaseRadius < m_size / 2)
	{
		biggestRadius += increaseRadius;
		increaseRadius += m_radius_increase;
		circles++;
	}

	float colors[] = {
		0.4f, 0.8f, 1.f,
		0.f, 0.f, 1.f,
		.4f, 0.4f, 1.f,
		.7f, .7f, 1.f
	};

	int colorsNumber = sizeof(colors) / sizeof(float) / 3;

	glPushMatrix();

	m_ground_height = (circles + 1) * 0.01f;

	while (circles > 0)
	{
		int colorIndex = circles % colorsNumber;
		glColor3f(colors[colorIndex * 3], colors[colorIndex * 3 + 1], colors[colorIndex * 3 + 2]);
		Draw::circle2d(biggestRadius, 60 * circles, true);
		increaseRadius -= m_radius_increase;
		biggestRadius -= increaseRadius;
		circles--;
		if (circles == 1)
			biggestRadius = m_inner_radius;
		glTranslatef(0.f, 0.01f, 0.f);
	}

	glPopMatrix();
}

void Ground::create_gl_list()
{
	if (m_gl_list > 0)
	{
		glDeleteLists(m_gl_list, 1);
		m_gl_list = 0;
	}
	
	int newList = glGenLists(1);

	if (!newList)
	{
		std::cout << "glGenLists for ground failed.\n";
		return;
	}

	glNewList(newList, GL_COMPILE);
	draw(false);
	glEndList();

	m_gl_list = newList;
}

void Ground::draw_paint_drops()
{
	glTranslatef(0.f, m_ground_height, 0.f);
	for (std::vector<GroundPainting>::iterator it = m_paint_drops.begin(); it != m_paint_drops.end(); it++)
		it->draw();
	glTranslatef(0.f, -m_ground_height, 0.f);
}