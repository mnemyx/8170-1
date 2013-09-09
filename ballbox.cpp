/********************************************************
  ballbox.cpp
  
  CPSC8170 - Proj 1   GBG   8/2013
*********************************************************/

#include "Vector.h"
#include "Entity.h"

#include <cstdlib>
#include <cstdio>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

#define WINDOW_WIDTH	800	/* window dimensions */
#define WINDOW_HEIGHT	600

	/* RGB colors */

#define RGBBLACK	0, 0, 0
#define RGBRED		1, 0, 0
#define RGBORANGE	1, 0.5, 0
#define RGBYELLOW	1, 1, 0
#define RGBGREEN	0, 1, 0
#define RGBBLUE		0.5, 0.5, 1
#define RGBVIOLET	1, 0, 0.5
#define RGBWHITE	1, 1, 1
#define RGBDIMWHITE	0.8, 0.8, 0.8
#define RGBDIMRED	0.3, 0, 0
#define RGBDIMGREEN	0, 0.3, 0
#define RGBDIMLUE	0, 0, 0.3

#define EPS		0.1

#define MAXSTEPS	10000

#define MenuContinuous	1
#define MenuReset	2
#define MenuClean	3
#define MenuQuit	4

#define NOAIR		0
#define LIGHT		1
#define MEDIUM		2
#define HEAVY		3

#define NEAR		10	// distance of near clipping plane
#define FAR		1000	// distance of far clipping plane
#define DEPTH		-500	// initial z coord. of center of cube

#define ORTHO		0	// projection system codes
#define PERSPECTIVE	1
#define NONE		-1	// used to indicate no mouse button pressed

#define ROTFACTOR	0.2     // degrees rotation per pixel of mouse movement
#define XLATEFACTOR	0.5     // units of translation per pixel of mouse movement

#define DRAWWIDTH	200	// view volume sizes (note: width and
#define DRAWHEIGHT	150	//   height should be in same ratio as window)

#define AMBIENT_FRACTION 0.1
#define DIFFUSE_FRACTION 0.4
#define SPECULAR_FRACTION 0.4

// colors used for lights, and materials for coordinate axes
const float DIM_PALEBLUE[] = {0.1, 0.1, 0.3, 1};
const float BRIGHT_PALEBLUE[] = {0.5, 0.5, 1, 1};
const float GRAY[] = {0.3, 0.3, 0.3, 1};
const float WHITE[] = {1, 1, 1, 1};
const float DIM_WHITE[] = {0.8, 0.8, 0.8, 1};
const float DIM_RED[] = {0.3, 0, 0, 1};
const float RED[] = {1, 0, 0, 1};
const float DIM_GREEN[] = {0, 0.3, 0, 1};
const float GREEN[] = {0, 1, 0, 1};
const float DIM_BLUE[] = {0, 0, 0.3, 1};
const float BLUE[] = {0, 0, 1, 1};

enum COLOR{MAT_WHITE, MAT_RED, MAT_ORANGE, MAT_YELLOW, MAT_GREEN, MAT_BLUE, MAT_VIOLET};

enum SHADEMODE{AMBIENT, DIFFUSE, SPEC_BROAD, SPEC_TIGHT, ALL};

// colors indexed for cycling material colors
float hues[][3] = { {1, 1, 1},    // white
		    {1, 0, 0},    // red
		    {1, 0.5, 0},  // orange
		    {1, 1, 0},    // yellow
		    {0, 1, 0},    // green
		    {0, 0, 1},    // blue
		    {0.5, 0, 1}   // violet
		  };

//
// Global variables updated and shared by callback routines
//
// Viewing parameters
static int Projection;
// Camera position and orientation
static double Pan;
static double Tilt;
static double Approach;

// model orientation
static double ThetaX;
static double ThetaY;

// global variables to track mouse and shift key
static int MouseX;
static int MouseY;
static int Button = NONE;

// global variables to track wireframe/shaded mode and material color
static int Color;
static int SmoothShading;
static int ShadeMode;
static int Wireframe;

static int MenuAttached;

//
// From Bounce
//

int FrameNumber = 0;

void TimerCallback(int value);

static double WinWidth = WINDOW_WIDTH;
static double WinHeight = WINDOW_HEIGHT;

static char *ParamFilename = NULL;

static int MiddleButton = false;
static Vector2d LastMouse(0, 0);

