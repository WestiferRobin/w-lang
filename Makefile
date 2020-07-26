default: bin/main.o bin/frontend.o bin/backend.o bin/processor.o bin/ll_init.o bin/ll_expression.o bin/ll_stmtlist.o bin/general_util.o bin/stdinit.o
	g++ -std=c++17 *.o -o wlang

bin/general_util.o: src/general_util.hpp
	g++ -std=c++17 -c src/general_util.cpp

bin/processor.o: src/processor.hpp src/general_types.hpp src/general_util.hpp
	g++ -std=c++17 -c src/processor.cpp

bin/backend.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp src/general_types.hpp src/general_util.hpp
	g++ -std=c++17 -c src/backend.cpp

bin/frontend.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp
	g++ -std=c++17 -c src/frontend.cpp

bin/ll_init.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp
	g++ -std=c++17 -c src/ll_init.cpp

bin/ll_expression.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp
	g++ -std=c++17 -c src/ll_expression.cpp

bin/ll_stmtlist.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp
	g++ -std=c++17 -c src/ll_stmtlist.cpp

bin/stdinit.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp src/general_types.hpp src/general_util.hpp
	g++ -std=c++17 -c src/stdinit.cpp

bin/main.o: src/main.cpp
	clear
	g++ -std=c++17 -c src/main.cpp

testN:
	clear
	./wlang test/test_space.w

testD:
	clear
	./wlang test/test_space.w -d

clean:
	clear
	rm *.o wlang

rebuild:
	clear
	rm *.o wlang
	make
	./wlang test/test_space.w -d