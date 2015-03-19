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

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>
#include "Utils.h"
#include "Drawing.h"
#include "Helicopter.h"
#include "Ground.h"
#include "World.h"

#define GLUT_DISABLE_ATEXIT_HACK

// This is the definition of how fast things update, change this to make software go faster
// 1 is the default. Lower than 1 is slower and bigger than 1 is faster.
// This can also be changed inside program up Up/Down arrow keys.
#define SPEED_ADJUSTMENT 1.f

#define TIMER_FUNC_INTERVAL 20

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

int gWidth, gHeight;
int gCamera = 0;
const char* gCameraNames[] = {"Camera 1", "Camera 2", "Camera 3", "Camera 4", "Top Camera (5)", "Side Camera (6)", "Back Camera (7)"};
float yRotation = 0.f;
float gScaling = 1.f;
float speedRotation = 2.f;
float speedAdjustment = SPEED_ADJUSTMENT;
bool showInfo = true;
bool infiniteBalloons = false;
std::vector<Point2D> fly_points;
Helicopter helicopter;
World world;

void setProjection()
{
	if (gCamera != 4)
		gluPerspective(45.0, (double)gWidth/gHeight, 1.0, 1000.0);
	else
		glOrtho(-85.0, 85.0, -85.0, 85.0, -500.f, 1000.0);
}

void setModelview()
{
	#define CAM_DIST 110.f

	float cameras[] = {
		115.f, 130.f, 115.f,
		170.f, 130.f, 0.f,
		-125.f, 85.f, -155.f,
		120.f, 44.f, 110.f
	};

	if (gCamera <= 3)
	{
		gluLookAt(cameras[gCamera * 3], cameras[gCamera * 3 + 1], cameras[gCamera * 3 + 2],
			0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	}
	else if (gCamera == 5)
		helicopter.sideCamera_gluLookAt();
	else if (gCamera == 6)
		helicopter.backCamera_gluLookAt();
	else
		glRotatef(90.f, 1.f, 0.f, 0.f);

	if (gCamera < 4)
	{
		glScalef(gScaling, gScaling, gScaling);
		glRotatef(yRotation, 0, 1, 0);
	}
}

void shootBall()
{
	PaintBall* ball = world.addPaintBall();
	if (!helicopter.dropPaintBall(ball))
		ball->remove();
}

void resetGame()
{
	speedAdjustment = SPEED_ADJUSTMENT;
	gScaling = 1.f;
	yRotation = 0.f;
	helicopter.reset();
	helicopter.setPaintBallsCount(6);
	world.reset();
}

void displayInfo()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, gWidth, 0, gHeight, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	char buffer[256];

	if (showInfo)
	{
		glColor3f(0.f, 0.f, 0.f);
		glTranslatef(gWidth/2.f, 55.f, 0.f);
		glRotatef(90.f, 1.f, 0.f, 0.f);
		Draw::rectangle2d(gWidth - 10, 100.f, true);
		glColor3f(1.f, 1.f, 1.f);
		Draw::rectangle2d(gWidth - 10, 100.f, false);
		glLoadIdentity();
		glColor3f(1., 1., 1.);
		Draw::text("Press 'Q' to hide this information box", 10, 90, 0.f, false);
		sprintf(buffer, "Speed Adjustment (Up/Down arrows): %.2f", speedAdjustment);
		Draw::text(buffer, 10, 70, 0.f, false);
		sprintf(buffer, "%s Mode ('X') Shadow is %s ('S')",
			helicopter.isFilled() ? "Filled" : "Wired ",
			helicopter.isShadowed() ? "enabled. " : "disabled.");
		Draw::text(buffer, 10, 50, 0.f, false);
		sprintf(buffer, "Scaling factor: %.2f ('+' / '-' to change)", gScaling);
		Draw::text(buffer, 10, 30, 0.f, false);
		sprintf(buffer, "Y Rotation: %.2f (Right/Left arrows)", yRotation);
		Draw::text(buffer, 10, 12, 0.f, false);
	}

	glColor3f(1.f, 1.f, 1.f);

	sprintf(buffer, "Engines are %s  Balloons left: %d  Engine RPM: %d%%",
		helicopter.isPowerOn() ? "on " : "off", helicopter.getBalloons(),
		helicopter.getRPMPercentage());
	Draw::text(buffer, 10, gHeight - 20.f, 0.f, false);

	int hits, misses;
	world.getHitInfo(hits, misses);

	sprintf(buffer, "Current Camera: %s", gCameraNames[gCamera]);
	Draw::text(buffer, 10, gHeight - 36.f, 0.f, false);

	sprintf(buffer, "Ballons hits/misses: %d/%d", hits, misses);
	Draw::text(buffer, 10, gHeight - 52.f, 0.f, false);
}

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setModelview();

	world.draw();
	helicopter.draw();

	displayInfo();

	glFlush();
}

