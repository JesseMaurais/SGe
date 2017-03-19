CC = g++
RM = rm
CFLAGS = -std=c++14 -g -MMD -MP
LIB = -ldl -lreadline -lX11 -lSDL2 -lGL -lGLU -lGLEW -lopenal -lode -llua -lexpat
ALL = Command.o Document.o Error.o Event.o Lua.o Lux.o main.o OpenGL.o SDL.o Window.o XML.o
OBJDIR = obj/
SRCDIR = src/
OBJ = $(addprefix $(OBJDIR), $(ALL))
DEP = $(OBJ:.o=.d)
BIN = SGe

all: $(BIN)

rebuild: clean all

clean:
	$(RM) $(BIN) $(OBJ) $(DEP)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIB)
	
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEP)
