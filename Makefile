CXX=c++
LLVMCONFIG=llvm-config-12
CXXFLAGS=-Wall `$(LLVMCONFIG) --cxxflags` -g
LDFLAGS=`$(LLVMCONFIG) --ldflags --system-libs --libs all`

llama: lexer.o parser.o ast.o
	$(CXX) $(CXXFLAGS) -o llama $^ $(LDFLAGS)

ast.o: ast.hpp symbol.hpp sem.hpp compile.hpp print.hpp

parser.hpp parser.cpp: parser.y lexer.hpp ast.hpp symbol.hpp
	bison -dv -o parser.cpp parser.y

lexer.cpp: lexer.l lexer.hpp parser.hpp ast.hpp symbol.hpp
	flex -s -o lexer.cpp lexer.l

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o

distclean: clean
	$(RM) llama