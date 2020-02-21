OBJS   = main.o

TARGETS = main

LIBS	=	-mwindows -mconsole -lopengl32 -lglu32  -static-libgcc -static-libstdc++ \
			-LC:\msys64\glew-2.0.0-win32\glew-2.0.0\lib\Release\x64 -lglew32 

INCS	=	-Wall -Wno-unused-but-set-variable \
			-Ic:/msys64 \

#all: $(TARGETS) 

main.exe: makefile $(OBJS) 
	g++ -o main.exe $(OBJS) $(LIBS)

main.o:main.cpp 
	g++	-c -o main.o main.cpp $(INCS)

clean:
	rm -f *.o *.out *.exe $(TARGETS)
