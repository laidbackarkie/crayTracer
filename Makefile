#Author: Adrion Q Arkenberg
#Makefile for pa3 410


#ADD TEST FILES WHEN CREATED

# List of files
C_SRCS		= main.cpp matrix.cpp model.cpp camera.cpp image.cpp rgb.cpp sphere.cpp ray.cpp light.cpp material.cpp point.cpp polygon.cpp world.cpp
T_SRCS		= test.cpp matrix.cpp model.cpp camera.cpp image.cpp rgb.cpp sphere.cpp ray.cpp light.cpp material.cpp point.cpp polygon.cpp world.cpp
C_OBJS		= main.o matrix.o model.o camera.o image.o rgb.o sphere.o ray.o light.o material.o point.o polygon.o world.o
T_OBJS		= test.o matrix.o model.o camera.o image.o rgb.o sphere.o ray.o light.o material.o point.o polygon.o world.o
C_HEADERS	= matrix.h model.h camera.h image.h rgb.h sphere.h ray.h light.h material.h point.h polygon.h world.h

OBJS		= ${C_OBJS}
TOBJS		= ${T_OBJS}
EXE	    = raytracer
TEXE		= testRayTracer

# Compiler and loader commands and flags
GXX	= g++
GXX_FLAGS	= -I. -Wall -g $(C_SRCS) –o $(EXE)
TGXX_FLAGS = -I. -Wall -g $(T_SRCS) –o $(TEXE)
LD_FLAGS	= -I. -Wall -g
TST_FLAGS = -I. -Wall -g -lgtest -lpthread

# Compile .c files to .o files
.c.o:
	$(GXX) ${GXX_FLAGS} $<
	$(GXX) ${TGXX_FLAGS} $<

# Target is the executable
default: $(OBJS)
	$(GXX) $(LD_FLAGS) $(OBJS) -o $(EXE)

# target is the google test executable
tests: $(TOBJS)
	$(GXX) $(TST_FLAGS) $(TOBJS) -o $(TEXE)

# Recompile C objects if headers change
${C_OBJS}: ${C_HEADERS}

# Recompile Test object files if headers change
${T_OBJS}: ${C_HEADERS}

# Make target .tar file to package and move project
target:
	tar cf $(EXE).tar *.cpp *.h Makefile README.txt

# Clean up the directory
clean:
	rm -f *.o *~ $(EXE) *.tar $(TEXE)
