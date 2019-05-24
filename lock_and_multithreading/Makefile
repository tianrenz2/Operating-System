EXEC=p2_exec
CC=g++
CFLAGS= -std=c++98 -I.

#----------------------------------------------
# complete this part
%.o: %.cpp	
	@$(CC) -c $< $(CFLAGS)
#----------------------------------------------

#----------------------------------------------
# complete this part
${EXEC}: main.o types_p2.o p2_threads.o utils.o
	@$(CC) -o ${EXEC} main.o types_p2.o p2_threads.o utils.o -I. -lpthread 
#----------------------------------------------


clean:
	rm -rf ./${EXEC}
	rm -rf ./*.o
