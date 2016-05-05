#!/usr/bin/make -f

USERLAND_ROOT=$(HOME)/projects/senior-design-prototyping/userland

CFLAGS_MAIN=-std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -Wreturn-type -Wno-multichar -MD
CFLAGS_ARCH=-march=native -mtune=native -mcpu=native -mfloat-abi=hard -mfpu=neon-vfpv4 -funsafe-math-optimizations
LDFLAGS_OPENCV=-lopencv_highgui -lopencv_core -lopencv_features2d -lopencv_legacy -lopencv_imgproc -lpthread -lm
LDFLAGS_PICAM=-L$(USERLAND_ROOT)/build/lib -lmmal_core -lmmal -lmmal_util -lvcos -lbcm_host
LDFLAGS_CVPICAM=-Lraspicam_cv -lraspicamcv

CFLAGS=$(CFLAGS_MAIN) $(CFLAGS_ARCH)
LDFLAGS+=$(LDFLAGS_CVPICAM) $(LDFLAGS_OPENCV) $(LDFLAGS_PICAM)
TARGETS=demo_detect controller

all: objs $(TARGETS)
objs: # directory for temporary objects
	mkdir objs

objs/%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@
demo_detect: objs/demo_detect.o
	g++ $(LDFLAGS) $+ -o $@
controller: objs/controller.o
	g++ $(LDFLAGS) $+ -o $@
clean:
	rm -rf objs $(TARGETS)

-include objs/*.d
