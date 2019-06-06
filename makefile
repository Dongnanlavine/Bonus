CC=gcc

SRCDIR= ./src/

all: springfitexe

clean:
	 del .\spring_fit.exe    
springfitexe: springobj
	 $(CC) -o spring_fit.exe $(SRCDIR)spring.o -lgsl -lgslcblas
	 del  .\src\spring.o
springobj: 
	 $(CC) -c -o $(SRCDIR)spring.o $(SRCDIR)spring.c -lgsl -lgslcblas
   


     
