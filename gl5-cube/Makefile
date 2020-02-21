OBJS   = main.o sys.o vec.o loaderbmp.o shapeprimitive.o shader.o

SHDR=\

LIBS	=	-m64 -mwindows -mconsole -lopengl32  -lglu32 \
			-LC:\msys64\glew-2.0.0-win32\glew-2.0.0\lib\Release\x64 -lglew32 

INCS	=	-m64 -g -c -Wall -Werror \
			-Ic:/msys64 \

main.exe	:	obj $(OBJS:%.o=obj/%.o) $(SHDR)
	clang++ -m64 -o main.exe $(OBJS:%.o=obj/%.o) $(LIBS)


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
