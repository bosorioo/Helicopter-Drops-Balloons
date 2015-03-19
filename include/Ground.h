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

#ifndef _GROUND_H
#define _GROUND_H

#include "Drawing.h"
#include <vector>

class GroundPainting
{
public:

	GroundPainting(float x, float y, float r, float g, float b, std::vector<Point2D>& points);
	void draw();
	void setFill(bool b);
	void toggleFill();

private:

	float m_x, m_y; // Position
	float m_r, m_g, m_b; // Color
	float m_rotation;
	bool m_filled;
	std::vector<Point2D> m_points;
};

class Ground
{
public:

	Ground();
	~Ground();
	void draw(bool draw_drops = true);
	void reset();
	void toggleMode();
	void toggleFill();
	void addPaintDrop(float x, float y, float r, float g, float b, bool filled = true);

private:

	void create_gl_list();
	void draw_quads();
	void draw_circles();
	void draw_paint_drops();

	float m_inner_radius;
	float m_radius_increase;
	float m_quad_size;
	float m_size;
	float m_ground_height;
	int m_gl_list;
	bool m_update_gl_list;
	bool m_quad_mode;

	std::vector<Point2D> m_paintdrop_shape;
	std::vector<GroundPainting> m_paint_drops;
};

#endif
