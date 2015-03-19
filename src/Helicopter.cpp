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

#include "Helicopter.h"
#include "Utils.h"
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

Helicopter::Helicopter()
{
	m_filled = true;
	m_accelerating = false;
	m_infinite_balloons = false;
	m_draw_shadow = true;
	m_draw_path = false;
	m_propeller_drag = 0;
	m_x = m_y = m_z = m_goal_height = 0.f;
	m_spin_speed = m_spin_angle = m_spin_speed_goal = 0.f;
	m_update_tick = 0.f;
	m_red = 1.0f; m_green = m_blue = 0.f;
	m_ws_red = m_ws_green = m_ws_blue = 0.6f;
	m_state = m_next_state = OFF;
	m_paint_balls = 0;
	m_balls_colors.clear();
	m_x_rot = m_y_rot = m_z_rot = 0.f;
	m_goal_x = 0.f;
	m_goal_y = 0.f;
	m_orders.clear();
}

void Helicopter::draw(bool shadow, bool picking)
{
	if (shadow)
		glColor3f(0.f, 0.f, 0.f);

	if (!picking) // Draw balloons only if not picking
		draw_balloons(shadow);

	glPushMatrix();

	glTranslatef(m_x, m_y + BASE_HEIGHT, m_z); // Height of the helicopter

	glRotatef(m_x_rot, 1.f, 0.f, 0.f);
	glRotatef(m_y_rot, 0.f, 1.f, 0.f);
	glRotatef(m_z_rot, 0.f, 0.f, 1.f);

#define CLIP_COS 0.927f
#define CLIP_SIN 0.374f

	static double clip_equation[] = {
		CLIP_COS, -CLIP_SIN, 0., 1.34, // For body
		-CLIP_COS, CLIP_SIN, 0., -1.34, // For windshield
		CLIP_COS, -CLIP_SIN, 0., 1.36, // For windshield contour
		-CLIP_COS, CLIP_SIN, 0., -1.32 // For windshield contour
	};

	if (picking)
	{
		glScalef(2.f, .8f, 1.f);
		glutSolidSphere(2.0, 15, 6);
	}
	else
	{
		// Start draw windshield
		glEnable(GL_CLIP_PLANE0);
		if (!shadow)
			glColor3f(m_ws_red, m_ws_green, m_ws_blue);
		glScalef(2.f, .8f, 1.f);
		glClipPlane(GL_CLIP_PLANE0, clip_equation + 4);

		if (m_filled)
			glutSolidSphere(2.0, 30, 12);
		else
			glutWireSphere(2.0, 30, 12);
		// Finish drawing windshield

		// Start draw main body
		if (!shadow) // Keep color in black if in shadow mode, otherwise set main body color
			glColor3f(m_red, m_green, m_blue);

		glClipPlane(GL_CLIP_PLANE0, clip_equation);

		if (m_filled)
			glutSolidSphere(2.0, 20, 12);
		else
			glutWireSphere(2.0, 20, 12);
		// Finish drawing main body
	
		// Start drawing windshield contour
		glColor3f(0, 0, 0);

		glEnable(GL_CLIP_PLANE1);
		glClipPlane(GL_CLIP_PLANE0, clip_equation + 8);
		glClipPlane(GL_CLIP_PLANE1, clip_equation + 12);
		glutSolidSphere(2.0, 30, 12);
		glDisable(GL_CLIP_PLANE1);
		glDisable(GL_CLIP_PLANE0);
		// Finish drawing windshield contour
	}

	glColor3f(0, 0, 0);

	// Start drawing tail
	glPushMatrix();
	glTranslatef(3.4f, 0.f, 0.f);
	glScalef(3.2f, .75f, .35f);
	if (m_filled)
		glutSolidCube(1.);
	else
		glutWireCube(1.);
	glPopMatrix();
	// End tail

	glTranslatef(0, 2.5f, 0);
	glScalef(0.2f, 1.f, 0.3f);

	// Drawing of the part that connects the propellers
	if (!picking) // Unnecessary to draw it during the picking
	{
		glRotatef(90.f, 1, 0, 0);

		if (m_filled)
			glutSolidCube(1.);
		else
			glutWireCube(1.);

		if (m_filled)
			glutSolidTorus(.5, .8f, 10, 10);
		else
			glutWireTorus(.5, 0.8f, 10, 10);

		glRotatef(-90.f, 1, 0, 0);
	}

	// End of the part that connects the propellers

	glTranslatef(25.f, -1.9f, 0.f); // For drawing upper/lower parts of the end of the tail

	// Starting drawing upper part of the tail
	glPushMatrix();
	glRotatef(32.f, 0, 0, 1);
	glScalef(3.1f, 0.7f, 1.2f);
	if (m_filled)
		glutSolidCube(1.);
	else
		glutWireCube(1.);
	glPopMatrix();
	// End upper part of the tail

	// Starting drawing lower part of the tail
	glPushMatrix();
	glTranslatef(.0f, -1.4f, 0.f);
	glRotatef(160.f, 0, 0, 1);
	glScalef(2.2f, 0.4f, 1.2f);
	if (m_filled)
		glutSolidCube(1.);
	else
		glutWireCube(1.);
	glPopMatrix();
	// End lower part of the tail

	glPopMatrix();

	draw_propellers(shadow, picking);

	if (!shadow && !picking && m_draw_shadow)
	// If not drawing shadow, draw shadow right after rendering helicopter
	// but only if not picking. During picking, the shadow is unwanted.
	{
		glPushMatrix();
		glTranslatef(0.f, .1f, 0.f);
		glScalef(1.f, 0.f, 1.f);
		draw(true);
		glPopMatrix();
	}

	if (!shadow && !picking && m_draw_path)
		draw_path();
}

