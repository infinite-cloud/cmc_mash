IDIR = inc
CC = g++
CFLAGS = -I$(IDIR) -Wall -std=c++17 -pthread -O3 -fopenmp

ODIR = obj
LDIR = lib

_DEPS = image.h ray.h scene.h
_DEPS += renderer.h object.h light.h
_DEPS += scene_loader.h model.h bvh.h
_DEPS += glm/*.hpp stb/*.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = main.o renderer.o
_OBJ += image.o object.o scene.o
_OBJ += scene_loader.o model.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: $(LDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(shell mkdir -p $(ODIR))
