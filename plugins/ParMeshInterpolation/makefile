NAME=ParMeshInterpolation
MPICOMP=$(shell which mpicxx)
NAPATH=$(shell pwd)/../../utils/NamedArgs
ifeq (${MPICOMP}, )
$(error CANNOT FIND MPI!)
endif

main:
	rm -rf ./obj
	mkdir -p obj
	${MPICOMP} -fPIC -std=c++11 -I${NAPATH}/include -c lmain.cc -o obj/lmain.o -L${NAPATH}/lib -lNa
	${MPICOMP} -fPIC -std=c++11 -I${NAPATH}/include -c InterpInfo.cpp -o obj/InterpInfo.o -L${NAPATH}/lib -lNa
	${MPICOMP} -fPIC -std=c++11 -shared obj/*.o -o ${CHAMPS_PLUGINS}/lib${NAME}.so -L${NAPATH}/lib -lNa

clean:
	rm -rf ./obj