void Helicopter::update(float interval)
{
	m_update_tick += interval;
	update_spin(interval);
	update_height(interval);
	update_position(interval);
	update_orders();
}

void Helicopter::reset()
{
	new (this) Helicopter;
}

void Helicopter::toggleFill()
{
	m_filled = !m_filled;
}

void Helicopter::power(bool on)
{
	if (!on)
	{
		if (m_y > 0.f)
			return;

		m_state = OFF;
		m_accelerating = true;
		m_spin_speed_goal = 0.f;
		m_goal_height = 0.f;
	}
	else
	{
		m_state = IDLE;
		m_spin_speed_goal = LIFITING_ROTATION;
		m_accelerating = true;
	}
}

void Helicopter::reachHeight(float height)
{
	if (m_state == OFF)
		return;

	m_goal_height = height;
}

void Helicopter::reachPosition(float x, float z)
{
	m_goal_x = x;
	m_goal_y = z;
}

void Helicopter::flyPath(std::vector<Point2D>& path)
{
	if (m_state == OFF)
		return;

	m_orders.clear();
	setFlyPoints(path);

	struct Order order;

	if (m_y < MOVE_HEIGHT)
	{
		order.state = FLYING_VERTICAL;
		order.arg1 = Utils::bitCopy<int>(FLY_HEIGHT);
		m_orders.push_back(order);
	}

	order.state = MOVING;
	order.ignore = false;

	for (std::vector<Point2D>::iterator it = path.begin(); it != path.end(); it++)
	{ 
		order.arg1 = Utils::bitCopy<int>(it->x());
		order.arg2 = Utils::bitCopy<int>(it->y());
		m_orders.push_back(order);
	}

	order.arg1 = Utils::bitCopy<int>(0.f);
	order.arg2 = Utils::bitCopy<int>(0.f);
	m_orders.push_back(order);

	order.state = LANDING;
	m_orders.push_back(order);
}

void Helicopter::setFlyPoints(std::vector<Point2D>& path)
{
	m_fly_path = path;
}

