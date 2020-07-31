default: bin/main.o bin/frontend.o bin/backend.o bin/processor.o bin/ll_init.o bin/ll_expression.o bin/ll_stmtlist.o bin/general_util.o bin/stdinit.o
	g++ -std=c++17 bin/*.o -o wlang

bin/general_util.o: src/general_util.hpp
	g++ -std=c++17 -c src/general_util.cpp && mv general_util.o bin/general_util.o

bin/processor.o: src/processor.hpp src/general_types.hpp src/general_util.hpp
	g++ -std=c++17 -c src/processor.cpp && mv processor.o bin/processor.o

bin/backend.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp src/general_types.hpp src/general_util.hpp
	g++ -std=c++17 -c src/backend.cpp && mv backend.o bin/backend.o

bin/frontend.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp
	g++ -std=c++17 -c src/frontend.cpp && mv frontend.o bin/frontend.o

bin/ll_init.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp
	g++ -std=c++17 -c src/ll_init.cpp && mv ll_init.o bin/ll_init.o

bin/ll_expression.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp
	g++ -std=c++17 -c src/ll_expression.cpp && mv ll_expression.o bin/ll_expression.o

bin/ll_stmtlist.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp
	g++ -std=c++17 -c src/ll_stmtlist.cpp && mv ll_stmtlist.o bin/ll_stmtlist.o

bin/stdinit.o: src/compiler_comp.hpp src/token_types.hpp src/ast_types.hpp src/general_types.hpp src/general_util.hpp
	g++ -std=c++17 -c src/stdinit.cpp && mv stdinit.o bin/stdinit.o

bin/main.o: src/main.cpp
	clear
	g++ -std=c++17 -c src/main.cpp && mv main.o bin/main.o

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
	rm bin/*.o wlang

rebuild:
	clear
	rm bin/*.o wlang
	make
	./wlang test/test_space.w -d