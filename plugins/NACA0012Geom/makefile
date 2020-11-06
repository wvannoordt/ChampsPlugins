GEOPATH := ${GEOLYTICAL}
OUTPUT = ${CHAMPS_PLUGINS}/libNACA0012Geom.so
.PHONY: main
main:
	g++ -std=c++11 -fPIC -shared geom.cc -o ${OUTPUT} -I. -I${GEOPATH}/include -L${GEOPATH}/lib -lgeolytical

clean:
	-rm ${OUTPUT}
