CC = g++-8
RM = rm
MKDIR = mkdir
STDFLAGS = -std=c++2a -Iinclude -I/usr/include/freetype2
DBGFLAGS = -Wall -Wextra -Wpedantic -Winvalid-pch -g
DFLAGS = -MMD -MP
CFLAGS = $(DBGFLAGS) $(STDFLAGS)
LIB = -ldl -lpthread -lstdc++fs -lreadline -lfreetype -lexpat -ljerry-core -ljerry-port-default -lSDL2 -lSDL2_image -lX11 -lGL -lGLU -lGLEW -lOpenCL -lopenal
ALL = Bezier.o BSP.o Crayon.o Desktop.o Event.o FreeType.o Hedron.o JavaScript.o main.o Matrix.o Mesh.o Model.o Nurbs.o OpenAL.o OpenCL.o OpenGL.o Plane.o Quaternion.o SDL.o Shader.o Signal.o Stream.o Strings.o Stroke.o Surface.o Tessel.o Tree.o Vector.o XML.o
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
