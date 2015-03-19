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

#include "World.h"
#include "Utils.h"
#include <cstdlib>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define TRANSITION_COLOR_TIME .45f

Object::PaintProgress::PaintProgress() {}
Object::PaintProgress::PaintProgress(const PaintProgress& pp) { *this = pp; }

Object::Object(float x, float y)
{
	m_filled = true;
	m_toggled_by_mouse = false;
	m_dragging = false;
	m_x = x; m_y = y;
	m_original_x = x;
	m_original_y = y;
	m_color.set(Utils::getRandomFloat(), Utils::getRandomFloat(), Utils::getRandomFloat());
	m_type = Utils::getRandomInt(0, 1);

	if (m_type == 1)
		m_size = Utils::getRandomFloat(10.f, 26.f);
	else
		m_size = Utils::getRandomFloat(14.f, 34.f);
}

void Object::stepdraw()
{
	if (m_filled)
	{
		if (m_type == 1)
		{
			glutSolidCube(m_size);
			glColor3f(0.f, 0.f, 0.f);
			glutWireCube(m_size);
		}
		else
			glutSolidSphere(m_size / 2.f, 18, 12);
	}
	else
	{
		if (m_type == 1)
			glutWireCube(m_size);
		else
			glutWireSphere(m_size / 2.f, 18, 12);
	}
}

void Object::draw()
{
	glTranslatef(m_x, m_size / 2.f + .1f, m_y);

	if (m_paint.empty())
	{
		m_color.glColor();
		stepdraw();
	}
	else
	{
		double equation[] = {
			0.,  1., 0., 0., //m_size / 2.f - (m_size * m_paint_percent),
			0., -1., 0., 0., //- m_size / 2.f + (m_size * m_paint_percent)
		};

		glEnable(GL_CLIP_PLANE0);
		glEnable(GL_CLIP_PLANE1);

		for (unsigned i = 0; i < m_paint.size(); i++)
		{
			m_paint[i].m_paint_color.glColor();
			equation[3] = -m_size / 2.f + (m_paint[i].m_percentage * m_size);
			
			if (i + 1 < m_paint.size())
				equation[7] = m_size / 2.f - (m_paint[i + 1].m_percentage * m_size );
			else
				equation[7] = m_size / 2.f + 0.01;

			glClipPlane(GL_CLIP_PLANE0, equation);
			glClipPlane(GL_CLIP_PLANE1, equation + 4);

			stepdraw();
		}

		glDisable(GL_CLIP_PLANE0);
		equation[7] = m_size / 2.f - (m_paint.front().m_percentage * m_size);
		glClipPlane(GL_CLIP_PLANE1, equation + 4);

		m_color.glColor();
		stepdraw();

		glDisable(GL_CLIP_PLANE1);

		if (m_filled && m_type == 1)
		{
			glColor3f(0.f, 0.f, 0.f);
			glutWireCube(m_size);
		}
	}

	glTranslatef(-m_x, -m_size / 2.f - .1f, -m_y);
}

void Object::update(float interval)
{
	std::vector<PaintProgress>::iterator it = m_paint.begin();
	while (it != m_paint.end())
	{
		it->m_percentage += TRANSITION_COLOR_TIME * interval;
		if (it->m_percentage >= 1.f)
		{
			m_color = it->m_paint_color;
			it = m_paint.erase(it);
			continue;
		}
		it++;
	}
}

void Object::reset()
{
	m_paint.clear();
	new (this) Object(m_original_x, m_original_y);
}

void Object::paint(Color c)
{
	PaintProgress pp;
	pp.m_paint_color = c;
	pp.m_percentage = 0.f;

	m_paint.push_back(pp);
}

void Object::setPosition(float x, float y, bool use_drag_relation)
{
	m_x = x;
	m_y = y;

	if (use_drag_relation)
	{
		m_x -= m_drag_x;
		m_y -= m_drag_y;
	}
}

Point2D Object::getPosition()
{
	return Point2D(m_x, m_y);
}

void Object::toggleFill()
{
	m_filled = !m_filled;
}

void Object::setMouseToggle(bool b)
{
	m_toggled_by_mouse = b;
}

void Object::setDrag(bool b, float relative_x, float relative_y)
{
	m_dragging = b;
	m_drag_x = relative_x;
	m_drag_y = relative_y;
}

bool Object::wasMouseToggled()
{
	return m_toggled_by_mouse;
}

bool Object::isDragging()
{
	return m_dragging;
}

