CC = g++
RM = rm
MKDIR = mkdir
CFLAGS = -std=c++14 -g -MMD -MP
LIB = -ldl -lreadline -lX11 -lSDL2 -lGL -lGLU -lGLEW -lopenal -lode -llua -lexpat -l3ds
ALL = 3ds.o Bezier.o Command.o Document.o Error.o Event.o Lua.o Lux.o main.o Matrix.o Mesh.o Model.o Nurbs.o OpenGL.o Plane.o Quaternion.o SDL.o Tessel.o Vector.o Window.o XML.o
OBJDIR = obj/
SRCDIR = src/
PCH = $(SRCDIR)main.pch
OBJ = $(addprefix $(OBJDIR), $(ALL))
DEP = $(OBJ:.o=.d)
BIN = SGe

rebuild: clean all

all: $(OBJDIR) $(BIN)

clean:
	$(RM) $(BIN) $(OBJ) $(DEP) $(PCH)

$(OBJDIR):
	$(MKDIR) -p $(OBJDIR)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIB)
	
$(OBJDIR)%.o: $(SRCDIR)%.cpp $(PCH)
	$(CC) $(CFLAGS) -c $< -o $@
		
$(SRCDIR)%.pch: $(SRCDIR)%.hpp
	$(CC) -c $< -o $@

-include $(DEP)
