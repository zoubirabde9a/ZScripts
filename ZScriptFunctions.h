#pragma once
/*#include "ZScript\Scanner.h"
#include "ZScript\Parser.h"
#include "ZScript\Interpreter.h"*/

#include "Scanner.h"
#include "Parser.h"
#include "Interpreter.h"

namespace ZScript {

	bool parseBuffer(std::vector < StmtPtr > & out, const std::string& buffer);
	StmtPtr parseExpressionStmt(const std::string& buffer);
	void doString(Interpreter* interpreter, const std::string& buffer);
	void doString(Interpreter* interpreter, const std::string& buffer, Environment* env);
}
