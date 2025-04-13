DATA_FILE_NAME := "data.txt"

all: compile run clean

compile:
	gcc -c main.c -o main.o
	gcc main.o -o out

run:
	./out ${DATA_FILE_NAME}

clean:
	rm main.o out
