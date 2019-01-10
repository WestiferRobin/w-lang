program: main.o parserUtility.o
	gcc -o main.o parserUtility.o

main.o: parser.c parserUtility.h
	gcc -c parser.c parserUtility.h

parserUtility.o: parserUtility.c parserUtility.h
	gcc -c parserUtility.c parserUtility.h

clean:
	rm *.o
