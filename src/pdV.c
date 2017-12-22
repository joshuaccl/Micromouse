/*
 ******************************************************************************
 * File Name          : pdV.c
 * Description        : This file provides code for the PD controller
 *                      for the velocity
 ****************************************************************************** */

#include "pdV.h"
	//error = target velocity - current velocity



float leftVelocityOldError;
float rightVelocityOldError;

void velocityLeft(float correction)
{

}
void velocityRight(float correction)
{

}
float getVelocityL(void)
{
	return leftVelocityOldError;
}
float getVelocityR(void)
{
	return rightVelocityOldError;
}
void setVelocityL(float value)
{
	leftVelocityOldError = value;
}
void setVelocityR(float value)
{
	rightVelocityOldError = value;
}
