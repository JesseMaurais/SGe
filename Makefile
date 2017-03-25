CC = g++
RM = rm
MKDIR = mkdir
STDFLAGS = -std=c++14
DBGFLAGS = -Wall -Wextra -g
DEPFLAGS = -MMD -MP
CFLAGS = $(STDFLAGS) $(DBGFLAGS) $(DEPFLAGS)
LIB = -ldl -lreadline -lX11 -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lSDL2_mixer -lGL -lGLU -lGLEW -lOpenCL -lopenal -lode -llua -lexpat -l3ds
ALL = 3ds.o Args.o Bezier.o BSP.o Command.o Crayon.o Document.o Event.o Fixed.o Hedron.o Height.o Lua.o Lux.o main.o Matrix.o Mesh.o Model.o Nurbs.o OpenAL.o OpenCL.o OpenGL.o Plane.o Quaternion.o SDL.o Stroke.o Surface.o Tessel.o Tree.o Vector.o Window.o XML.o
OBJDIR = obj/
SRCDIR = src/
PCH = $(SRCDIR)main.pch
OBJ = $(addprefix $(OBJDIR), $(ALL))
DEP = $(OBJ:.o=.d)
BIN = SGe

rebuild: clean all

all: $(OBJDIR) $(BIN)

clean:
	$(RM) $(BIN) $(OBJ) $(DEP) 

$(OBJDIR):
	$(MKDIR) -p $(OBJDIR)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIB)
	
$(OBJDIR)%.o: $(SRCDIR)%.cpp 
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEP)
