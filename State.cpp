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

void State::UpdateObj(int current, int indx) {
  if(current) 
    Particle.BuildSphere(Radius, SPDEPTH, Center.x, Center.y, Center.z);
  else 
    Particle.BuildSphere(Radius, SPDEPTH, OldCenter[indx].x, OldCenter[indx].y, OldCenter[indx].z);
}

void State::AddCollision(int collision, int indx) {
  Collision[indx] = collision;
}

void State::AddOldCenter(Vector3d cold, int indx) {
  OldCenter[indx].set(cold);
}

void State::SetResting(int type) { Resting = type; }

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

int State::Collided(int indx) { return Collision[indx]; }

//
// Functions
//
// Find Acceleration
void CalcAcceleration() {
	Acceleration = G;
	
	if(HaveWind) 
		Acceleration = Acceleration + Viscosity * (Wind - Velocity) / Mass;
	else
		Acceleration = Acceleration - Viscosity * Velocity / Mass
}
