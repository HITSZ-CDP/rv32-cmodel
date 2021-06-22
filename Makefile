emu_src: stage/* include/* main.c
run-emu: emu
	./emu
emu: emu_src
	gcc main.c stage/*.c -o emu
clean:
	rm -f emu
