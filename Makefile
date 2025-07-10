CXX = gcc
CXXFLAGS =
SRCS = main.c
TARGET = ptsaca

.PHONY: run clean build bar

${TARGET}:
	${CXX} ${SRCS} ${CXXFLAGS} -o ${TARGET}

run: ${TARGET}
	./${TARGET}

clean: ${TARGET}
	rm -f ${TARGET}

build: ${SRCS} ${TARGET}
	#

# Build and Run
bar: ${SRCS}
	make clean
	make run

