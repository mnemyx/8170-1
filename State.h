/*
  State.h
  
  Header file for State Class
  
  CPSC8170 - Proj 1   GBG   8/2013
*/

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
#define SPDEPTH		3


class State{
  private:
    
    Vector3d Velocity;
    Vector3d V0;
    Vector3d Acceleration;
    
    Vector3d Center;
    double Mass;
    float Radius;
    
    int Start;
    int Stopped;
    int Step;
    int Resting;
    
    double CoeffofRestitution;
    double CoeffofFriction;
    Vector3d EPS;
    
    int Collision[MAXSTEPS];
    Vector3d OldCenter[MAXSTEPS];
    
    Model Particle;
    
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
    void SetAcceleration(Vector3d a);
    void SetCenter(Vector3d c);
    void SetCenter(float x, float y, float z);
    void SetCX(float x);
    void SetCY(float y);
    void SetCZ(float z);
    void SetMass(double m);
    void SetRadius(float r);
    void SetStart(int start);
    void SetStopped(int stop);
    void SetStep(int step);
    void SetCoeffR(double cor);
    void SetCoeffF(double cof);
    void SetEPS(Vector3d eps);
    void SetResting(double timestep, float miny);
    void SetResting(int type);
    void UpdateObj(int current, int indx = 0);
    void AddCollision(int collision, int indx);
    void AddOldCenter(Vector3d cold, int indx);
    
    // Getters
    Vector3d GetVelocity();
    Vector3d GetInitialVelocity();
    Vector3d GetAcceleration();
    Vector3d GetCenter();
    double GetMass();
    double GetRadius();
    int IsStarted();
    int IsStopped();
    int IsStep();
    int IsResting();
    double GetCoeffR();
    double GetCoeffF();
    Vector3d GetEPS();
    int Collided(int i = 0);
    
    // Functions
    void DrawParticle(int wireframe = 0);
   
};

#endif
