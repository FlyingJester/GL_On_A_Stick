import os
import sys
import glob

sourcedir = "src"

builddir  = "build"

cflags = ""
linkflags= ""

libs = []

if sys.platform == 'win32' or sys.platform == 'win64':
	cflags= ' /O2 /Zi '
	linkflags = ' MSVCRT.lib kernel32.lib /NODEFAULTLIB:LIBCMT.lib /ENTRY:Main /SUBSYSTEM:WINDOWS /DEBUG '
	libs = ["OpenGL32"]
else:
	libs = ["GL"]


files = glob.glob(os.path.join(sourcedir, "*.cpp"))+glob.glob(os.path.join(sourcedir, "*.c"))

glExtra = StaticLibrary("glExtra", os.path.join(sourcedir, os.path.join("glextra", "glExtra.c")), CCFLAS = cflags, CPPPATH="extra")

Program(os.path.join(builddir, "gloas-demo"), files, LIBS=["SDL2"]+libs+glExtra, CCFLAGS = cflags, LINKFLAGS = linkflags, CPPPATH="extra")