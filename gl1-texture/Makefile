OBJS   = main.o win.o

GLEW	= C:\glew-2.1.0

LIBS	=	-mwindows -mconsole -lopengl32 -lglu32  -static-libgcc -static-libstdc++ \
			-lglew32 -mwin32 \
			-L$(GLEW)/lib\Release/x64 \

INCS	=	-Wall -Wno-unused-but-set-variable \
			-Ic:/msys64 \
			-I$(GLEW)/include \

#all: $(TARGETS) 

main.exe: makefile $(OBJS) 
	g++ -o main.exe $(OBJS) $(LIBS)

main.o:main.cpp 
	g++	-c -o main.o main.cpp $(INCS)

clean:
	rm -f *.o *.out *.exe
