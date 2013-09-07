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
void State::SetAcceleration(Vector3d a) { Acceleration = a; }
void State::SetCenter(Vector3d c) { Center = c ;}
void State::SetCenter(float x, float y, float z) { Center.set(x, y, z); }
void State::SetCX(float x) { Center.x = x; }
void State::SetCY(float y) { Center.y = y; }
void State::SetCZ(float z) { Center.z = z; }
void State::SetMass(double m) { Mass = m; }
void State::SetRadius(float r) { Radius = r; }
void State::SetStart(int start) { Start = start; }
void State::SetStopped(int stop) { Stopped = stop; }
void State::SetStep(int step) { Step = step; }
void State::SetCoeffR(double cor) { CoeffofRestitution = cor; }
void State::SetCoeffF(double cof) { CoeffofFriction = cof; }
void State::SetEPS(Vector3d eps) { EPS = eps; }

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
Vector3d State::GetAcceleration() { return Acceleration; }
Vector3d State::GetCenter() { return Center;}
double State::GetMass() { return Mass; }
double State::GetRadius() { return Radius; }
int State::IsStarted() { return Start; }
int State::IsStopped() { return Stopped; }
int State::IsStep() { return Step; }
int State::IsResting() { return Resting;}
Vector3d State::GetG() { return G; }
double State::GetViscosity() { return Viscosity; }
Vector3d State::GetWind() { return Wind; }
int State::HaveWind() return HaveWind; }
double State::GetCoeffR() { return CoeffofRestitution; }
double State::GetCoeffF() { return CoeffofFriction; }
float State::GetEPS() { return EPS; }
Vector3d State::GetColiddedN() { return CollidedN; }
int State::Collided(int indx) { return Collision[indx]; }
double State::GetT() { return T; }

//
// Functions
//
// Find Acceleration
// use for particles
void State::CalcAcceleration() {
	Acceleration = G;
	
	if(HaveWind) 
		Acceleration = Acceleration + Viscosity * (Wind - Velocity) / Mass;
	else
		Acceleration = Acceleration - Viscosity * Velocity / Mass
}

// Find New Velocity -- but don't store it!
// use for particles
Vector3d State::CalcNewVelocity(double timestep) {
	//   newvelocity = Velocity + TimeStep * acceleration;
	return Velocity + timestep * Acceleration;
}

// Find New Velocity w/ time fraction
// use for particles
Vector3d State::CalcNewVelocity(double timestep, double f, int atCollision) {
	// if at: newvelocity = Velocity + f * TimeStep * acceleration;
	// else: newvelocity = Velocity + (1 - f) * TimeStep * acceleration;
	if (atCollision) 
		return Velocity + f * timestep * Acceleration;
	else
		return Velocity + (1 - f) * timestep * Acceleration;
}

// Scale the velocity w/ coefficients of friction & restition - DO STORE IT.
// user for particles
void State::ScaleVelocity(Vector3d pnormal) {
	Vector3d vn, vt;
	
	vn = (Velocity * pnormal) * pnormal;
	vt = Velocity - (Velocity * pnormal) * pnormal;
	vn = -CoeffofRestitution * vn;
	vt = (1 - CoeffofFriction) * vt;
	
	Velocity = vn + vt;
}

// Find New Position -- but don't store it either!
// use for particles
Vector3d State::CalcNewPosition(double timestep) {
	//   newball = Ball + f * TimeStep * Velocity;
	return Center + f * timestep * Velocity;
}

// Find New Position w/ time fraction
// use for particles
Vector3d State::CalcNewPosition(double timestep, double f, int atCollision) {
	// if at: newball = Ball + f * TimeStep * Velocity;
	// else: newball = newball + (1 - f) * TimeStep * Velocity;
	if (atCollision) 
		return Center + f * timestep * Velocity;
	else
		return Center + (1 - f) * timestep * Velocity;
}

// Adjust the acceleration, velocity, and position of the particle
// use for particles
void State::AdjustAccVelPos(Vector3d pnormal, Vector3d pvertex) {
	// reverse the direction of the vector i want to subtract...so...bVelocity - vNorm?
	// then set the new center so that...radius = t....& solve for bCenter?
	// bCenter	= vertex - t * (vN * bvel) / vn 
	// then subtract from the acceleration...

	Vector3d vn, an, nc;
	
	vn = (Velocity * pnormal) * pnormal;
	an = (Acceleration * pnormal) * pnormal;
	nc = (Radius * (pnormal * Velocity) / pnormal) - pvertex;
	
    Velocity = Velocity - vn;
	Acceleration = Acceleration - an;
	Center = nc;
}