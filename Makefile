CC = c++
CFLAGS = -std=c++11 -g -c
OBJ = 3ds.o Bezier.o BSP.o Crayon.o Event.o Font.o FreeType.o Geom.o GUI.o Height.o Lua.o Lux.o Lwo.o main.o Matrix.o Mesh.o Model.o Nurbs.o Obj.o ODE.o OpenAL.o OpenGL.o Plane.o Ply.o Quaternion.o Render.o SDL.o Shader.o SkyBox.o Sphere.o Stroke.o Tessel.o Texture.o Tree.o VBO.o Vector.o Widget.o

all: $(OBJ)
	c++ $(OBJ) -o SGe -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lSDL2_net -lGLEW -lopenal -lode -l3ds -llwo -lrply -llua5.2 -lexpat

clean:
	rm *.o SGe

3ds.o: src/3ds.cpp src/3ds.hpp
	$(CC) $(CFLAGS) src/3ds.cpp

Bezier.o: src/Bezier.cpp src/Bezier.hpp
	$(CC) $(CFLAGS) src/Bezier.cpp

BSP.o: src/BSP.cpp src/BSP.hpp
	$(CC) $(CFLAGS) src/BSP.cpp

Crayon.o: src/Crayon.cpp src/Crayon.hpp
	$(CC) $(CFLAGS) src/Crayon.cpp

Event.o: src/Event.cpp src/Event.hpp
	$(CC) $(CFLAGS) src/Event.cpp

Font.o: src/Font.cpp src/Font.hpp
	$(CC) $(CFLAGS) src/Font.cpp

FreeType.o: src/FreeType.cpp src/FreeType.hpp
	$(CC) $(CFLAGS) src/FreeType.cpp

Geom.o: src/Geom.cpp src/Geom.hpp
	$(CC) $(CFLAGS) src/Geom.cpp

GUI.o: src/GUI.cpp src/GUI.hpp
	$(CC) $(CFLAGS) src/GUI.cpp

Height.o: src/Height.cpp src/Height.hpp
	$(CC) $(CFLAGS) src/Height.cpp

Lua.o: src/Lua.cpp src/Lua.hpp
	$(CC) $(CFLAGS) src/Lua.cpp

Lux.o: src/Lux.cpp src/Lux.hpp src/Geom.hpp src/Shader.hpp
	$(CC) $(CFLAGS) src/Lux.cpp

Lwo.o: src/Lwo.cpp src/Lwo.hpp
	$(CC) $(CFLAGS) src/Lwo.cpp

main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp

Matrix.o: src/Matrix.cpp src/Matrix.hpp
	$(CC) $(CFLAGS) src/Matrix.cpp

Mesh.o: src/Mesh.cpp src/Mesh.hpp
	$(CC) $(CFLAGS) src/Mesh.cpp

Model.o: src/Model.cpp src/Model.hpp
	$(CC) $(CFLAGS) src/Model.cpp

Nurbs.o: src/Nurbs.cpp src/Nurbs.hpp
	$(CC) $(CFLAGS) src/Nurbs.cpp

Obj.o: src/Obj.cpp src/Obj.hpp
	$(CC) $(CFLAGS) src/Obj.cpp

ODE.o: src/ODE.cpp src/ODE.hpp
	$(CC) $(CFLAGS) src/ODE.cpp

OpenAL.o: src/OpenAL.cpp src/OpenAL.hpp
	$(CC) $(CFLAGS) src/OpenAL.cpp

OpenGL.o: src/OpenGL.cpp src/OpenGL.hpp
	$(CC) $(CFLAGS) src/OpenGL.cpp

Plane.o: src/Plane.cpp src/Plane.hpp
	$(CC) $(CFLAGS) src/Plane.cpp

Ply.o: src/Ply.cpp src/Ply.hpp
	$(CC) $(CFLAGS) src/Ply.cpp

Quaternion.o: src/Quaternion.cpp src/Quaternion.hpp
	$(CC) $(CFLAGS) src/Quaternion.cpp

Render.o: src/Render.cpp src/Render.hpp
	$(CC) $(CFLAGS) src/Render.cpp

SDL.o: src/SDL.cpp src/SDL.hpp
	$(CC) $(CFLAGS) src/SDL.cpp

Shader.o: src/Shader.cpp src/Shader.hpp
	$(CC) $(CFLAGS) src/Shader.cpp

SkyBox.o: src/SkyBox.cpp src/SkyBox.hpp
	$(CC) $(CFLAGS) src/SkyBox.cpp

Sphere.o: src/Sphere.cpp src/Sphere.hpp
	$(CC) $(CFLAGS) src/Sphere.cpp

Stroke.o: src/Stroke.cpp src/Stroke.hpp
	$(CC) $(CFLAGS) src/Stroke.cpp

Tessel.o: src/Tessel.cpp src/Tessel.hpp
	$(CC) $(CFLAGS) src/Tessel.cpp

Texture.o: src/Texture.cpp src/Texture.hpp
	$(CC) $(CFLAGS) src/Texture.cpp

Tree.o: src/Tree.cpp src/Tree.hpp
	$(CC) $(CFLAGS) src/Tree.cpp

VBO.o: src/VBO.cpp src/VBO.hpp
	$(CC) $(CFLAGS) src/VBO.cpp

Vector.o: src/Vector.cpp src/Vector.hpp
	$(CC) $(CFLAGS) src/Vector.cpp

Widget.o: src/Widget.cpp src/GUI.cpp src/GUI.hpp src/Event.cpp src/Event.hpp
	$(CC) $(CFLAGS) src/Widget.cpp
