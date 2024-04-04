# This is the Makefile for Hello GLUT. It builds the Hello GLUT executable.

# Directories containing C headers. Edit to match path containing freeglut headers.
INCLUDE_DIRS = -I.\include -I"C:\Users\Cristhian Ocola\Desktop\freeglutinclude" -Isrc/grafo -Isrc/punto

# Directories containing import libraries. Edit to match path containing freeglut libs.
LIB_DIRS = -L"C:\Users\Cristhian Ocola\Desktop\freeglutlib"

# Warnings to be raised by the C compiler
WARNS = -Wall

# Names of tools to use when building
CC = gcc

# Compiler and linker flags
CFLAGS = -O3 -std=c99 -D _WIN32_IE=0x0500 -D WINVER=0x500 ${WARNS} ${INCLUDE_DIRS}
LDFLAGS = -s -lfreeglut -lopengl32 -lglu32 -Wl,--subsystem,windows ${LIB_DIRS}

# Object files to create Hello GLUT exe
OBJS = obj/main.o obj/grafo.o

.PHONY: all clean

all: bin/main.exe

clean:
	if exist obj\*.o            del obj\*.o
	if exist bin\main.exe  del bin\main.exe

obj:
	@if not exist obj mkdir obj

bin:
	@if not exist bin mkdir bin

bin/main.exe: ${OBJS} | bin
	${CC} -o "$@" ${OBJS} ${LDFLAGS}

obj/main.o: src/main.c | obj
	${CC} ${CFLAGS} -c $< -o $@

obj/grafo.o: src/grafo/grafo.c | obj
	${CC} ${CFLAGS} -c $< -o $@
