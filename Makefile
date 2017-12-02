CC = g++
RM = rm
MKDIR = mkdir
STDFLAGS = -std=c++17 -I/usr/include/freetype2
DBGFLAGS = -Wall -Wextra -Winvalid-pch -g
DFLAGS = -MMD -MP
CFLAGS = $(DBGFLAGS) $(STDFLAGS) 
LIB = -ldl -lpthread -lstdc++fs -lboost_system -lboost_filesystem -lreadline -lfreetype -lexpat -ljerry-core -lX11 -lSDL2 -lSDL2_image -lGL -lGLU -lGLEW -lOpenCL -lopenal -lode
ALL = Bezier.o BSP.o Crayon.o Desktop.o Event.o Files.o FreeType.o Hedron.o JavaScript.o main.o Manager.o Matrix.o Mesh.o Model.o Nurbs.o OpenAL.o OpenCL.o OpenGL.o Plane.o Quaternion.o SDL.o Shader.o Stream.o Strings.o Stroke.o Surface.o Tessel.o Tree.o Vector.o XML.o
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
 
