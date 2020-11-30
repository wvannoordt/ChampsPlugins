CUR_DIR := $(shell pwd)
OBJD := ${CUR_DIR}/obj
LIBD := ${CUR_DIR}/lib
IDIR := ${CUR_DIR}/include

main: setup
	ln -sf ${CUR_DIR}/NamedArgs.h ${IDIR}/NamedArgs.h
	g++ -fPIC -std=c++11 -c ${CUR_DIR}/NamedArgs.cpp -o ${OBJD}/NamedArgs.o
	g++ -fPIC -shared ${OBJD}/*.o -o ${LIBD}/libNa.a

setup:
	mkdir -p ${OBJD}
	mkdir -p ${LIBD}
	mkdir -p ${IDIR}

clean:
	rm -rf ${OBJD}
	rm -rf ${LIBD}
	rm -rf ${IDIR}	
