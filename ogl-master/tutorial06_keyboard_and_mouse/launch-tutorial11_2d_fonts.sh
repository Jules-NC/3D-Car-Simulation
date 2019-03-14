#!/bin/sh
bindir=$(pwd)
cd /home/lambyor/Documents/OpenGL/3D-Car-Simulation/ogl-master/tutorial11_2d_fonts/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/lambyor/Documents/OpenGL/3D-Car-Simulation/ogl-master/tutorial06_keyboard_and_mouse/tutorial11_2d_fonts 
	else
		"/home/lambyor/Documents/OpenGL/3D-Car-Simulation/ogl-master/tutorial06_keyboard_and_mouse/tutorial11_2d_fonts"  
	fi
else
	"/home/lambyor/Documents/OpenGL/3D-Car-Simulation/ogl-master/tutorial06_keyboard_and_mouse/tutorial11_2d_fonts"  
fi
