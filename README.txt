README for cs410 pa4 Adrion Q Arkenberg

FILES:
main.cpp
matrix.{cpp,h}
model.{cpp,h}
camera.{cpp,h}
image.{cpp,h}
rgb.{cpp,h}
sphere.{cpp,h}
ray.{cpp,h}
light.{cpp,h}
material.{cpp,h}
point.{cpp,h}
polygon.{cpp,h}
world.{cpp,h}

STEPS
"make clean"
"make"

EXECUTABLE NAME: raytracer
USAGE: ./raytracer [cameraFile] [materialFile] [model1 ... modeln] [outputFile]

NOTE: my program is VERY slow, so give it a WHILE on larger inputs
NOTE: for a ballpark estimate on it’s speed:
	it ran sphere in about 10 minutes
	it ran 2 mirrors and a dodecahedron in 10seconds
	it ran a window and a dodecahedron in 10seconds
	it ran a clear cube, a sphere, and a pyramid, in 2 hours
