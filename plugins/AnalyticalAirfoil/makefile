OUTPUT = ${CHAMPS_PLUGINS}/libAnalyticalAirfoil.so
NAPATH = $(shell pwd)/../../utils/NamedArgs
.PHONY: main
main:
	g++ -std=c++11 -fPIC -shared -I. -I${GEOLYTICAL}/include -I${NAPATH}/include main.cc -o ${OUTPUT} -L${NAPATH}/lib -lNa -L${GEOLYTICAL}/lib -lgeolytical

clean:
	-rm ${OUTPUT}