static double TimeStep;
static double DispTime;
static int TimeStepsPerDisplay;
static int TimerDelay;

static int NSteps = 0;
static int NTimeSteps = -1;
static double Time = 0;

Entity Particle;
Entity Cube[6];



//************************** GOD I NEED TO CLEAN THIS UP PLEASE *************************//

// 
// Get the shading setup for the objects 
//
void GetShading(int wireframe) {
  float ambient_color[4];
  float diffuse_color[4];
  float specular_color[4];
  int shininess;
  
  if(wireframe) {
    // set drawing color to current hue, and draw with thicker wireframe lines
    glColor3f(hues[Color][0], hues[Color][1], hues[Color][2]);
    glLineWidth(2);
  } else {
    // set up material colors to current hue.
    for(int i = 0; i < 3; i++)
      ambient_color[i] = diffuse_color[i] = specular_color[i] = 0;
    ambient_color[3] = diffuse_color[3] = specular_color[3] = 1;
    shininess = 1;
    
    switch(ShadeMode){
      case AMBIENT:
	for(int i = 0; i < 3; i++)
	  ambient_color[i] = AMBIENT_FRACTION * 3 * hues[Color][i];
	break;
	
      case DIFFUSE:
	for(int i = 0; i < 3; i++){
	  ambient_color[i] = AMBIENT_FRACTION * hues[Color][i];
	  diffuse_color[i] = DIFFUSE_FRACTION * 2 * hues[Color][i];
	}
	break;
	
      case SPEC_BROAD:
	for(int i = 0; i < 3; i++){
	  ambient_color[i] = AMBIENT_FRACTION * hues[Color][i];
	  specular_color[i] = SPECULAR_FRACTION * 2 * hues[MAT_WHITE][i];
	  shininess = 10;
	}
	break;
	
      case SPEC_TIGHT:
	for(int i = 0; i < 3; i++){
	  ambient_color[i] = AMBIENT_FRACTION * hues[Color][i];
	  specular_color[i] = SPECULAR_FRACTION * 2 *hues[MAT_WHITE][i];
	  shininess = 60;
	}
	break;
	
      case ALL:
	for(int i = 0; i < 3; i++){
	  ambient_color[i] = AMBIENT_FRACTION * hues[Color][i];
	  diffuse_color[i] = DIFFUSE_FRACTION * hues[Color][i];
	  specular_color[i] = SPECULAR_FRACTION * hues[MAT_WHITE][i];
	  shininess = 60;
	}
	break;
    }
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
  }
}


//
// Draw the moving objects
//
void DrawMovingObj(int wireframe) {
  Particle.UpdateModel();
  Particle.Draw(wireframe);
}

//
// Draw the non moving objects
//
void DrawNonMovingObj(int wireframe) {
  int i;
  for ( i = 0; i < 6; i++ ) {
	  Cube[i].Draw(wireframe);
  }
}


//
//  Draw the ball, its traces and the floor if needed
//
void DrawScene(int collision, int cubeIndx){
  
  int i,j;
  Model p;

  glClear(GL_COLOR_BUFFER_BIT);
  
  glColor3f(RGBORANGE);
  DrawMovingObj(false);
  
  if(collision) glColor3f(RGBRED);
  DrawNonMovingObj(true);
  
  glutSwapBuffers();

  if(NSteps < MAXSTEPS){
	Cube[cubeIndx].AddOCenter(NSteps);
	Cube[cubeIndx].AddOCollision(collision, NSteps);
    NSteps++;
  }
  else{
    cerr << "Ball position table overflow, restarting!!" << endl;
    Time = 0;
    NSteps = 0;
  }
}


