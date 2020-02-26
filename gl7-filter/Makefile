OBJS   = main.o loaderbmp.o vec.o mat.o

SHDR=\

GLEW	= C:\glew-2.1.0

INCS	=	-m64 -std=c++14 -g -c -Wall -Werror -Wno-unused-parameter \
			-Ic:/msys64 \
			-I$(GLEW)/include \

LIBS	=	-m64 -std=c++14 -mwindows -mconsole -lopengl32  -lglu32 \
			-L$(GLEW)/lib\Release/x64 -lglew32 \

main.exe	:	obj $(OBJS:%.o=obj/%.o) $(SHDR)
	clang++ -o main.exe $(OBJS:%.o=obj/%.o) $(LIBS)


obj/%.o:%.cpp
	clang++  $(INCS) $< -o $@

%.spv:%.frag
	glslangValidator.exe $< -V -o $@

%.spv:%.vert
	glslangValidator.exe $< -V -o $@

obj:
	mkdir obj

clean:
	rm -f main.exe
	rm -rf obj
