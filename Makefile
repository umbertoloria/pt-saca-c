CXX = gcc
CXXFLAGS =
SRCS = main.c
TARGET = pt-saca-c

.PHONY: run clean

${TARGET}:
	${CXX} ${SRCS} ${CXXFLAGS} -o ${TARGET}

run: ${TARGET}
	./${TARGET}

clean: ${TARGET}
	rm -f ${TARGET}

