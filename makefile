CC=gcc

SRCDIR= ./src/

all: springfitexe

clean:
	 del .\spring_fit.exe    
springfitexe: springobj
	 $(CC) -o spring_fit  $(SRCDIR)spring.o -lgsl -lgslcblas
	 rm -f ./src/spring.o
springobj: 
	 $(CC) -c -o $(SRCDIR)spring.o $(SRCDIR)spring.c 
   


     
