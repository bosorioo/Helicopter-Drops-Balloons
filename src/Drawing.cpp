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

#include "Drawing.h"
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

Point2D::Point2D() {}
Point2D::Point2D(float x, float y) : m_x(x), m_y(y) {}

void Point2D::setPoint(float x, float y)
{
	m_x = x; m_y = y;
}

float Point2D::x()
{
	return m_x;
}

float Point2D::y()
{
	return m_y;
}

Color::Color() {}
Color::Color(int r, int g, int b) { set(r/255.f, g/255.f, b/255.f); }
Color::Color(float r, float g, float b) : m_r(r), m_g(g), m_b(b) {}
Color::Color(const Color& c) { *this = c; }

void Color::glColor()
{
	glColor3f(m_r, m_g, m_b);
}
void Color::set(int r, int g, int b)
{
	m_r = r / 255.f; m_g = g / 255.f; m_b = b / 255.f;
}

void Color::set(float r, float g, float b)
{
	m_r = r; m_g = g; m_b = b;
}

void Color::set(Color c)
{
	*this = c;
}

float Color::red()
{
	return m_r;
}

float Color::green()
{
	return m_g;
}

float Color::blue()
{
	return m_b;
}

void Draw::drawpoints(std::vector<Point2D>& points, bool filled)
{
	glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_STRIP);

	if (filled)
		glVertex3f(0.f, 0.f, 0.f);

	for (std::vector<Point2D>::iterator it = points.begin(); it != points.end(); it++)
		glVertex3f(it->x(), 0.f, it->y());

	glVertex3f(points.begin()->x(), 0.f, points.begin()->y());
	glEnd();
}

void Draw::text(const char* text, float x, float y, float z, bool centered)
{
	float textWidth = glutBitmapLength(GLUT_BITMAP_9_BY_15, (unsigned char*)text);
	float xBegin = centered ? x - textWidth/2. : x;
	float yBegin = centered ? y - 5.f : y;
	glRasterPos3f(xBegin, yBegin, z);

	while (*text)
	{
		if (*text == '\n')
		{
			yBegin += 18;
			glRasterPos3f(xBegin, yBegin, z);
			text++;
		}
		else
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *text++);
	}
}

void Draw::circle2d(float radius, int points, bool filled)
{
	float angle = 0.0f;
	float angle_delta = 2 * PI / points;
	glBegin(filled ? GL_TRIANGLE_FAN : GL_LINE_STRIP);

	if (filled)
		glVertex3f(0.f, 0.f, 0.f);

	for (; points > 0; points--)
	{
		glVertex3f(std::cos(angle) * radius, 0.f, std::sin(angle) * radius);
		angle += angle_delta;
	}

	glVertex3f(radius, 0.f, 0.f);
	glEnd();
}

void Draw::square2d(float size, bool filled)
{
	size /= 2.f;
	glBegin(filled ? GL_QUADS : GL_LINE_STRIP);
	glVertex3f(-size, 0.f, -size); glVertex3f(size, 0.f, -size);
	glVertex3f(size, 0.f, size); glVertex3f(-size, 0.f, size);
	if (!filled)
		glVertex3f(-size, 0.f, -size);
	glEnd();
}

void Draw::rectangle2d(float x, float y, bool filled)
{
	x /= 2.f;
	y /= 2.f;
	glBegin(filled ? GL_QUADS : GL_LINE_STRIP);
	glVertex3f(-x, 0.f, -y); glVertex3f(x, 0.f, -y);
	glVertex3f(x, 0.f, y); glVertex3f(-x, 0.f, y);
	if (!filled)
		glVertex3f(-x, 0.f, -y);
	glEnd();
}

std::vector<Point2D> Draw::supershape2dpoints(float a, float b, float m, float n1, float n2, float n3, int points, float ang_from, float ang_to)
{
	std::vector<Point2D> result;
	float delta = (ang_to - ang_from) / points;
	float r;
	Point2D point;
	for (float t = ang_from; t <= ang_to; t += delta)
	{
		r = std::pow((std::pow(std::abs(std::cos(m * t / 4.f)), n2) +
			          std::pow(std::abs(std::sin(m * t / 4.f)), n3)), -1 / n1);
		result.push_back(Point2D(r * std::cos(t) * a, -r * std::sin(t) * b));
	}
	return result;
}

void Draw::supershape2d(float a, float b, float m, float n1, float n2, float n3, bool filled, int points, float ang_from, float ang_to)
{
	std::vector<Point2D> shapepoints = supershape2dpoints(a, b, m, n1, n2, n3, points, ang_from, ang_to);
	drawpoints(shapepoints, filled);
}




