OUTPUT = ${CHAMPS_PLUGINS}/libFlatPlateCurvilinear.so
.PHONY: main
main:
	g++ -std=c++11 -fPIC -shared makemesh.cc -o ${OUTPUT} -I.

clean:
	-rm ${OUTPUT}

