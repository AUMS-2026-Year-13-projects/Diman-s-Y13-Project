CXX = g++
CC = gcc

TARGET = main

CXXFLAGS = -Iinclude
CFLAGS = -Iinclude
LDFLAGS = -Llib
LIBS = -lglfw3 -lopengl32 -lgdi32

CPP_SRC = src/main.cpp
C_SRC = src/glad.c

CPP_OBJ = $(CPP_SRC:.cpp=.o)
C_OBJ = $(C_SRC:.c=.o)

OBJ = $(CPP_OBJ) $(C_OBJ)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LIBS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q $(OBJ) $(TARGET).exe 2>nul || exit 0
