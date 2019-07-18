default: main.o frontend.o processor.o processor_types.o processor_comp.o
	g++ main.o frontend.o processor.o processor_types.o processor_comp.o

processor_types.o: processor_types.hpp processor_types.cpp
	g++ -c processor_types.cpp

processor_comp.o: processor_comp.hpp
	g++ -c processor_comp.cpp

processor.o: processor_comp.hpp processor_types.hpp processor.hpp processor.cpp
	g++ -c processor.cpp

frontend.o: compiler.hpp compiler_comp.hpp
	g++ -c frontend.cpp

main.o: main.cpp
	g++ -c main.cpp

testExp:
	./a.out exp.w

test1:
	./a.out wain_1.w

clean:
	rm *.o
