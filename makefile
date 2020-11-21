ALLDIRS := $(wildcard plugins/* utils/*)
TARGET := 
.PHONY: all
all:
	@for tdir in ${ALLDIRS} ; do\
		${MAKE} -C $${tdir} -f makefile ${TARGET};\
	done
