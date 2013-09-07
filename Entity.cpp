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
// Setters
//
void Entity::Rest(int type) { EntState.SetResting(type); }
void Entity::Start(int start) { EntState.SetStart(star); }
void Entity::Stopped(int stop) { EntState.SetStopped(stop); } 
void Entity::Step(int step) { EntState.SetStep(step); }
void Entity::Center(Vector3d newc) { EntState.SetCenter(newc); }	
void Entity::Velocity(Vector3d newv) { EntState.SetVelocity(newv); }

// 
// Getters
//
Vector3d Entity::Vector3d InitialCenter( return EntState.GetInitialCenter(); }
Vector3d Entity::Vector3d InitialVelocity() { return EntState.GetInitialVelocity(); }
Vector3d Entity::Center() { return EntState.GetCenter(); }
Vector3d Entity::Velocity() { return EntState.GetVelocity(); }
Vector3d Entity::Acceleration() { return EntState.GetAcceleration(); }
double Entity::Radius() { return EntState.GetRadius(); }
int Entity::Rest() { return EntState.IsResting(); }
int Entity::Stopped() { return EntState.IsStopped(); }
Vector3d Entity::PlaneNormal() { return EntState.GetCollidedN(); }
Vector3d Entity::PlaneVertex(int indx) { return vertices[indx]; }
int Entity::Start() { return EntState.IsStarted(); }
int Entity::Stopped() { return EntState.IsStopped(stop); } 
int Entity::Step() { return EntState.IsStep(step); }


//
// Functions
//
// calculate f to determine if the ball collided with the plane
// called by plane...
double Entity::PlaneBallColl(Vector3d bCenter, Vector3d bVelocity, Vector3d bNewCenter, float bRadius) {
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

// called by plane...
void Entity::RestingOnPlane(Vector3d bCenter, Vector3d bVelocity, float bRadius, double timeStep) {
	// t is the distance from the center of the ball to the intersection on the plane
	// t < 0 then it's behind the starting point 
	// t == 0 then it's parallel and never hits
	// t= (Xn dot (PointOnPLANE - Raystart)) / (Xn dot Raydirection)-- NeHe Collision Detection
	//  Resting = Abs(timestep * Velocity.y) < EPS.y && Abs(Center.y - (Radius + miny)) < EPS.y;
	float mt = 0;
	float t;
	int i;
	Vector3d vN;
	
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
	EntState.SetResting(Abs(timeStep * vN) < EntState.GetEPS() && Abs(mt - bRadius) < EntState.GetEPS());
	EntState.SetCollidedN(vN);
	EntState.SetT(mt);
}

// need to figure out magnitude...of acceleration in direction of the normal
// called by plane...
int Entity::AccelOnPlane(Vector3d bAccel) {
	if ((bAccel * EntState.GetCollidedN()) < EntState.GetEPS()) return true;
	else return false;
}

// need to figure out magnitude...of velocity in the direction of normal
// called by plane...
int Entity::VelOnPlane(Vector3d bVelocity) {
	if ((bVelocity * EntState.GetCollidedN()) < EntState.GetEPS()) return true;
	else return false;
}

// need to figure out the distance from collision < radius
// called by plane...
int Entity::CenOnPlane(Vector3d bRadius) {
	if((EntState.GetT - bRadius) < EntState.GetEPS()) return true;
	else return false;
}

// For tracing, if we keep that feature
void Entity::AddOCenter(int nsteps) {
	EntState.AddOldCenter(nsteps);
}

// For tracing, if we keep that feature
void Entity::AddOCollision(int collision, int nteps){
	EntState.AddCollision(int collision, int nsteps)
}

// acceleration -- called by particle.
void Entity::Accel() {
	EntState.CalcAcceleration();
}

// called by particle...hm.
void Entity::AdjustAVC(Vector3d pnormal, Vector3d pvertex) {
	EntState.AdjustAccVelPos(prnormal, pvertex);
}

// called by particle
Vector3d Entity::Velocity(double timestep, double f, int atCollision) { return EntState.CalcNewVelocity(timestep, f, atCollision); }
Vector3d Entity::Velocity(double timestep, double f) { return EntState.CalcNewVelocity(timestep); }
Vector3d Entity::Center(double timestep, double f, int atCollision) { return EntState.CalcNewPosition(timestep, f, atCollision); }
Vector3d Entity::Center(double timestep) { return EntState.CalcNewPosition(timestep); }
void Entity::ScaleVel(Vector3d pnormal) { EntState.ScaleVelocity(pnormal); }
