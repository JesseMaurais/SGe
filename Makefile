CC = g++
RM = rm
MKDIR = mkdir
STDFLAGS = -std=c++17 -D_POSIX_C_SOURCE -I/usr/include/lua5.3 -I/usr/include/freetype2
DBGFLAGS = -Wall -Wextra -Winvalid-pch -g
DFLAGS = -MMD -MP
CFLAGS = $(DBGFLAGS) $(STDFLAGS) 
LIB = -ldl -lstdc++fs -lreadline -lX11 -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lSDL2_mixer -lGL -lGLU -lGLEW -lOpenCL -lopenal -lode -llua5.3 -lfreetype -lexpat -l3ds
ALL = 3ds.o Args.o Bezier.o BSP.o Crayon.o Desktop.o Document.o Event.o FreeType.o Hedron.o Height.o Lua.o Lux.o main.o Matrix.o Mesh.o Model.o Nurbs.o OpenAL.o OpenCL.o OpenGL.o Plane.o Quaternion.o SDL.o Shader.o Source.o Strings.o Stroke.o Surface.o Tessel.o Tree.o Vector.o XML.o
OBJDIR = obj/
SRCDIR = src/
PCH = $(SRCDIR)main.hpp
OBJ = $(addprefix $(OBJDIR), $(ALL))
DEP = $(OBJ:.o=.d)
BIN = SGe

all: $(OBJDIR) $(BIN)

clean:
	$(RM) $(BIN) $(OBJ) $(DEP) $(PCH).gch

$(OBJDIR):
	$(MKDIR) -p $(OBJDIR)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIB)
	
$(OBJDIR)%.o: $(SRCDIR)%.cpp $(PCH).gch
	$(CC) $(CFLAGS) $(DFLAGS) -include $(PCH) -c $< -o $@ 
		
$(PCH).gch: $(PCH)
	$(CC) $(CFLAGS) -c $< -o $@  

-include $(DEP)
 