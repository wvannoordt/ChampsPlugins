OUTPUT = ${CHAMPS_PLUGINS}/libFlatPlateCurvilinear.so
NAPATH = $(shell pwd)/../../utils/NamedArgs
.PHONY: main
main:
	g++ -std=c++11 -fPIC -shared -I. -I${NAPATH}/include makemesh.cc -o ${OUTPUT} -L${NAPATH}/lib -lNa

clean:
	-rm ${OUTPUT}