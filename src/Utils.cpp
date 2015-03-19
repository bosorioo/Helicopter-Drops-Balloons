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

#include "Utils.h"

int Utils::getRandomInt(int min, int max)
{
	if (min > max)
		return getRandomInt(max, min);

	if (min == max)
		return min;
		
	return static_cast<int>((float)(rand() % RAND_MAX) / RAND_MAX * (max - min + 1) + min);
}

float Utils::getRandomFloat(float min, float max)
{
	if (min > max)
		return getRandomFloat(max, min);

	if (min == max)
		return min;
	
	float random = static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX);

	return random * (max - min + 1) + min;
}

float Utils::toRad(float degrees)
{
	return degrees / 180.f * PI;
}

float Utils::toDeg(float radians)
{
	return radians * 180.f / PI;
}

float Utils::degreeRange(float degrees)
{
	int rounds = static_cast<int>(degrees) / 360;
	if (degrees < 0.f)
		rounds--;
	degrees = degrees - rounds * 360.f;
	return degrees >= 360.f ? degrees - 360.f : degrees;
}

int Utils::signalFromBool(bool b)
{
	return b ? 1 : -1;
}