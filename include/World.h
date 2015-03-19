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

#ifndef _WORLD_H
#define _WORLD_H

#include "PaintBall.h"
#include "Ground.h"
#include <vector>

class Object
{
public:

	Object(float x, float y);
	void stepdraw();
	void draw();
	void update(float interval);
	void reset();
	void paint(Color c);
	void setPosition(float x, float y, bool use_drag_relation = false);
	Point2D getPosition();
	void toggleFill();
	void setMouseToggle(bool b);
	void setDrag(bool b, float relative_x = 0.f, float relative_y = 0.f);
	bool wasMouseToggled();
	bool isDragging();
	bool isCollidingWithCircle(float x, float y, float z, float radius);
	bool isMouseOver(float x, float y);

private:

	struct PaintProgress {
		PaintProgress();
		PaintProgress(const PaintProgress& pp);
		Color m_paint_color;
		float m_percentage;
	};

	int m_type;
	float m_original_x, m_original_y;
	float m_x, m_y, m_drag_x, m_drag_y;
	float m_size;
	Color m_color;
	bool m_filled, m_toggled_by_mouse, m_dragging;
	std::vector<PaintProgress> m_paint;
};

class World
{
public:

	World();

	void draw();
	void update(float interval);
	void reset();
	void addObject(float x, float y);
	void mouseMove(float x, float y);
	void mouseDrag(float x, float y);
	void mouseClick(float x, float y);
	Point2D getObjectPosition(unsigned index);
	void getHitInfo(int& hits, int& misses);
	void getPathOverObjects(std::vector<Point2D>& out_vector);

	PaintBall* addPaintBall();

	Ground& getGround();
	void toggleFill();

private:

	std::vector<PaintBall> m_paint_balls;
	std::vector<Object> m_objects;
	Ground m_ground;
	bool m_filled;
	int m_hits, m_misses;
};

#endif
