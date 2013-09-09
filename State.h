/***************************************************
  State.h
  
  Header file for State Class
  
  CPSC8170 - Proj 1   GBG   8/2013
****************************************************/

#ifndef _STATE_H_
#define _STATE_H_

#include "Vector.h"
#include "Model.h"
#include <vector>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define MAXSTEPS	10000


  
class State{
  private:
    
    Vector3d Velocity;		// object's velocity
    Vector3d V0;			// object's initial velocity
    Vector3d Acceleration;	// object's acceleration
    
    Vector3d Center;		// for particles...really. i should consider changing these...
	Vector3d C0;
    double Mass;			// for particles...really. i should consider changing these...
    float Radius;			// for particles...really. i should consider changing these...
    
    int Start;				// is the object started
    int Stopped;			// is the object stopped
    int Step;				// is...stepping?
    int Resting;			// is the object resting...
    int Trace;				// i'll probably keep this - trace the object's path
    
    double CoeffofRestitution;		// the object's coefficient of restitution
    double CoeffofFriction;			// the object's coefficient of friction
    float EPS;					// the "fudge factor"
	
	Vector3d Wind;			// the wind vector
    Vector3d G;				// gravity vector...
    double Viscosity;		// viscosity
    
    int Collision[MAXSTEPS];		// keeping track of the collisions
    Vector3d OldCenter[MAXSTEPS];	// keeping track of the object's old centers (for collision sake & tracing)
	Vector3d CollidedN;				// normal that was collided with...need to clean this up...
	double T;						// distance from the hit..

    
  public:
    // Constructor
    State();
    
    // Setters
    void SetVelocity(Vector3d v);
    void SetVX(float x);
    void SetVY(float y);
    void SetVZ(float z);
    void SetVelocity(float x, float y, float z);
    void SetInitialVelocity(Vector3d v0);
	void SetInitialVelocity(float x, float y, float z);
    void SetAcceleration(Vector3d a);
    void SetCenter(Vector3d c);
    void SetCenter(float x, float y, float z);
    void SetCX(float x);
    void SetCY(float y);
    void SetCZ(float z);
	void SetInitialCenter(Vector3d c0);
	void SetInitialCenter(float x, float y, float z);
    void SetMass(double m);
    void SetRadius(float r);
    void SetStart(int start);
    void SetStopped(int stop);
    void SetStep(int step);
	void SetTrace(int trace);
    void SetCoeffR(double cor);
    void SetCoeffF(double cof);
    void SetEPS(float eps);
    void SetResting(int type);
    void AddCollision(int collision, int indx);
    void AddOldCenter(int indx);
	void SetCollidedN(Vector3d vn);
	void SetT(double t);
	void SetGravity(Vector3d g);
	void SetWind(Vector3d w);
	void SetViscosity(double viscosity);
    
    // Getters
    Vector3d GetVelocity();
    Vector3d GetInitialVelocity();
	Vector3d GetInitialCenter();
    Vector3d GetAcceleration();
    Vector3d GetCenter();
    double GetMass();
    double GetRadius();
    int IsStarted();
    int IsStopped();
    int IsStep();
    int IsResting();
	int IsTrace();
    double GetCoeffR();
    double GetCoeffF();
	Vector3d GetG();
	double GetViscosity();
	Vector3d GetWind();
    float GetEPS();
    int Collided(int i = 0);
	Vector3d GetCollidedN();
	Vector3d GetOldCenter(int indx = 0);
	double GetT();
   
   // Functions  ||  Rule of thumb: if the calculations relies *mostly* on state variables, place in state.  If it relies 2 entities; then...should probably NOT put it here.
   void CalcAcceleration();
   Vector3d CalcNewVelocity(double timestep);
   Vector3d CalcNewVelocity(double timestep, double timefraction);
   void ScaleVelocity(Vector3d pnormal);
   Vector3d CalcNewPosition(double timestep);
   Vector3d CalcNewPosition(double timestep, double timefraction);
   void AdjustAccVelPos(Vector3d prnormal, Vector3d pvertex);
};

#endif
