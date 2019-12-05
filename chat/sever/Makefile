#Makefile
TARGET = chat_sever
CC = gcc
OPTION =  
SRC_FILES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRC_FILES))

%.o : %.c
	${CC} ${OPTION} -c $<

${TARGET}: ${OBJS}
	${CC} ${OPTION} -o $@ $^ 
	
.PHONY: clean
clean:
	rm -rf ${OBJS} ${TARGET}