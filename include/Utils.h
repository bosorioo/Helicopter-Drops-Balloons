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

#ifndef _UTILS_H
#define _UTILS_H

#include <cstdlib>
#include <cmath>

#define PI 3.14159265f

namespace Utils
{
	int getRandomInt(int min = 0, int max = 1);
	float getRandomFloat(float min = 0.f, float max = 1.f);
	float toRad(float degrees);
	float toDeg(float radians);
	float degreeRange(float degrees);
	int signalFromBool(bool b);
	template <class T> bool isPositive(T num);
	template <class T> T limit(T min, T num, T max);
	template <class T> T pointDistance(T x1, T y1, T z1, T x2, T y2, T z2);
	template <class T> T pointDistance(T x1, T y1, T x2, T y2);
	template <class T> T pointDistance(T x1, T x2);
	template <class T> T pointAngle(T x1, T y1, T x2, T y2);
	template <class T, class C> T bitCopy(C bits);
};

template <class T>
bool Utils::isPositive(T num)
{
	return num >= 0 ? true : false;
}

template <class T>
T Utils::limit(T min, T num, T max)
{
	if (num < min)
		return min;
	else if (num > max)
		return max;

	return num;
}

template <class T>
T Utils::pointDistance(T x1, T y1, T z1, T x2, T y2, T z2)
{
	T difX = x1 - x2, difY = y1 - y2, difZ = z1 - z2;
	return std::sqrt(difX * difX + difY * difY + difZ * difZ);
}

template <class T>
T Utils::pointDistance(T x1, T y1, T x2, T y2)
{
	T difX = x1 - x2, difY = y1 - y2;
	return std::sqrt(difX * difX + difY * difY);
}

template <class T>
T Utils::pointDistance(T x1, T x2)
{
	return std::abs(x1 - x2);
}

template <class T>
T Utils::pointAngle(T x1, T y1, T x2, T y2)
{
	T difX = x2 - x1, difY = y2 - y1;
	return std::atan2(difY, difX);
}

template <class T, class C>
T Utils::bitCopy(C bits)
{
    void* ptr = (void*)&bits;
	return *(T*)ptr;
}

#endif
