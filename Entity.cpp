/***************************************************
  Entity.cpp
  
  Source file for Entity Class
  
  CPSC8170 - Proj 1   GBG   8/2013
****************************************************/

#include "Entity.h"

#include <cstdlib>
#include <cstdio>

using namespace std;

//
// Constructor
//
Entity::Entity(){	
}

// 
// Getters
//


//
// Functions
//
// calculate f to determine if the ball collided with the plane
float Entity::PlaneBallColl(Vector3d bCenter, Vector3d bVelocity, Vector3d bNewCenter, float bRadius) {
	float mf = -1;
	float f;
	int i;
	Vector3d bCentMod(bCenter.x - bRadius, bCenter.y - bRadius, bCenter.z - bRadius);
	
	for(i = 0; i < ntriangles; i++) {
		f = ((bCentMod - vertices[0] * normals[i]) / ((bCenter - bNewCenter) * normals[i];
		
		if( f <= 0 && f < 1 ) {
			if ( i == 0 ) mf = f; 
			else if ( f < mf ) mf = f; 
		}
	}
	
	return mf;
}

void Entity::RestingOnPlane(Vector3d bCenter, Vector3d bVelocity, float bRadius, double timeStep) {
	// t is the distance from the center of the ball to the intersection on the plane
	// t < 0 then it's behind the starting point 
	// t == 0 then it's parallel and never hits
	// t= (Xn dot (PointOnPLANE - Raystart)) / (Xn dot Raydirection)-- NeHe Collision Detection
	//  Resting = Abs(timestep * Velocity.y) < EPS.y && Abs(Center.y - (Radius + miny)) < EPS.y;
	float mt = 0;
	float t;
	int i;
	Vector vN;
	
	for (i = 0; i < ntriangles; i++) {
		t = (normals[i] * (vertices[0] - bCenter)) / (normals[i] * bVelocity);
		
		if (t > 0) {
			if ( i == 0 ) {
				mt = t;
				vN = bVelocity * normals[i];
			}
			else if ( t < mt ) { 
				mt = t;
				vN = bVelocity * normals[i];
			}
		}
	}
	
	// Don't I need to figure out the velocity in the direction of the normal and see if it's
	// below the threshold?  ...Added above.
	EntState->SetResting(Abs(timeStep * vN) < EntState->GetEPS() && Abs(mt - bRadius) < EntState->GetEPS());
	EntState->SetCollidedN(vN);
}

