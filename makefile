ALLDIRS := $(wildcard utils/* plugins/*)
TARGET := 
.PHONY: all
all:
	@for tdir in ${ALLDIRS} ; do\
		${MAKE} -C $${tdir} -f makefile ${TARGET};\
	done
