
#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "symbol_linking.h"
#include "interpret.h"

using namespace rose;

std::pair<std::vector<Plot>, std::vector<TintColor>> translate(const char *filename, int max_time, bool print) {
	std::vector<Plot> plots;
	std::vector<TintColor> colors;
	try {
		Lexer lexer(filename);
		Start ast = rose::Parser(&lexer).parse();
		SymbolLinking sl;
		ast.apply(sl);
		Interpreter in(sl, filename);
		AProgram program = ast.getPProgram().cast<AProgram>();
		if (program.getProcedure().size() == 0) {
			throw Exception("No procedures");
		}
		AProcedure mainproc = program.getProcedure().front().cast<AProcedure>();
		if (mainproc.getParams().size() != 0) {
			throw CompileException(mainproc.getName(), "Entry procedure must not have any parameters");
		}
		plots = in.interpret(mainproc, max_time);
		colors = in.get_colors(program);
	} catch (const CompileException& exc) {
		if (print) {
			printf("%s:%d:%d: Error: %s\n", filename, exc.getToken().getLine(), exc.getToken().getPos(), exc.getMessage().c_str());
			fflush(stdout);
		}
	} catch (const Exception& exc) {
		if (print) {
			printf("%s: Error: %s\n", filename, exc.getMessage().c_str());
			fflush(stdout);
		}
	}

	return std::make_pair(std::move(plots), std::move(colors));
}

