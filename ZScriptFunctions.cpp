#include "ZScriptFunctions.h"



namespace ZScript {
	bool parseBuffer(std::vector < StmtPtr > & out, const std::string& buffer) {
		out.clear();
		Scanner scanner(buffer);
		const std::vector < Token > &tokens = scanner.scanTokens();

		if (scanner.hasError()) {
			printf("ZScript scanner error\n");
			return false;
		}
		Parser parser(tokens);
		parser.parse(out);
		for (auto it : tokens) {
			it.free();
		}
		if (!parser.hasError()) {
			return true;
		}
		return false;
	}
	StmtPtr parseExpressionStmt(const std::string& buffer) {
		Scanner scanner(buffer);
		const std::vector < Token > &tokens = scanner.scanTokens();

		if (!scanner.hasError()) {
			Parser parser(tokens);
			StmtPtr out = parser.parseExpressionStatement();
			for (auto it : tokens) {
				it.free();
			}
			if (parser.hasError()) return nullptr;
			return out;
		}
		return nullptr;
	}
	void doString(Interpreter* interpreter, const std::string& buffer) {
		std::vector < StmtPtr > statements;
		if (parseBuffer(statements, buffer)) {
			interpreter->interpret(statements);
		}
	}
	void doString(Interpreter* interpreter, const std::string& buffer, Environment* env) {
		std::vector < StmtPtr > statements;
		if (parseBuffer(statements, buffer)) {
			interpreter->interpret(statements, env);
		}
	}
}