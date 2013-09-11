/*****************************************************
  State.cpp
  
  Source file for State Class
  
  CPSC8170 - Proj 1   GBG   8/2013
******************************************************/

#include "State.h"

#include <cstdlib>
#include <cstdio>
#include <math.h>

using namespace std;

//
// Constructor
//
State::State() {
	
	Velocity.set(0,0,0);
	Acceleration.set(0,0,0);
	V0.set(0,0,0);
	
	Center.set(0,0,0);
	C0.set(0,0,0);
	Mass = 0;
	Radius = 0;
	
	Start = true;
	Stopped = true;
	Step = false;
	Resting = false;
	Trace = true;
	
	CoeffofRestitution = 0;
	CoeffofFriction = 0;
	EPS = 0.1;
	
	Wind.set(0,0,0);
	G.set(0, -9.86, 0);
	Viscosity = 0;
}


//
// Setters
//
void State::SetVelocity(Vector3d v) { Velocity = v; }
void State::SetVX(float x) { Velocity.x = x; }
void State::SetVY(float y) { Velocity.y = y; }
void State::SetVZ(float z) { Velocity.z = z; }
void State::SetVelocity(float x, float y, float z) { Velocity.set(x, y, z); }
void State::SetInitialVelocity(Vector3d v0) { V0 = v0; }
void State::SetInitialVelocity(float x, float y, float z) { V0.set(x, y, z); }
void State::SetAcceleration(Vector3d a) { Acceleration = a; }
void State::SetCenter(Vector3d c) { Center = c ;}
void State::SetCenter(float x, float y, float z) { Center.set(x, y, z); }
void State::SetInitialCenter(Vector3d c0) { C0 = c0; }
void State::SetInitialCenter(float x, float y, float z) { C0.set(x, y, z); }
void State::SetCX(float x) { Center.x = x; }
void State::SetCY(float y) { Center.y = y; }
void State::SetCZ(float z) { Center.z = z; }
void State::SetMass(double m) { Mass = m; }
void State::SetRadius(float r) { Radius = r; }
void State::SetStart(int start) { Start = start; }
void State::SetStopped(int stop) { Stopped = stop; }
void State::SetStep(int step) { Step = step; }
void State::SetTrace(int trace) { Trace = trace; }
void State::SetCoeffR(double cor) { CoeffofRestitution = cor; }
void State::SetCoeffF(double cof) { CoeffofFriction = cof; }
void State::SetEPS(float eps) { EPS = eps; }
void State::SetGravity(Vector3d g) { G = g; }
void State::SetWind(Vector3d w) { Wind = w; }
void State::SetViscosity(double viscosity) { Viscosity = viscosity; }

void State::AddCollision(int collision, int indx) {
  Collision[indx] = collision;
}

void State::AddOldCenter(int indx) {
  OldCenter[indx].set(Center);
}

void State::SetResting(int type) { Resting = type; }
void State::SetCollidedN(Vector3d vn) { CollidedN = vn; }
void State::SetT(double t) { T = t; }

//
// Getters
//
Vector3d State::GetVelocity() { return Velocity; }
Vector3d State::GetInitialVelocity() { return V0; }
Vector3d State::GetInitialCenter() { return C0; }
Vector3d State::GetAcceleration() { return Acceleration; }
Vector3d State::GetCenter() { return Center;}
double State::GetMass() { return Mass; }
double State::GetRadius() { return Radius; }
int State::IsStarted() { return Start; }
int State::IsStopped() { return Stopped; }
int State::IsStep() { return Step; }
int State::IsResting() { return Resting;}
int State::IsTrace() { return Trace; }
Vector3d State::GetG() { return G; }
double State::GetViscosity() { return Viscosity; }
Vector3d State::GetWind() { return Wind; }
double State::GetCoeffR() { return CoeffofRestitution; }
double State::GetCoeffF() { return CoeffofFriction; }
float State::GetEPS() { return EPS; }
Vector3d State::GetCollidedN() { return CollidedN; }
int State::Collided(int indx) { return Collision[indx]; }
Vector3d State::GetOldCenter(int indx) { return OldCenter[indx]; }
double State::GetT() { return T; }

//
// Functions
//
// Find Acceleration
// use for particles
void State::CalcAcceleration() {
	Acceleration = G;
	
	Acceleration = Acceleration + Viscosity * (Wind - Velocity) / Mass;
	// no wind: Acceleration = Acceleration - Viscosity * Velocity / Mass
}

// Find New Velocity -- but don't store it!
// use for particles
Vector3d State::CalcNewVelocity(double timestep) {
	return Velocity + timestep * Acceleration;
}

// Find New Velocity w/ time fraction
// use for particles
Vector3d State::CalcNewVelocity(double timestep, double f) {
		return Velocity + f * timestep * Acceleration;
}

// Scale the velocity w/ coefficients of friction & restition - DO STORE IT.
// use for particles
void State::ScaleVelocity(Vector3d pnormal) {
	Vector3d vn, vt;
	Vector3d unorm;
	
	unorm.set(pnormal.normalize());
	
	if (Velocity * unorm == 0) vn.set(0,0,0);
	else vn = (Velocity * unorm) * unorm;
	
	if (Velocity * unorm == 0) vt = Velocity;
	else vt = Velocity - (Velocity * unorm) * unorm;
	
	vn = -CoeffofRestitution * vn;
	vt = (1 - CoeffofFriction) * vt;
	
	Velocity = vn + vt;
}

// Find New Position -- but don't store it either!
// use for particles
Vector3d State::CalcNewPosition(double timestep) {
	return Center + timestep * Velocity;
}

// Find New Position w/ time fraction
// use for particles
Vector3d State::CalcNewPosition(double timestep, double f) {
		return Center + f * timestep * Velocity;
}

// Adjust the acceleration, velocity, and position of the particle
// use for particles
void State::AdjustAccVelPos(Vector3d pnormal, Vector3d pvertex) {
	// reverse the direction of the vector i want to subtract...so...bVelocity - vNorm?
	// then set the new center so that...radius = t....& solve for bCenter?
	// bCenter	= vertex - t * (vN * bvel) / vn 
	// then subtract from the acceleration...

	Vector3d vn, an;
	Vector3d intersect;
	float p;
	
	vn = (Velocity * pnormal) * pnormal;
	an = (Acceleration * pnormal) * pnormal;
	
	// find where we intersect and find  if we're on the plane...
	//once we have the point of intersection, decide where the ball is & adjust the intersection to account for the radius
	p = (Velocity.normalize() * pvertex) - (Velocity.normalize() * pnormal);
	if(p < 0) { // we're behind
		intersect = Center + Velocity * T ;
	} else {
		intersect = Center - Velocity * T ;
	}
		
    Velocity = Velocity - vn;
	Acceleration = Acceleration - an;
	Center = intersect;
}

