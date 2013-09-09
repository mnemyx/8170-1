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
void Entity::Start(int start) { EntState.SetStart(start); }
void Entity::Stopped(int stop) { EntState.SetStopped(stop); } 
void Entity::Step(int step) { EntState.SetStep(step); }
void Entity::Trace(int trace) { EntState.SetTrace(trace); }
void Entity::Center(Vector3d newc) { EntState.SetCenter(newc); }	
void Entity::Velocity(Vector3d newv) { EntState.SetVelocity(newv); }
void Entity::InitState(Vector3d vel, Vector3d cen, double mass, double radius, double coeffr, double coefff, float eps, double viscosity, Vector3d wind, Vector3d gravity) {
	BuildSphere(radius, 3, cen.x, cen.y, cen.z);
	EntState.SetInitialVelocity(vel);
	EntState.SetCenter(cen);
	EntState.SetInitialCenter(cen);
	EntState.SetMass(mass);
	EntState.SetRadius(radius);
	EntState.SetCoeffR(coeffr);
	EntState.SetCoeffF(coefff);
	EntState.SetEPS(eps);
	EntState.SetViscosity(viscosity);
	EntState.SetWind(wind);
	EntState.SetGravity(gravity);
}
void Entity::InitState(Vector3d plane, Vector3d cen, float eps) {
	BuildPlane(plane.x, plane.y, plane.z, cen.x, cen.y, cen.z);
	EntState.SetEPS(eps);
	EntState.SetCenter(cen);
	EntState.SetInitialCenter(cen);
}
	
	
// 
// Getters
//
Vector3d Entity::InitialCenter() { return EntState.GetInitialCenter(); }
Vector3d Entity::InitialVelocity() { return EntState.GetInitialVelocity(); }
Vector3d Entity::Center() { return EntState.GetCenter(); }
Vector3d Entity::Velocity() { return EntState.GetVelocity(); }
Vector3d Entity::Acceleration() { return EntState.GetAcceleration(); }
double Entity::Radius() { return EntState.GetRadius(); }
Vector3d Entity::PlaneNormal() { return EntState.GetCollidedN(); }
Vector3d Entity::PlaneVertex(int indx) { return vertices[indx]; }
int Entity::Rest() { return EntState.IsResting(); }
int Entity::Start() { return EntState.IsStarted(); }
int Entity::Stopped() { return EntState.IsStopped(); } 
int Entity::Step() { return EntState.IsStep(); }
int Entity::Trace() { return EntState.IsTrace(); }
int Entity::GetCollision(int indx) { return EntState.Collided(indx); }	// called by planes
Vector3d Entity::OldCenter(int indx) { return EntState.GetOldCenter(indx); }	// called by planes
float Entity::FudgeFactor() { return EntState.GetEPS(); }