void Helicopter::land()
{
	if (m_state == OFF)
		return;

	m_orders.clear();

	struct Order order;

	order.state = MOVING;
	order.ignore = false;
	order.arg1 = Utils::bitCopy<int>(0.f);
	order.arg2 = Utils::bitCopy<int>(0.f);

	if (m_x != 0.f || m_z != 0.f)
		m_orders.push_back(order);

	order.state = FLYING_VERTICAL;
	order.arg1 = Utils::bitCopy<int>(0.f);

	if (m_y > 0.f)
		m_orders.push_back(order);

	order.state = LANDING;
	m_orders.push_back(order);
}

void Helicopter::setPaintBallsCount(int count)
{
	count = Utils::limit(0, count, 6);

	if (count > m_paint_balls)
	{
		for (int current = m_paint_balls; current < count; current++)
			m_balls_colors.push_back(Color(Utils::getRandomFloat(), Utils::getRandomFloat(), Utils::getRandomFloat()));
	}
	else
		m_balls_colors.resize(count);

	m_paint_balls = count;
}

void Helicopter::setInfiniteBalloons(bool b)
{
	m_infinite_balloons = b;
}

void Helicopter::showShadow(bool b)
{
	m_draw_shadow = b;
}

void Helicopter::sideCamera_gluLookAt()
{
	#define SIDE_DISTANCE 6.f
	#define FRONT_DISTANCE 14.f
	
	float xChange =  SIDE_DISTANCE * std::cos(Utils::toRad(m_y_rot + 40.f));
	float zChange = -SIDE_DISTANCE * std::sin(Utils::toRad(m_y_rot + 40.f));
	float frontxChange = FRONT_DISTANCE * std::cos(Utils::toRad(m_y_rot + 180.f));
	float frontzChange = -FRONT_DISTANCE * std::sin(Utils::toRad(m_y_rot + 180.f));
	gluLookAt(m_x + xChange, m_y + BASE_HEIGHT * 1.8f, m_z + zChange, m_x + frontxChange, m_y, m_z + frontzChange, 0.f, 1.f, 0.f);
}

void Helicopter::backCamera_gluLookAt()
{
	float xChange = 30.f * std::cos(Utils::toRad(m_y_rot));
	float zChange = 30.f * std::sin(Utils::toRad(m_y_rot));
	gluLookAt(m_x + xChange, m_y + BASE_HEIGHT * 5.f, m_z - zChange, m_x, m_y, m_z, 0.f, 1.f, 0.f);
}

void Helicopter::toggleDrawPath()
{
	m_draw_path = !m_draw_path;
}

bool Helicopter::isShadowed()
{
	return m_draw_shadow;
}

bool Helicopter::dropPaintBall(PaintBall* out)
{
	if (m_infinite_balloons)
	{
		if (out)
		{
			int r = Utils::getRandomInt(0, 255);
			int g = Utils::getRandomInt(0, 255);
			int b = Utils::getRandomInt(0, 255);
			out->setPosition(m_x, m_y - .3f + BASE_HEIGHT / 2.f, m_z);
			out->getColor().set(r, g, b);
			if (m_state == MOVING)
				out->setSpeed(std::cos(Utils::toRad(m_y_rot + 180.f)) * MOVE_SPEED, 0.f, -std::sin(Utils::toRad(m_y_rot + 180.f)) * MOVE_SPEED);
		}

		return true;
	}

	if (m_paint_balls <= 0)
		return false;
	
	m_paint_balls--;

	if (out)
	{
		out->setPosition(m_x, m_y - .3f + BASE_HEIGHT / 2.f, m_z);
		out->getColor().set(m_balls_colors.back());
		if (m_state == MOVING)
			out->setSpeed(std::cos(Utils::toRad(m_y_rot + 180.f)) * MOVE_SPEED, 0.f, -std::sin(Utils::toRad(m_y_rot + 180.f)) * MOVE_SPEED);
	}

	m_balls_colors.erase(m_balls_colors.begin() + m_paint_balls);

	return true;
}

bool Helicopter::isPowerOn()
{
	return m_state != OFF;
}