///
//  Run a single time step in the simulation
//
void Simulate(){
  int i;
  Vector3d acceleration;
  Vector3d newvelocity, newball;
  double tn = 0;
  double f;
  
  // don't do anything if our moving object isn't, well, moving.
  if(Particle.Stopped()) { cout << "I STOPPED!!" << endl;
    return;
  }
  // set the ball's resting...for all the planes...
  for (i = 0; i < 6; i++) {
	  cout << "I FOR RESTINGONPLANE(): ------------ " << i << endl;
	  Cube[i].RestingOnPlane(Particle.Center(), Particle.Velocity(), Particle.Radius(), TimeStep);
  }

  // get the new acceleration
  //out << "particle at start accel: "; Particle.Acceleration().print(); out << endl;
  Particle.Accel();

  // if ball in resting contact, and its acceleration is zero or down in the direction of the plane...
  // then cancel the velocity in the direction of the normal and...uhm...set the ball down on the plane...?
  // Clear resting contact if acceleration is up.
  for (i = 0; i < 6; i++) {
	  if(Cube[i].Rest() && Cube[i].AccelOnPlane(Particle.Acceleration())) {
		  cout << i << " -------------------------- I NEED TO REST KTHX" << endl;
		  Particle.AdjustAVC(Cube[i].PlaneNormal(), Cube[i].PlaneVertex());
	  } 
	  else Cube[i].Rest(false);
  }
  
  
  // evil Euler integration to get velocity and position at next timestep
  newvelocity = Particle.CalcVelocity(TimeStep); 
  newball = Particle.CalcCenter(TimeStep);
  cout << "vel @ expected end ts: ";
				newvelocity.print(); cout << endl;
			  cout << "pos @ expected end ts: "; newball.print(); cout << endl;
  
  i = 0;
  tn = TimeStep;
  int checkCollision = 1;
  int hadCollision = 0;
  int cubeCollisions[6] = {0, 0, 0, 0, 0, 0};
  // while loop should wrap around here, to determine if we're still under the time stamp.
  while (checkCollision) { 
	for (i = 0; i < 6; i++ ) {  
	  f = Cube[i].PlaneBallColl(Particle.Center(), newvelocity, newball, Particle.Radius());
	  
	  if (cubeCollisions[i]) { cubeCollisions[i] = 0; }
	  else {
		  cout << " F from PlabeBallColl(): " << f << endl;
		  // if ball not in resting contact, check for collision in the timestep
		  if (!Cube[i].Rest() && f >= 0 - Cube[i].FudgeFactor()  && f < 1 + Cube[i].FudgeFactor() ) {
				  hadCollision = 1;
				  cubeCollisions[i] = 1;
				  
				  cout << "Im not resting and I uh, collided @ -------> " << i << endl;
				  // have collision. get fraction of timestep at this collision will occur.
				  
				  // compute the velocity & position of the ball at the collision time
				  newvelocity = Particle.CalcVelocity(tn, f);
				  newball = Particle.CalcCenter(tn, f);
				  cout << "vel @ collision: ";
					newvelocity.print(); cout << endl;
				  cout << "pos @ collision: "; newball.print(); cout << endl;
				  
				  // reflect the velocity from the floor & scale the vertical component...
				  Particle.Velocity(newvelocity);
				  cout << "particle not scaled velocity: "; Particle.Velocity().print(); cout << endl;
				  Particle.ScaleVel(Cube[i].PlaneNormal());  // stores this into Particle->Velocity
				  Particle.Center(newball);
				  
				  cout << "particle scaled velocity: "; Particle.Velocity().print(); cout << endl;
				  cout << "particle center: "; Particle.Center().print(); cout << endl;

				  // draw the scene because we collided (change so that plane lights up instead)
				  DrawScene(1, i);
				  
				  // finish intergrating over the remainder of the time step...
				  Particle.Accel();
				  //out << "particle end of ts accel: "; Particle.Acceleration().print(); out << endl;
				  newvelocity = Particle.CalcVelocity(tn, 1 - (tn * f));
				  newball = Particle.CalcCenter(tn, 1 - (tn * f));
				  
				  tn -= tn * f;
				  //i = 7;
				  cout << "TN: _____________________________________ " << tn << endl;
				  //Particle.Velocity(newvelocity);
				  //Particle.Center(newball);
				  	//cout << "****** particle end ts IN LOOP velocity: "; Particle.Velocity().print(); cout << endl;
			  //cout << "****** particle end ts IN LOOP center: "; Particle.Center().print(); cout << endl;
		  }

		}
		
	}
	
	if(hadCollision) {
		hadCollision = 0;
		if(tn > 0) checkCollision = 1;
		else checkCollision = 0;
	} else checkCollision = 0;
  }
  
  
  // advance the real timestep and set the velocity and position to their new values
  Time += TimeStep;
  NTimeSteps++;
  Particle.Velocity(newvelocity);
  Particle.Center(newball);
				cout << "****** particle end ts  velocity: "; Particle.Velocity().print(); cout << endl;
			  cout << "****** particle end ts center: "; Particle.Center().print(); cout << endl;
  ////////////

  // draw only if we are at a display time
  if(NTimeSteps % TimeStepsPerDisplay == 0)
    DrawScene(0,0);

  // set up time for next timestep if in continuous mode
  glutIdleFunc(NULL);
  if(Particle.Step())
    Particle.Stopped(true);
  else{
    Particle.Stopped(false);
    glutTimerFunc(TimerDelay, TimerCallback, 0);
  }
  
}

