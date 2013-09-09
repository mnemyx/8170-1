/***************************************************
  Entity.h
  
  Header file for Entity Class
  
  CPSC8170 - Proj 1   GBG   8/2013
****************************************************/

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "State.h"
#include "Model.h"

#define MAXSTEPS	10000
#define SPDEPTH		3

  
class Entity : public Model {		// entity is a model that has a state...
  private:
    State EntState;		// the entity's associated state variables
	
  public:
    // Constructor
    Entity();
    
	// Setters
	void Rest(int type);
	void Stopped(int type);
	void Start(int type);
	void Step(int type);
	void Trace(int type);
	void Velocity(Vector3d newv);
	void Center(Vector3d newc);	
	void InitState(Vector3d vel, Vector3d cen, double mass, double radius, double coeffr, double coefff, float eps, double viscosity, Vector3d wind, Vector3d gravity);
	void InitState(Vector3d plane, Vector3d cen, float eps);
	
	// Getters
	Vector3d InitialCenter();
	Vector3d InitialVelocity();
	Vector3d Center();
	Vector3d Velocity();
	Vector3d Acceleration();
	double Radius();
	int Rest();
	int Stopped();
	int Start();
	int Step();
	int Trace();
	Vector3d PlaneNormal();
	Vector3d PlaneVertex(int indx = 0);
	int GetCollision(int indx = 0);
	Vector3d OldCenter(int indx = 0);
	float FudgeFactor();

	// Functions
	/* called by nonmoving objects */
	float PlaneBallColl(Vector3d bCenter, Vector3d bVelocity, Vector3d bNewCenter, float bRadius);
	void RestingOnPlane(Vector3d bCenter, Vector3d bVelocity, float bRadius, double timeStep);
	int AccelOnPlane(Vector3d bAccel);
	int VelOnPlane(Vector3d bVelocity);
	int CenOnPlane(float radius);
	void AddOCenter(int nsteps);
	void AddOCollision(int collision, int nsteps);
	
	/* called by moving objects */
	void UpdateModel();
	void Accel();
	void AdjustAVC(Vector3d pnormal, Vector3d pvertex);
	Vector3d CalcVelocity(double timestep, double f);
	Vector3d CalcVelocity(double timestep);
	Vector3d CalcCenter(double timestep, double f);
	Vector3d CalcCenter(double timestep);
	void ScaleVel(Vector3d pnormal);
};

#endif