void initWindow(void)
{
	glClearColor(.0f, .0f, 0.f, 0.f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void resize(int w, int h)
{
	gHeight= h;
	gWidth = w;
	glViewport(0, 0, gWidth, gHeight);
}

void mouseInput(int button, int state, int x, int y)
{
	y = gHeight - y;

	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float worldCoordX = (float)x / (float)viewport[2] * 170.f - 85.f;
	float worldCoordY = (float)y / (float)viewport[3] * 170.f - 85.f;

	if (gCamera == 4)
		world.mouseClick(worldCoordX, -worldCoordY);

	GLuint buffer[100];

	glSelectBuffer(100, buffer);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPickMatrix((GLdouble)x, (GLdouble)y, 5.0, 5.0, viewport);
	setProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	setModelview();

	helicopter.draw(false, true); // Render simpler helicopter in picking mode, for performance

	if (glRenderMode(GL_RENDER))
		shootBall();
}

void mousePassiveMove(int x, int y)
{
	if (gCamera != 4)
		return;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float worldCoordX = (float)x / (float)viewport[2] * 170.f - 85.f;
	float worldCoordY = (float)(viewport[3] - y) / (float)viewport[3] * 170.f - 85.f;

	world.mouseMove(worldCoordX, -worldCoordY);
}

void mouseMotionFunc(int x, int y)
{
	if (gCamera != 4)
		return;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float worldCoordX = (float)x / (float)viewport[2] * 170.f - 85.f;
	float worldCoordY = (float)(viewport[3] - y) / (float)viewport[3] * 170.f - 85.f;

	world.mouseDrag(worldCoordX, -worldCoordY);
	world.getPathOverObjects(fly_points);
	helicopter.setFlyPoints(fly_points);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	if (key >= 'A' && key <= 'Z')
		key += 32;

	if (key == '0')
		key = '5';

	if (key >= '1' && key <= ('0' + (char)(sizeof(gCameraNames) / sizeof(*gCameraNames))))
		gCamera = (int)key - (int)'1';

	switch(key)
	{
		case 27:
			exit(0);
			break;

		case 'p':
			if (helicopter.isOnGround())
				helicopter.power(!helicopter.isPowerOn());
			else
				helicopter.land();
			break;

		case 'd':
			helicopter.toggleDrawPath();
			break;

		case 'f':
			if (helicopter.isPowerOn())
			{
				world.getPathOverObjects(fly_points);
				helicopter.flyPath(fly_points);
			}
			break;

		case 's':
			helicopter.showShadow(!helicopter.isShadowed());
			break;

		case 'q':
			showInfo = !showInfo;
			break;

		case 'w':
			infiniteBalloons = !infiniteBalloons;
			helicopter.setInfiniteBalloons(infiniteBalloons);
			break;

		case 'a':
			shootBall();
			break;

		case 'r':
			resetGame();
			break;

		case 'z':
			gCamera = (gCamera + 1) % (sizeof(gCameraNames) / sizeof(*gCameraNames));
			break;

		case 'x':
			helicopter.toggleFill();
			world.toggleFill();
			break;

		case 'c':
			world.getGround().toggleMode();
			break;

		case '+':
			gScaling += .05f;
			break;

		case '-':
			gScaling -= .05f;
			break;

		default:
			break;
	}

	gScaling = Utils::limit(0.5f, gScaling, 3.f);
}

void specialInput(int key, int x, int y)
{
	switch(key)
	{

	case GLUT_KEY_DOWN:
		speedAdjustment -= .05f;
		break;

	case GLUT_KEY_UP:
		speedAdjustment += .05f;
		break;

	case GLUT_KEY_RIGHT:
		yRotation += speedRotation;
		break;

	case GLUT_KEY_LEFT:
		yRotation -= speedRotation;
		break;

		default:
			break;
	}

	speedAdjustment = Utils::limit(0.05f, speedAdjustment, 10.f);

	if (yRotation >= 360.f)
		yRotation -= 360.f;
	else if (yRotation < 0)
		yRotation += 360.f;
}

void timerFunction(int arg)
{
	helicopter.update(arg / 1000.f);
	world.update(arg / 1000.f);
	glutPostRedisplay();
	glutTimerFunc(TIMER_FUNC_INTERVAL, timerFunction, TIMER_FUNC_INTERVAL * speedAdjustment);
}

// Main routine.
int main(int argc, char **argv)
{
	std::cout << "Possible inputs:\n";
	std::cout << "Up/Down arrows - adjust simulation speed\n";
	std::cout << "Left/Right arrows - rotate world for some cameras\n";
	std::cout << "'+' and '-' - scale the world\n";
	std::cout << "'C' - change ground rendering mode\n";
	std::cout << "'P' - turns on/off helicopter when it is on ground, lands on center if helicopter if flying.\n";
	std::cout << "'D' - toggle view of flight path\n";
	std::cout << "'F' - makes helicopter fly again over objects if it is turned on\n";
	std::cout << "'S' - toggle view of helicopter shadow\n";
	std::cout << "'Q' - toggle view of information box on bottom of screen\n";
	std::cout << "'W' - toggle infinite balloons mode\n";
	std::cout << "'A' / Mouse click on helicopter - shoots a balloon\n";
	std::cout << "'R' - reset most configurations to default\n";
	std::cout << "'Z' - changes camera one by one\n";
	std::cout << "'1' until '7' - sets current camera\n";
	std::cout << "'0' - sets current camera to overhead camera (same as key '5')\n";
	std::cout << "'X' - toggle helicopter/balloons/paint batches wired mode\n";

	srand(time(NULL));
	rand(); rand(); rand();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HW3");
	initWindow();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialInput);
	glutMouseFunc(mouseInput);
	glutPassiveMotionFunc(mousePassiveMove);
	glutMotionFunc(mouseMotionFunc);

	glutTimerFunc(TIMER_FUNC_INTERVAL, timerFunction, TIMER_FUNC_INTERVAL);

	resetGame();
	world.addObject(-45.f, -45.f);
	world.addObject(-45.f, 45.f);
	world.addObject(45.f, -45.f);
	world.addObject(45.f, 45.f);
	world.getPathOverObjects(fly_points);
	helicopter.setFlyPoints(fly_points);
	glutMainLoop();

	return 0;
}
