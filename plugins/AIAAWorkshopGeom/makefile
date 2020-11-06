GEOPATH := ${GEOLYTICAL}
OUTPUT = ${CHAMPS_PLUGINS}/libAIAAWorkshopGeom.so
.PHONY: main
main:
	g++ -std=c++11 -fPIC -shared geom.cc -o ${OUTPUT} -I${GEOPATH}/include -L${GEOPATH}/lib -lgeolytical

clean:
	-rm ${OUTPUT}
