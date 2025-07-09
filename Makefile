CXX = gcc
CXXFLAGS =
SRCS = main.c
TARGET = ptsaca

.PHONY: run clean build

${TARGET}:
	${CXX} ${SRCS} ${CXXFLAGS} -o ${TARGET}

run: ${TARGET}
	./${TARGET}

clean: ${TARGET}
	rm -f ${TARGET}

# Build and Run
bar: ${SRCS}
	make clean
	make run

