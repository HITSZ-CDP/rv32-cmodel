emu_src: stage/* include/* main.c
emu: emu_src
	gcc main.c stage/*.c -o emu
