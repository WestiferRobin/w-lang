go_chicken: main.o frontEndUtility.o
	gcc main.o frontEndUtility.o -o go_chicken

main.o: main.c
	gcc -c main.c 

frontEndUtility.o: frontEndUtility.c frontEndUtility.h
	gcc frontEndUtility.c frontEndUtility.h -c

clean:
	rm *.o go_chicken