//
//  Run a single time step in the simulation
//
void TimerCallback(int){
  Simulate();
}

//
//  Adjust mouse coordinates to match window coordinates
//
void AdjustMouse(int& x, int& y){

  /* reverse y, so zero at bottom, and max at top */
  y = int(WinHeight - y);

  /* rescale x, y to match current window size (may have been rescaled) */
  y = int(y * WINDOW_HEIGHT / WinHeight);
  x = int(x * WINDOW_WIDTH / WinWidth);
}

//
//  Watch mouse motion 
//
void handleMotion(int x, int y){
  if(MenuAttached) {
    Vector2d mouse;

    if(!MiddleButton)
      return;

    AdjustMouse(x, y);	/* adjust mouse coords to current window size */
    mouse.set(x, y);

    LastMouse = mouse;
  } else {
    int delta;
  
    y = -y;
    int dy = y - MouseY;
    int dx = x - MouseX;
  
    switch(Button){
      case GLUT_LEFT_BUTTON:
        ThetaX -= ROTFACTOR * dy;
        ThetaY += ROTFACTOR * dx;
        glutPostRedisplay();
        break;
      case GLUT_MIDDLE_BUTTON:
        Pan -= ROTFACTOR * dx;
        Tilt += ROTFACTOR * dy;
        glutPostRedisplay();
        break;
      case GLUT_RIGHT_BUTTON:
        delta = (fabs(dx) > fabs(dy)? dx: dy);
        Approach += XLATEFACTOR * delta;
        glutPostRedisplay();
        break;
    }
  
    MouseX = x;
    MouseY = y;
  }
}

//
//  Watch mouse button presses and handle them
//
void handleButton(int button, int state, int x, int y){
  
  if(MenuAttached) {
  
    if(button == GLUT_MIDDLE_BUTTON)
      MiddleButton = (state == GLUT_DOWN);

    if(button != GLUT_LEFT_BUTTON)
      return;

    AdjustMouse(x, y);	/* adjust mouse coords to current window size */

    if(state == GLUT_UP){
      if(Particle.Start()){
        Particle.Start(false);
        Particle.Stopped(false);
        Particle.Center(Particle.InitialCenter());
	    Particle.Velocity(Particle.InitialVelocity());
        DrawScene(0,0);
        glutIdleFunc(Simulate);   
      }
      else if(Particle.Stopped()){
        Particle.Stopped(false);
        glutIdleFunc(Simulate);    
      }
      else{
        Particle.Stopped(true);
        glutIdleFunc(NULL);    
      }
    }
  } else {
  
    if(state == GLUT_UP)
      Button = NONE;		// no button pressed
    else{
      MouseY = -y;		// invert y window coordinate to correspond with OpenGL
      MouseX = x;
    
      Button = button;		// store which button pressed
    }
    
  }
}

//
//  On Redraw request, erase the window and redraw everything
//
void drawDisplay(){
  // distant light source, parallel rays coming from front upper right
  const float light_position[] = {1, 1, 1, 0};
  
  // clear the window to the background color
  glClear(GL_COLOR_BUFFER_BIT);

  if(!Wireframe)
    glClear(GL_DEPTH_BUFFER_BIT);  // solid - clear depth buffer
  
  if(!Wireframe && SmoothShading)  // establish shading model, flat or smooth
    glShadeModel(GL_SMOOTH);
  else
    glShadeModel(GL_FLAT);
    
  // light is positioned in camera space so it does not move with object
  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, WHITE);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
  glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
  
  // establish camera coordinates
  glRotatef(Tilt, 1, 0, 0);	    // tilt - rotate camera about x axis
  glRotatef(Pan, 0, 1, 0);	    // pan - rotate camera about y axis
  glTranslatef(0, 0, Approach);     // approach - translate camera along z axis
  
  // rotate the model
  glRotatef(ThetaY, 0, 1, 0);       // rotate model about x axis
  glRotatef(ThetaX, 1, 0, 0);       // rotate model about y axis  
    
  DrawScene(0, 0);
  
  glutSwapBuffers();
}


