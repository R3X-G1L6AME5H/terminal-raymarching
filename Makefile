##
# Terminal Raymarching
# Copyright (c) 2022 Nino Čandrlić/Nemo Czanderlitch

INCLUDE = -I. -I./include/
LIBS    = -lm

trmarch: config.h include/mat3.h include/raymarch.h include/renderer.h include/sdfs.h include/vec3.h
	gcc src/main.c -o $@ $(INCLUDE) $(LIBS)

# end
