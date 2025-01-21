OBJS	= main.o construction.o vizi.o util.o
SOURCE	= main.cpp construction.cpp vizi.cpp util.cpp
HEADER	= servidor_job.h construction.h vizi.h util.h
OUT	= projeto
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

construction.o: construction.cpp
	$(CC) $(FLAGS) construction.cpp 

vizi.o: vizi.cpp
	$(CC) $(FLAGS) vizi.cpp 

util.o: util.cpp
	$(CC) $(FLAGS) util.cpp 


clean:
	rm -f $(OBJS) $(OUT)