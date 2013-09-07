#----------------------------------------------
#  Makefile for CpSc8170 - Proj1: Bouncing Balls
#----------------------------------------------
#
#  Gina Guerrero
#  August 28, 2013
#

CC      = g++
C	= cpp
H	= h
CFLAGS 	= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lm -L/usr/local/lib -lGLU
  endif
endif

HFILES 	= Matrix.${H} Vector.${H} Utility.${H} Model.${H} State.${H} Entity.${H}
OFILES 	= Matrix.o Vector.o Utility.o Model.o State.o Entity.o
PROJECT = ballbox

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

Entity.o: Entity.${C} Entity.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} Entity.${C}
	
State.o: State.${C} State.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} State.${C}

Model.o: Model.${C} Model.${H}
	${CC} ${CFLAGS} -c ${INCFLAGS} Model.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}
