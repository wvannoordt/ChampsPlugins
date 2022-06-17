OUTPUT = ${CHAMPS_PLUGINS}/libAAirfoilGeom.so
NAPATH = $(shell pwd)/../../utils/NamedArgs
.PHONY: main
main:
	g++ -std=c++11 -fPIC -shared a_airfoil.cc -o ${OUTPUT} -I${GEOLYTICAL}/include -I${NAPATH}/include -L${NAPATH}/lib -lNa -L${GEOLYTICAL}/lib -lgeolytical

clean:
	-rm ${OUTPUT}
