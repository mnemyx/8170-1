PROJECT
==================================================================

	Gina Guerrero
	
	CpSc8170 - Fall 2013
	
	Project #1 - Bouncing Ball in Box
	
	C++/OpenGL



DESCRIPTION
==================================================================

	A simulation of a ball bouncing in a box

	Model & Vector/Utility by Dr. House.
	Model Class modified to create an isosphere & plane

	Usage: ballbox [parameter_file]
	
	
KEY COMMANDS
==================================================================
	m or M		IMPORTANT: transforming camera view to simulation control (default: camera)
	p or P		switches from ortho to perspective views (default: perspective)
	q or ESC	quit
	
	
MOUSE COMMANDS
==================================================================
	SIMULATION:
	RMB			opens menu
	LMB 		triggers selected option
	
	CAMERA: 		  left button		 |	   middle button  	|		right button
	left drag	(-) rotation: model's y	 |  (+)-r: camera's y	|  (+) translation: camera's z
	right drag	(+) rotation: model's y	 |  (-)-r: camera's y	|  (-) translation: camera's z
	down drag	(+) rotation: model's x	 |  (+)-r: camera's x	|  (+) translation: camera's z
	up drag		(-) rotation: model's x	 |  (-)-r: camera's x	|  (-) translation: camera's z
	
	
MENU COMMANDS
==================================================================
	Continuous/Step		changes simulation between continuous or step
	Reset				resets the ball
	Clean				cleans the simulation history & resets ball
	Quit				quit


FILES
==================================================================
	ballbox.cpp (main program)
	Entity.cpp, Entity.h
	Model.cpp, Model.h
	State.cpp, State.h
	
	
MISC FILES
==================================================================
	README.md
	Makefile
	Utility.cpp, Utility.h
	Vector.cpp, Vector.h
	
	
SAMPLE OUTPUT FILES
==================================================================
	N/A
	

TOTAL FILE COUNT
==================================================================
	13