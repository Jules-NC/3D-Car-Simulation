#!/bin/sh
bindir=$(pwd)
cd /home/lambyor/Documents/OpenGL/3D-Car-Simulation/ogl-master/tutorial18_billboards_and_particles/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/lambyor/Documents/OpenGL/3D-Car-Simulation/ogl-master/tutorial06_keyboard_and_mouse/tutorial18_particles 
	else
		"/home/lambyor/Documents/OpenGL/3D-Car-Simulation/ogl-master/tutorial06_keyboard_and_mouse/tutorial18_particles"  
	fi
else
	"/home/lambyor/Documents/OpenGL/3D-Car-Simulation/ogl-master/tutorial06_keyboard_and_mouse/tutorial18_particles"  
fi
