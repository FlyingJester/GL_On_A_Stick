import os
import sys
import glob

sourcedir = "src"

builddir  = "build"

cflags = ""
linkflags= ""
cpppath = ""
cppflags = ""

libs = []

env = Environment(ENV = os.environ)

if sys.platform == 'win32' or sys.platform == 'win64':
	cflags= ' /O2 /Zi '
	linkflags = ' MSVCRT.lib kernel32.lib /NODEFAULTLIB:LIBCMT.lib /ENTRY:Main /SUBSYSTEM:WINDOWS /DEBUG '
	libs = ["OpenGL32"]
	cpppath = "extra"
else:
#	libs = ["GL", "X11"]
	cflags = ' -std=c11 -O2 -pg '
	linkflags = ' -eMain -g '
	cppflags = " -DGL_GLEXT_PROTOTYPES "

if True or ARGUMENTS.get('emscripten', 'n') == 'y':
	env.Replace(CC="emcc")
	env.Append(CFLAGS=" -I/Users/mozilla/ts2/GL_On_A_Stick/SDL-emscripten/include ")
        env.Append(LINKFLAGS=" -L/Users/mozilla/ts2/GL_On_A_Stick/SDL-emscripten/ ")
        env.Append(CFLAGS=" -L/Users/mozilla/ts2/GL_On_A_Stick/SDL-emscripten/ ")

files = glob.glob(os.path.join(sourcedir, "*.cpp"))+glob.glob(os.path.join(sourcedir, "*.c"))

if sys.platform == 'win32' or sys.platform == 'win64':
    glExtra = StaticLibrary("glExtra", os.path.join(sourcedir, os.path.join("glextra", "glExtra.c")), CCFLAS = cflags, CPPPATH=cpppath)
    libs += glExtra

env.Program(os.path.join(builddir, "gloas-demo"), files, LIBS=["SDL2"]+libs, CCFLAGS = cflags, LINKFLAGS = linkflags, CPPPATH=cpppath, CPPFLAGS=cppflags)
