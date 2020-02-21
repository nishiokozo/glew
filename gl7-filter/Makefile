OBJS   = main.o loaderbmp.o vec.o mat.o

SHDR=\

INCS	=	-m64 -std=c++14 -g -c -Wall -Werror -Wno-unused-parameter \
			-Ic:/msys64 \

LIBS	=	-m64 -std=c++14 -mwindows -mconsole -lopengl32  -lglu32 \
			-LC:\msys64\glew-2.0.0-win32\glew-2.0.0\lib\Release\x64 -lglew32

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