bool Helicopter::isGoingUp()
{
	return m_goal_height != 0.f;
}

bool Helicopter::isFilled()
{
	return m_filled;
}

bool Helicopter::isOnGround()
{
	return m_y < 0.01f;
}

int Helicopter::getBalloons()
{
	if (m_infinite_balloons)
		return 999;

	return m_paint_balls;
}

int Helicopter::getRPMPercentage()
{
	return m_spin_speed / LIFITING_ROTATION * 100;
}

void Helicopter::update_spin(float interval)
{
	if (m_accelerating)
	{
		if (m_spin_speed < m_spin_speed_goal)
		{
			m_spin_speed += (m_spin_speed / SPIN_ACCEL / 2.f + 1) * SPIN_ACCEL * interval;
			if (m_spin_speed >= m_spin_speed_goal)
			{
				m_spin_speed = m_spin_speed_goal;
				m_accelerating = false;
			}
		}
		else
		{
			if (m_spin_speed >= LIFITING_ROTATION / 2.f)
				m_spin_speed -= (m_spin_speed / SPIN_ACCEL / 2.f + 1) * SPIN_ACCEL * interval;
			else
				m_spin_speed -= (m_spin_speed / SPIN_ACCEL / 3.f + 1) * SPIN_ACCEL * interval / 2.5f;

			if (m_spin_speed <= m_spin_speed_goal)
			{
				m_spin_speed = m_spin_speed_goal;
				m_accelerating = false;
			}
		}
	}

	float angle_change = m_spin_speed * interval * 2;

	if (angle_change > DRAG_CONSTANT)
	{
		m_propeller_drag = angle_change / DRAG_CONSTANT;
		angle_change = std::min(std::max(angle_change, 17.f), 44.f);
	}
	else
		m_propeller_drag = 0;

	m_spin_angle += angle_change;

	if (m_spin_angle >= 720.f)
		m_spin_angle -= 720.f;
}

void Helicopter::update_height(float interval)
{
	if (m_state == OFF || std::abs(m_y - m_goal_height) < 0.1f)
		return;

	float height_change = 0.f;

	if (m_spin_speed >= LIFITING_ROTATION)
		height_change = FLY_UP_SPEED * interval * Utils::signalFromBool(m_goal_height > m_y);

	if (std::abs(m_y - m_goal_height) < height_change)
	{
		m_y = m_goal_height;
		if (m_goal_height == 0.f && m_state == LAND_AND_POWER_OFF)
		{
			m_state = OFF;
			m_spin_speed_goal = 0.f;
			m_accelerating = true;
		}
	}
	else
		m_y += height_change;

	if (m_y <= 0.f)
		m_y = 0.f;
}

void Helicopter::update_position(float interval)
{
	if (getRPMPercentage() < .95f || m_y < MOVE_HEIGHT ||
		(m_x == m_goal_x && m_z == m_goal_y))
	{
		if (m_z_rot > 0.f)
			m_z_rot = std::max(0.f, m_z_rot - ORIENTATION_TRANSITION * interval);

		return;
	}

	if (Utils::pointDistance(m_x, m_z, m_goal_x, m_goal_y) <= MOVING_ORIENTATION_ANGLE / ORIENTATION_TRANSITION * MOVE_SPEED &&
		(m_orders.size() <= 1 || (m_orders.begin() + 1)->state != MOVING))
		m_z_rot = std::max(0.f, m_z_rot - ORIENTATION_TRANSITION * interval);
	else
		m_z_rot = std::min(MOVING_ORIENTATION_ANGLE, m_z_rot + ORIENTATION_TRANSITION * interval);

	float goalAngle = Utils::degreeRange(Utils::toDeg(Utils::pointAngle(m_x, -m_z, m_goal_x, -m_goal_y)));
	float myAngle = Utils::degreeRange(m_y_rot - 180.f); // Helicopter points to the -x direction, so correct that
	bool clockwise = Utils::degreeRange(myAngle - goalAngle) < 180.f;
	float deltaAngle = clockwise ? -DIRECTION_TRANSITION * interval : DIRECTION_TRANSITION * interval;
	
	if (Utils::degreeRange(goalAngle - myAngle) <= std::abs(deltaAngle) * 2.f)
		myAngle = goalAngle;
	else
		myAngle += deltaAngle;

	m_y_rot = std::floor(myAngle + 180.f);

	if (Utils::pointDistance(m_x, m_z, m_goal_x, m_goal_y) / MOVE_SPEED <= std::abs((goalAngle - myAngle) / DIRECTION_TRANSITION))
		return;

	float deltaX = std::cos(Utils::toRad(myAngle)) * MOVE_SPEED * interval;
	float deltaY = -std::sin(Utils::toRad(myAngle)) * MOVE_SPEED * interval;

	if (Utils::pointDistance(m_x, m_z, m_goal_x, m_goal_y) <= MOVE_SPEED * interval)
	{
		m_x = m_goal_x;
		m_z = m_goal_y;
	}
	else
	{
		m_x += deltaX;
		m_z += deltaY;
	}
}

