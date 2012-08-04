# filename : generic.mak

all: generic.exe

generic.res : generic.rc generic.h
	rc generic.rc

generic.obj : generic.c generic.h
	cl -c -W3 -Gz -D_X86_ -DWIN32 generic.c

generic.exe : generic.obj generic.res
	link /MACHINE:I386 -subsystem:windows generic.res generic.obj kernel32.lib user32.lib gdi32.lib


