LIBS=-lGL -lGLU -lglut -lGLEW
CC=g++

SOURCES=main_file.cpp tga.cpp shaderprogram.cpp moje.cpp
HEADERS=tga.h shaderprogram.h moje.h
OBJECTS=$(SOURCES:.cpp=.o)

all: main_file

main_file: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@
	
clean:
	-rm -rf *.o main_file