void Helicopter::update_orders()
{
	if (m_state == OFF)
		return;

	if (m_orders.empty())
	{
		m_state = IDLE;
		return;
	}

	struct Order currentOrder = m_orders.front();
	m_state = currentOrder.state;
	switch (currentOrder.state)
	{
	case MOVING:

		if (currentOrder.ignore)
		{
			if (m_x == m_goal_x && m_z == m_goal_y)
				m_orders.erase(m_orders.begin());
			break;
		}

		reachPosition(
			Utils::bitCopy<float>(currentOrder.arg1),
			Utils::bitCopy<float>(currentOrder.arg2)
		);

		m_orders.front().ignore = true;

		break;

	case LANDING:

		if (m_x != 0.f || m_z != 0.f)
		{
			currentOrder.ignore = false;
			currentOrder.state = MOVING;
			currentOrder.arg1 = Utils::bitCopy<int>(0.f);
			currentOrder.arg2 = Utils::bitCopy<int>(0.f);
			m_orders.insert(m_orders.begin(), currentOrder);
			return;
		}

		if (m_y <= 0.f)
			m_orders.erase(m_orders.begin());
		else
			reachHeight(0.f);
		break;

	case FLYING_VERTICAL:
		{
			float height = Utils::bitCopy<float>(currentOrder.arg1);
			if (std::abs(m_y - height) > 0.1)
				reachHeight(height);
			else
				m_orders.erase(m_orders.begin());
		}
		break;

	default:
		break;
	}
}

