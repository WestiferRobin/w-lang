default: main.o frontend.o backend.o processor.o ll_init.o ll_expression.o ll_stmtlist.o general_util.o stdinit.o
	g++ -std=c++17 *.o -o wlang

general_util.o: general_util.hpp
	g++ -std=c++17 -c general_util.cpp

processor.o: Processor/processor.hpp general_types.hpp general_util.hpp
	g++ -std=c++17 -c Processor/processor.cpp

backend.o: Compiler/compiler_comp.hpp Compiler/token_types.hpp Compiler/ast_types.hpp general_types.hpp general_util.hpp
	g++ -std=c++17 -c Compiler/backend.cpp

frontend.o: Compiler/compiler_comp.hpp Compiler/token_types.hpp Compiler/ast_types.hpp
	g++ -std=c++17 -c Compiler/frontend.cpp

ll_init.o: Compiler/compiler_comp.hpp Compiler/token_types.hpp Compiler/ast_types.hpp
	g++ -std=c++17 -c Compiler/ll_init.cpp

ll_expression.o: Compiler/compiler_comp.hpp Compiler/token_types.hpp Compiler/ast_types.hpp
	g++ -std=c++17 -c Compiler/ll_expression.cpp

ll_stmtlist.o: Compiler/compiler_comp.hpp Compiler/token_types.hpp Compiler/ast_types.hpp
	g++ -std=c++17 -c Compiler/ll_stmtlist.cpp

stdinit.o: Compiler/compiler_comp.hpp Compiler/token_types.hpp Compiler/ast_types.hpp general_types.hpp general_util.hpp
	g++ -std=c++17 -c Compiler/stdinit.cpp

main.o: main.cpp
	clear
	g++ -std=c++17 -c main.cpp

test:
	clear
	./wlang test_space.w

testD:
	clear
	./wlang test_space.w -d

clean:
	clear
	rm *.o wlang

rebuild:
	clear
	rm *.o wlang
	make
	./wlang test_space.w -d
