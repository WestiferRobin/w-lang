go_chicken: main.o frontEndUtility.o
	gcc main.o frontEndUtility.o -o go_chicken

frontEndUtility.o: frontEndUtility.c frontEndUtility.h lexingUtility.h parserUtility.h
	gcc -c frontEndUtility.c -o frontEndUtility.o

main.o: main.c
	gcc -c main.c frontEndUtility.h -o main.o

clean:
	rm *.o