void Helicopter::draw_propellers(bool shadow, bool picking)
{
	#define PROPELLER_LENGTH 7.f
	#define BACK_PROPELLER_LENGTH 1.f

	glPushMatrix();

	if (picking) // For picking, do a much simpler rendering
	{
		glTranslatef(m_x, m_y + BASE_HEIGHT + 2.3f, m_z); // Move propellers along with helicopter height
		Draw::circle2d(PROPELLER_LENGTH, 10, true);
		glPopMatrix();
		return;
	}

	glTranslatef(m_x, m_y + BASE_HEIGHT, m_z); // Move propellers along with helicopter height

	glRotatef(m_x_rot, 1.f, 0.f, 0.f);
	glRotatef(m_y_rot, 0.f, 1.f, 0.f);
	glRotatef(m_z_rot, 0.f, 0.f, 1.f);

	if (!shadow)
		glColor3f(m_ws_red, m_ws_green, m_ws_blue);

	int dragging = Utils::limit(0, m_propeller_drag, 24);

	for (int i = 0; i < 8; i++)
	{
		glPushMatrix();

		if (i < 4)
		{
			glTranslatef(0, 2.3f, 0.f);
			glRotated(90 * i + m_spin_angle, 0, 1, 0);
			glTranslatef(PROPELLER_LENGTH / 2 + 0.4f, 0, 0);
			glScalef(PROPELLER_LENGTH, 0.1f, .6f);
		}
		else
		{
			glTranslatef(9.5f, 0.f, -.3f);
			glRotated(90 * i - m_spin_angle, 0, 0, 1);
			glTranslatef(BACK_PROPELLER_LENGTH / 2 + 0.1f, 0, 0);
			glScalef(BACK_PROPELLER_LENGTH, 0.3f, .1f);
		}

		if (m_filled)
		{
			glutSolidCube(1.);

			if (!m_propeller_drag)
			{
				if (!shadow)
					glColor3f(0, 0, 0);
				glutWireCube(1.);
				if (!shadow)
					glColor3f(m_ws_red, m_ws_green, m_ws_blue);
			}
		}
		else
		{
			glutWireCube(1.);
		}

		if (dragging)
		{
			glPopMatrix();

			for (int j = -dragging; j < dragging; j++)
			{
				glPushMatrix();

				if (i < 4)
				{
					glTranslatef(0, 2.3f, 0.f);
					glRotated(90 * i + m_spin_angle - j * DRAG_ANGLE, 0, 1, 0);
					glTranslatef(PROPELLER_LENGTH / 2 + 0.4f, 0, 0);
					glScalef(PROPELLER_LENGTH, 0.1f, .6f);
				}
				else
				{
					glTranslatef(9.5f, 0.f, -.3f);
					glRotated(90 * i - m_spin_angle + j * DRAG_ANGLE, 0, 0, 1);
					glTranslatef(BACK_PROPELLER_LENGTH / 2 + 0.1f, 0, 0);
					glScalef(BACK_PROPELLER_LENGTH, 0.3f, .1f);
				}

				if (m_filled)
					glutSolidCube(1.);
				else
					glutWireCube(1.);

				glPopMatrix();
			}
		}
		else
			glPopMatrix();
	}

	glPopMatrix();
}

void Helicopter::draw_balloons(bool shadow)
{
	if (m_infinite_balloons) // If in infinite mode, don't draw ballons.
		return;

	PaintBall ball;

	if (!m_filled)
		ball.toggleFill();

	float x, y, z;

	for (int i = 0; i < m_paint_balls; i++)
	{
		glPushMatrix();
		get_balloon_position(i, x, y, z);
		ball.setPosition(x, y, z);
		glTranslatef(m_x, m_y, m_z);
		glRotatef(m_x_rot, 1.f, 0.f, 0.f);
		glRotatef(m_y_rot, 0.f, 1.f, 0.f);
		glRotatef(m_z_rot, 0.f, 0.f, 1.f);
		glTranslatef(-m_x, -m_y, -m_z);

		if (shadow)
			ball.getColor().set(0.f, 0.f, 0.f);
		else
			ball.getColor().set(m_balls_colors[i]);
		ball.draw();
		glPopMatrix();
	}
}

void Helicopter::draw_path()
{
	glEnable(GL_LINE_STIPPLE);
	glLineWidth(14.f);
	glLineStipple(32, 0xAAAA);

	glColor3f(0.f, 1.f, 0.f);

	float delta_y = PATH_HEIGHT / (m_fly_path.size() + 2);
	float y = 0.f;

	glBegin(GL_LINE_STRIP);
	glVertex3f(0.f, y, 0.f);
	y += delta_y;

	for (std::vector<Point2D>::iterator it = m_fly_path.begin(); it != m_fly_path.end(); it++)
	{	
		glVertex3f(it->x(), y, it->y());
		y += delta_y;
	}

	glVertex3f(0.f, y, 0.f);
	glEnd();

	glDisable(GL_LINE_STIPPLE);
	glLineWidth(1.f);
}

void Helicopter::get_balloon_position(int index, float& x, float& y, float& z)
{
	z = m_z + 1.f * Utils::signalFromBool((index % 2) == 0);
	x = m_x + ((index / 2L) - 1) * 1.2f;
	y = m_y + 1.4f;
}