//
// Functions
//
// calculate f to determine if the ball collided with the plane
// called by plane...
float Entity::PlaneBallColl(Vector3d bCenter, Vector3d bVelocity, Vector3d bNewCenter, float bRadius) {
	float mf;
	float f;
	float p;
	int i;
	Vector3d bCentMod;
	Vector3d bNewCentMod;
	
	for(i = 0; i < ntriangles; i++) {
		// need to decide where are sphere is located in terms of the plane | point-plane equation
		// so we know to add or subtract the radius from the center
		p = normals[i] * (bCenter - vertices[1]);
		
		if(p < 0) { //we're behind the plane we're testing
			bCentMod.set(bCenter.x + bRadius, bCenter.y + bRadius, bCenter.z + bRadius);
			bNewCentMod.set(bNewCenter.x + bRadius, bNewCenter.y + bRadius, bNewCenter.z + bRadius);
		} else { // we're in front
			bCentMod.set(bCenter.x - bRadius, bCenter.y - bRadius, bCenter.z - bRadius);
			bNewCentMod.set(bNewCenter.x - bRadius, bNewCenter.y - bRadius, bNewCenter.z - bRadius);
		}
		
  		f = ((bCentMod - vertices[1]) * normals[i] / ((bCentMod - bNewCentMod) * normals[i]));

		if ( i == 0 ) mf = f; 
		else if ( f < mf && f >= 0 - FudgeFactor() && f < 1 + FudgeFactor()) mf = f; 
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
	float p;
	int i;
	Vector3d vN;
	Vector3d bCentMod;
	
	// average normal?
	Vector3d avgN;
	
	for (i = 0; i < ntriangles; i++) {
		avgN = avgN + normals[i];
	}
	
	avgN = avgN / 2;
	avgN.normalize();
	
	
	p = avgN * (bCenter - vertices[1]);
		
	cout << "P IN RESTINGINPLANE() :::::::::::::::::::::::::::: " << p << endl;
	if(p < 0) { //we're behind the plane we're testing
		bCentMod.set(bCenter.x + bRadius, bCenter.y + bRadius, bCenter.z + bRadius);
	} else { // we're in front
		bCentMod.set(bCenter.x - bRadius, bCenter.y - bRadius, bCenter.z - bRadius);
	}

	if (avgN * bVelocity == 0) {
		t = (bCentMod - vertices[1]).normsqr();
	} else {
		t = - (avgN * (bCentMod - vertices[1])) / (avgN * bVelocity);
	}

	cout << "t: " << t << endl;

	if ( i == 0 ) {
		mt = t;
		vN = bVelocity * avgN;
		EntState.SetCollidedN(avgN);
	}
	else if ( t < mt ) { 
		mt = t;
		vN = bVelocity * avgN;
		EntState.SetCollidedN(avgN);
	}
	
	// Don't I need to figure out the velocity in the direction of the normal and see if it's
	// below the threshold?  ...Added above.
	cout << "timeSTep: " << timeStep << " * vN: "; (timeStep * vN).print(); cout << endl;
	cout << "mt: " << mt  << endl;
	EntState.SetResting(((Abs(timeStep * vN.x) < FudgeFactor()) && (Abs(timeStep * vN.y) < FudgeFactor()) && (Abs(timeStep * vN.z) < FudgeFactor())) && Abs(mt) < FudgeFactor());
	// note to self: change fudge factor for radius.
	cout << EntState.IsResting() << " is resting()" << endl;
	//EntState.SetCollidedN(vN);
	EntState.SetT(mt);
}

// need to figure out magnitude...of acceleration in direction of the normal
// called by plane...
int Entity::AccelOnPlane(Vector3d bAccel) {
	cout << "bAccel * CollidedN: " << bAccel * EntState.GetCollidedN() << endl;
	Vector3d acceln;
	
	acceln = (bAccel * EntState.GetCollidedN()) * EntState.GetCollidedN();

	cout << "acceln: "; acceln.print(); cout << endl;
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
int Entity::CenOnPlane(float bRadius) {
	if((EntState.GetT() - bRadius) < EntState.GetEPS()) return true;
	else return false;
}

// For tracing, if we keep that feature
void Entity::AddOCenter(int nsteps) {
	EntState.AddOldCenter(nsteps);
}

// For tracing, if we keep that feature
void Entity::AddOCollision(int collision, int nsteps){
	EntState.AddCollision(collision, nsteps);
}

// for updating the model's triangles -- called by particle
void Entity::UpdateModel() {
	BuildSphere(EntState.GetRadius(), 3, EntState.GetCenter().x, EntState.GetCenter().y, EntState.GetCenter().z);
}

// acceleration -- called by particle.
void Entity::Accel() {
	EntState.CalcAcceleration();
}

// called by particle...hm.
void Entity::AdjustAVC(Vector3d pnormal, Vector3d pvertex) {
	EntState.AdjustAccVelPos(pnormal, pvertex);
}

// called by particle
Vector3d Entity::CalcVelocity(double timestep, double f) { return EntState.CalcNewVelocity(timestep, f); }
Vector3d Entity::CalcVelocity(double timestep) { return EntState.CalcNewVelocity(timestep); }
Vector3d Entity::CalcCenter(double timestep, double f) { return EntState.CalcNewPosition(timestep, f); }
Vector3d Entity::CalcCenter(double timestep) { return EntState.CalcNewPosition(timestep); }
void Entity::ScaleVel(Vector3d pnormal) { EntState.ScaleVelocity(pnormal); }
