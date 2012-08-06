# filename : generic.mak

all: generic.exe

generic.res : generic.rc generic.h
	rc generic.rc

generic.obj : generic.c generic.h DBWindow.h DBWindow.cpp
	cl -c -W3 -Gz -D_X86_ -DWIN32 generic.c

DBWindow.obj : DBWindow.cpp DBWindow.h
	cl -c -W3 -Gz -D_X86_ -DWIN32 DBWindow.cpp

generic.exe : generic.obj DBWindow.obj generic.res
	link /MACHINE:I386 -subsystem:windows generic.res generic.obj DBWindow.obj kernel32.lib user32.lib gdi32.lib