bool Object::isCollidingWithCircle(float x, float y, float z, float radius)
{
	if (m_type == 1)
	return (y - radius <= m_size) &&
		((x >= m_x - m_size / 2.f && x <= m_x + m_size / 2.f) ||
		 (x + radius >= m_x - m_size / 2.f && x + radius <= m_x + m_size / 2.f) ||
		 (x - radius >= m_x - m_size / 2.f && x - radius <= m_x + m_size / 2.f)
		) &&
		((z >= m_y - m_size / 2.f && z <= m_y + m_size / 2.f) ||
		 (z + radius >= m_y - m_size / 2.f && z + radius <= m_y + m_size / 2.f) ||
		 (z - radius >= m_y - m_size / 2.f && z - radius <= m_y + m_size / 2.f)
		);

	return Utils::pointDistance(m_x, m_size / 2.f, m_y, x, y, z) <= radius + m_size / 2.f;
}

bool Object::isMouseOver(float x, float y)
{
	if (m_type == 1)
	{
		return x >= m_x - m_size / 2.f && x <= m_x + m_size / 2.f &&
			y >= m_y - m_size / 2.f && y <= m_y + m_size / 2.f;
	}

	return Utils::pointDistance(x, y, m_x, m_y) <= m_size / 2.f;
}

World::World()
{
	m_filled = true;
	m_hits = m_misses = false;
}

void World::reset()
{
	m_hits = m_misses = false;
	m_ground.reset();
	for (std::vector<Object>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
		it->reset();
}

void World::addObject(float x, float y)
{
	m_objects.push_back(Object(x, y));
}

void World::mouseMove(float x, float y)
{
	for (std::vector<Object>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		if (it->isMouseOver(x, y))
		{
			if (!it->wasMouseToggled())
				it->toggleFill();

			it->setMouseToggle(true);
		}
		else
			it->setMouseToggle(false);
	}
}

void World::mouseDrag(float x, float y)
{
	for (std::vector<Object>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		if (it->isDragging())
			it->setPosition(x, y, true);
	}
}

void World::mouseClick(float x, float y)
{
	bool disabled = false;

	for (std::vector<Object>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		it->setDrag(false);

		if (disabled)
			continue;

		if (it->isMouseOver(x, y))
		{
			Point2D pos = it->getPosition();
			it->setDrag(true, x - pos.x(), y - pos.y());
			disabled = true;
		}
	}
}

Point2D World::getObjectPosition(unsigned index)
{
	if (index < 0 || index >= m_objects.size())
		return Point2D(0.f, 0.f);

	return m_objects[index].getPosition();
}

void World::getHitInfo(int& hits, int& misses)
{
	hits = m_hits;
	misses = m_misses;
}

void World::getPathOverObjects(std::vector<Point2D>& out_vector)
{
	out_vector.clear();
	for (std::vector<Object>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
		out_vector.push_back(it->getPosition());
}

PaintBall* World::addPaintBall()
{
	m_paint_balls.push_back(PaintBall());

	if (!m_filled)
		m_paint_balls.back().toggleFill();

	return &m_paint_balls.back();
}

Ground& World::getGround()
{
	return m_ground;
}

void World::toggleFill()
{
	m_filled = !m_filled;
	m_ground.toggleFill();
}

void World::draw()
{
	m_ground.draw();

	std::vector<PaintBall>::iterator it = m_paint_balls.begin();
	while (it != m_paint_balls.end())
	{
		if (!it->isRemoved())
			it->draw();

		it++;
	};

	for (std::vector<Object>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
	{
		it->draw();
	}
}

void World::update(float interval)
{
	std::vector<PaintBall>::iterator it = m_paint_balls.begin();

	while (it != m_paint_balls.end())
	{
		if (it->isRemoved())
		{
			it = m_paint_balls.erase(it);
			continue;
		}

		it->update(interval);

		if (it->y() <= 0.f)
		{
			it->remove();
			Color c = it->getColor();
			m_ground.addPaintDrop(it->x(), it->z(), c.red(), c.green(), c.blue(), m_filled);
			m_misses++;
		}
		else
		{
			for (std::vector<Object>::iterator it_obj = m_objects.begin(); it_obj != m_objects.end(); it_obj++)
			{
				if (it_obj->isCollidingWithCircle(it->x(), it->y(), it->z(), it->radius()))
				{
					it_obj->paint(it->getColor());
					it_obj->toggleFill();
					it->remove();
					m_hits++;
				}
			}
		}

		it++;
	};

	for (std::vector<Object>::iterator it_obj = m_objects.begin(); it_obj != m_objects.end(); it_obj++)
	{
		it_obj->update(interval);
	}
}