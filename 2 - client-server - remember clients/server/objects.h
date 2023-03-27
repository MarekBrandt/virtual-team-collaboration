#include <stdio.h>
#include "quaternion.h"

#define PI 3.1415926

struct ObjectState
{
	Vector3 vPos;             // vehicle center position (geometric center)
	quaternion qOrient;       // orientation (angular position)
	Vector3 vV, vA;           // vehicle center velocity and acceleration vectors 
	Vector3 vV_ang, vA_ang;   // angular velocity and acceleration vectors 
	float steering_angle;     // front wheels rotation angle  (positive - to the left)
};