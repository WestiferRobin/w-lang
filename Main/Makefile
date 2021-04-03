default: bin/main.o bin/ast_utils.o bin/stdinit.o bin/processor.o bin/loop.o bin/cond.o bin/scope.o bin/assign.o bin/system_op.o bin/expression.o bin/frontend.o bin/error_reader.o bin/backend.o bin/start.o
	g++ -std=c++17 bin/*.o -o wlang

bin/ast_utils.o: src/general_util.hpp
	g++ -std=c++17 -c src/ast_utils.cpp && mv ast_utils.o bin/ast_utils.o

bin/backend.o: src/main_comp.hpp
	g++ -std=c++17 -c src/backend.cpp && mv backend.o bin/backend.o

bin/error_reader.o: src/general_util.hpp
	g++ -std=c++17 -c src/error_reader.cpp && mv error_reader.o bin/error_reader.o

bin/frontend.o: src/main_comp.hpp
	g++ -std=c++17 -c src/frontend.cpp && mv frontend.o bin/frontend.o

bin/expression.o: src/parser_comp.hpp
	g++ -std=c++17 -c src/expression.cpp && mv expression.o bin/expression.o

bin/processor.o: src/main_comp.hpp
	g++ -std=c++17 -c src/processor.cpp && mv processor.o bin/processor.o

bin/stdinit.o: src/general_util.hpp
	g++ -std=c++17 -c src/stdinit.cpp && mv stdinit.o bin/stdinit.o

bin/loop.o: src/parser_comp.hpp
	g++ -std=c++17 -c src/loop.cpp && mv loop.o bin/loop.o

bin/cond.o: src/parser_comp.hpp
	g++ -std=c++17 -c src/cond.cpp && mv cond.o bin/cond.o

bin/assign.o: src/parser_comp.hpp
	g++ -std=c++17 -c src/assign.cpp && mv assign.o bin/assign.o

bin/system_op.o: src/parser_comp.hpp
	g++ -std=c++17 -c src/system_op.cpp && mv system_op.o bin/system_op.o

bin/scope.o: src/parser_comp.hpp
	g++ -std=c++17 -c src/scope.cpp && mv scope.o bin/scope.o

bin/start.o: src/parser_comp.hpp
	g++ -std=c++17 -c src/start.cpp && mv start.o bin/start.o

bin/main.o: src/main.cpp
	clear
	g++ -std=c++17 -c src/main.cpp  && mv main.o bin/main.o

test_runner: test_runner.o
	g++ -std=c++17 test_runner.o -o test_runner
	./test_runner

test_runner.o:
	g++ -std=c++17 -c src/test_runner.cpp 

testN:
	clear
	./wlang test/test_space.w

testD:
	clear
	./wlang test/test_space.w -d

stress:
	clear
	./wlang test/stress_test.w

clean:
	clear
	rm bin/*.o wlang test_runner test_runner.o

rebuild:
	clear
	rm bin/*.o wlang
	make
	./wlang test/test_space.w -d