//
//  Load parameter file and reinitialize global parameters
//
void LoadParameters(char *filename){
  
  FILE *paramfile;
  
  Vector3d bvelocity;
  Vector3d bcenter;
  double bmass, bradius, coeffr, coefff, beps, viscosity;
  Vector3d wind;
  Vector3d gravity;
  
  Vector3d plane1, plane2, plane3, plane4, plane5, plane6;
  Vector3d plcen1, plcen2, plcen3, plcen4, plcen5, plcen6;
  double peps1, peps2, peps3, peps4, peps5, peps6;
	
  if((paramfile = fopen(filename, "r")) == NULL){
    fprintf(stderr, "error opening parameter file %s\n", filename);
    exit(1);
  }

  ParamFilename = filename;

 if(fscanf(paramfile, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	&(bvelocity.x), &(bvelocity.y), &(bvelocity.z), &(bcenter.x), &(bcenter.y), &(bcenter.z), 
	&bmass, &bradius, &coeffr, &coefff, &beps,
	&(wind.x), &(wind.y), &(wind.z), &(gravity.x), &(gravity.y), &(gravity.z), &viscosity, 
	&(plane1.x), &(plane1.y), &(plane1.z), &(plcen1.x), &(plcen1.y), &(plcen1.z), &peps1, 
	&(plane2.x), &(plane2.y), &(plane2.z), &(plcen2.x), &(plcen2.y), &(plcen2.z), &peps2, 
	&(plane3.x), &(plane3.y), &(plane3.z), &(plcen3.x), &(plcen3.y), &(plcen3.z), &peps3, 
	&(plane4.x), &(plane4.y), &(plane4.z), &(plcen4.x), &(plcen4.y), &(plcen4.z), &peps4, 
	&(plane5.x), &(plane5.y), &(plane5.z), &(plcen5.x), &(plcen5.y), &(plcen5.z), &peps5, 
	&(plane6.x), &(plane6.y), &(plane6.z), &(plcen6.x), &(plcen6.y), &(plcen6.z), &peps6,
	&TimeStep, &DispTime) != 62){
    fprintf(stderr, "error reading parameter file %s\n", filename);
    fclose(paramfile);
    exit(1);
  }

  Particle.InitState(bvelocity, bcenter, bmass, bradius, coeffr, coefff, beps, viscosity, wind, gravity);
  Cube[0].InitState(plane1, plcen1, peps1);
  Cube[1].InitState(plane2, plcen2, peps2);
  Cube[2].InitState(plane3, plcen3, peps3);
  Cube[3].InitState(plane4, plcen4, peps4);
  Cube[4].InitState(plane5, plcen5, peps5);
  Cube[5].InitState(plane6, plcen6, peps6);
  
  TimeStepsPerDisplay = Max(1, int(DispTime / TimeStep + 0.5));
  TimerDelay = int(0.5 * TimeStep * 1000);
}

//
// Initialize Camera
//
void InitCamera() {
  Projection = ORTHO;
  
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  Color = MAT_WHITE;
  ShadeMode = AMBIENT;
  SmoothShading = false;
  MenuAttached = false;
  Wireframe = true;
  
  Pan = 0;
  Tilt = 0;
  Approach = DEPTH;
  
  ThetaX = 0;
  ThetaY = 0;
}

//
// Set up the projection matrix to be either orthographic or perspective
//
void updateProjection(){
  
  // initialize the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  // determine the projection system and drawing coordinates
  if(Projection == ORTHO)
    glOrtho(-DRAWWIDTH/2, DRAWWIDTH/2, -DRAWHEIGHT/2, DRAWHEIGHT/2, NEAR, FAR);
  else{
    // scale drawing coords so center of cube is same size as in ortho
    // if it is at its nominal location
    double scale = fabs((double)NEAR / (double)DEPTH);
    double xmax = scale * DRAWWIDTH / 2;
    double ymax = scale * DRAWHEIGHT / 2;
    glFrustum(-xmax, xmax, -ymax, ymax, NEAR, FAR);
  }
  
  // restore modelview matrix as the one being updated
  glMatrixMode(GL_MODELVIEW);
}

//
//  On Reshape request, reshape viewing coordinates to keep the viewport set
//  to the original window proportions and to keep the window coordinates fixed
//
void doReshape(int w, int h){
  
  glViewport(0, 0, w, h);
  WinWidth = w;
  WinHeight = h;
  
  updateProjection();
}


