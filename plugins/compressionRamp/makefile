GEOPATH := ${GEOLYTICAL}
NAME := compressionRamp
OUTPUT = ${CHAMPS_PLUGINS}/lib${NAME}.so
NAPATH := $(shell pwd)/../../utils/NamedArgs
.PHONY: main
main:
	g++ -std=c++11 -fPIC -shared geom.cc -o ${OUTPUT} -I${NAPATH}/include -I${GEOPATH}/include -L${NAPATH}/lib -lNa -L${GEOPATH}/lib -lgeolytical

clean:
	-rm ${OUTPUT}
