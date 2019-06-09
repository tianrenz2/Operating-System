EXEC=p3_exec
CC=g++
CFLAGS=-I.

%.o: %.cpp	
	$(CC) -c $< $(CFLAGS)

${EXEC}: main.o p3_threads.o types_p3.o utils.o
	g++ -o ${EXEC} main.o p3_threads.o types_p3.o utils.o -I. -lpthread


clean:
	rm -rf ./${EXEC}
	rm -rf ./*.o
