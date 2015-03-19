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

#ifndef _HELICOPTER_H
#define _HELICOPTER_H

#define SPIN_ACCEL 70.f
#define DRAG_CONSTANT 33.f
#define DRAG_ANGLE 2.f
#define LIFITING_ROTATION 4000.f
#define BASE_HEIGHT 2.6f
#define FLY_HEIGHT 35.f
#define MOVE_HEIGHT 30.f
#define FLY_UP_SPEED 18.F
#define MOVE_SPEED 34.f
#define ORIENTATION_TRANSITION 12.f
#define DIRECTION_TRANSITION 85.f
#define MOVING_ORIENTATION_ANGLE 25.f
#define PATH_HEIGHT 35.f

#include <vector>
#include "Drawing.h"
#include "PaintBall.h"

class Helicopter
{
public:

	Helicopter();
	void draw(bool shadow = false, bool picking = false);
	void update(float interval);
	void reset();
	void toggleFill();
	void power(bool on = true);
	void reachHeight(float height);
	void reachPosition(float x, float z);
	void flyPath(std::vector<Point2D>& path);
	void setFlyPoints(std::vector<Point2D>& path);
	void land();
	void setPaintBallsCount(int count);
	void setInfiniteBalloons(bool b);
	void showShadow(bool b);
	void sideCamera_gluLookAt();
	void backCamera_gluLookAt();
	void toggleDrawPath();
	bool isShadowed();
	bool dropPaintBall(PaintBall* out);
	bool isPowerOn();
	bool isGoingUp();
	bool isFilled();
	bool isOnGround();
	int getBalloons();
	int getRPMPercentage();

private:

	enum States {
		MOVING,
		LANDING,
		LAND_AND_POWER_OFF,
		IDLE,
		FLYING_VERTICAL,
		OFF
	};

	struct Order {
		int arg1, arg2;
		States state;
		bool done, ignore;
	};

	void update_spin(float interval);
	void update_height(float interval);
	void update_position(float interval);
	void update_orders();
	void draw_propellers(bool shadow = false, bool picking = false);
	void draw_balloons(bool shadow = false);
	void draw_path();
	void get_balloon_position(int index, float& x, float& y, float& z);

	std::vector<Point2D> m_fly_path;
	std::vector<struct Order> m_orders;
	States m_state, m_next_state; // States of the helicopter
	bool m_accelerating; // If propellers needs rotation speed change
	bool m_filled; // If it's filled or not
	bool m_infinite_balloons; // If the helicopter can shoot infinite balloons
	bool m_draw_shadow; // If the shadow is supossed to appear
	bool m_draw_path;
	int m_propeller_drag; // Effect of very fast propeller rotation
	int m_paint_balls; // Balloons to drop
	std::vector<Color> m_balls_colors; // Colors of the paint balls
	float m_goal_x, m_goal_y; // Position the helicopter should go to
	float m_goal_height; // Height it's supossed to be
	float m_spin_speed, m_spin_angle, m_spin_speed_goal; // Speed propellers are rotating, it's current angle and speed it should be rotating
	float m_update_tick; // Update counter
	float m_red, m_green, m_blue; // Fuselage color
	float m_ws_red, m_ws_green, m_ws_blue;  // Windshield color
	float m_x, m_y, m_z; // Position
	float m_x_rot, m_y_rot, m_z_rot; // Rotation

};

#endif
