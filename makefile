CC = gcc
SRC_DIR = src
CCFLAGS = -g -Wall -Iinclude
EXEC = wordpairs
OBJS = crc64.o getWord.o hashtable.o wordPair.o

.PHONEY: clean run memcheck

${EXEC}: ${OBJS}
	${CC} ${CCFLAGS} -o ${EXEC} ${OBJS}

crc64.o: ${SRC_DIR}/crc64.c
	${CC} ${CCFLAGS} -c ${SRC_DIR}/crc64.c

getWord.o: ${SRC_DIR}/getWord.c
	${CC} ${CCFLAGS} -c ${SRC_DIR}/getWord.c

hashtable.o: ${SRC_DIR}/hashtable.c
	${CC} ${CCFLAGS} -c ${SRC_DIR}/hashtable.c

wordPair.o: ${SRC_DIR}/wordPair.c
	${CC} ${CCFLAGS} -c ${SRC_DIR}/wordPair.c

run: ${EXEC}
	./${EXEC} -25 ./dataset/*

memcheck: ${EXEC}
	valgrind --tool=memcheck --track-origins=yes --leak-check=full ./${EXEC} -10 ./dataset/*

clean:
	rm -f ${OBJS} ${EXEC}