//
//  Initialize the Simulation
//
void InitSimulation(int argc, char* argv[]){

  if(argc != 2){
    fprintf(stderr, "usage: bounce paramfile\n");
    exit(1);
  }
  
  LoadParameters(argv[1]);

  NSteps = 0;
  NTimeSteps = -1;
  Time = 0;
}



//
// Routine to restart the ball at the top
//
void RestartBall(){

  LoadParameters(ParamFilename); // reload parameters in case changed
  
  Particle.Start(true);
  Particle.Stopped(true);
  
  NTimeSteps = -1;
  glutIdleFunc(NULL);
  Time = 0;
  
  Particle.Center(Particle.InitialCenter());
  Particle.Velocity(Particle.InitialVelocity());
    
  DrawScene(0, 0);
}

/*
  Menu callback
*/
void HandleMenu(int index){

  switch(index){

  case MenuContinuous:
    if(Particle.Step()){
      Particle.Step(false);
      glutChangeToMenuEntry(index, "Step", index);
    }
    else{
      Particle.Step(true);
      glutChangeToMenuEntry(index, "Continuous", index);
    }  
    break;  

  case MenuReset:
    RestartBall();
    break;

  case MenuClean:
    NSteps = 0;
    RestartBall();
    break;

  case MenuQuit:
    exit(0);

  }
}


//
//  Set up pop-up menu on right mouse button
//
void MakeMenu(){

  int id = glutCreateMenu(HandleMenu);

  glutAddMenuEntry("Continuous", MenuContinuous);
  glutAddMenuEntry("Reset", MenuReset);
  glutAddMenuEntry("Clean", MenuClean);
  glutAddMenuEntry("Quit", MenuQuit);

  glutSetMenu(id);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//
// Keypress handling
//
void handleKey(unsigned char key, int x, int y){
  
  switch(key){
    case 'q':		// q - quit
    case 'Q':
    case 27:		// esc - quit
      exit(0);
    case 'p':			// P -- toggle between ortho and perspective
    case 'P':
      Projection = !Projection;
      updateProjection();
      glutPostRedisplay();
      break;
    case 'c':			// M -- cycle through material colors
    case 'C':
      Color = (Color == MAT_VIOLET? MAT_WHITE: Color + 1);
      glutPostRedisplay();
      break;
      
    case 'h':			// H -- cycle through shading modes
    case 'H':
      ShadeMode = (ShadeMode == ALL? AMBIENT: ShadeMode + 1);
      switch(ShadeMode){
	case AMBIENT:
	  cout << "Ambient" << endl;
	  break;
	case DIFFUSE:
	  cout << "Diffuse" << endl;
	  break;
	case SPEC_BROAD:
	  cout << "Specular - broad" << endl;
	  break;
	case SPEC_TIGHT:
	  cout << "Specular - tight" << endl;
	  break;
	case ALL:
	  cout << "A + D + S" << endl;
	  break;
      }
      glutPostRedisplay();
      break;
    case 's':			// S -- toggle between flat and smooth shading
    case 'S':
      SmoothShading = !SmoothShading;
      glutPostRedisplay();
      break;
      
    case 'w':			// M -- toggle between wireframe and shaded viewing
    case 'W':
      Wireframe = !Wireframe;
      if(Wireframe){
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
      }
      else{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
      }
      glutPostRedisplay();
      break;
    case 'm':
    case 'M':
      MenuAttached = !MenuAttached;
      if(MenuAttached) { MakeMenu(); }
      else glutDetachMenu(GLUT_RIGHT_BUTTON);
      break;
    default:		// not a valid key -- just ignore it
      return;
  }
  
  glutPostRedisplay();
}

/*
   Main program to set up display
*/
int main(int argc, char* argv[]){
  
  glutInit(&argc, argv);

  InitSimulation(argc, argv);
  InitCamera();

  /* open window and establish coordinate system on it */
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Bouncing Ball in Box Demo");

  /* register display and mouse-button callback routines */
  glutReshapeFunc(doReshape);
  glutDisplayFunc(drawDisplay);
  glutMouseFunc(handleButton);
  glutMotionFunc(handleMotion);
  glutKeyboardFunc(handleKey);

  /* Set up to clear screen to black */
  glClearColor(RGBBLACK, 0);

  /* Set shading to flat shading */
  glShadeModel(GL_FLAT);


  glutMainLoop();
  return 0;
}
