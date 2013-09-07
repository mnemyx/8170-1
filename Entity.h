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

class Entity : class Model {		// entity is a model that has a state...
  private:
    State EntState;		// the entity's associated state variables
	
  public:
    // Constructor
    Entity();
    
	// Setters
	
	// Getters
	
	// Functions
	float PlaneBallColl(Vector3d bCenter, Vector3d bVelocity, Vector3d bNewCenter, float bRadius);
	void RestingOnPlane(Vector3d bCenter, Vector3d bVelocity, float bRadius, double timeStep);
};

#endif
