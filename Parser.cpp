#include "Parser.h"


namespace ZScript {
	Parser::Parser(const std::vector < Token >& tokens)
	: m_tokens(tokens){

	}
	void Parser::parse(std::vector < StmtPtr > &statements) {
		statements.clear();
		m_current = 0;
		size_t size = 0;
		while (!isAtEnd()) {
			declarationCount();
			size++;
		}
		if (!m_hasError) {
			statements.reserve(size);
			m_current = 0;
			while (!isAtEnd()) {
				statements.push_back(declaration());
			}
		}
	}
	StmtPtr Parser::parseExpressionStatement() {
		if (!isAtEnd()) {
			Expr* expr = expression();
			return std::make_shared < Expression >(expr);
		}
		else return nullptr;
	}

	void Parser::declarationCount() {
		while (match(NEW_LINE));
		try {
			if (match(FUN)) return functionCount();
			if (match(VAR)) return varDeclarationCount();

			return statementCount();
		}
		catch (const ParseError& error) {
			synchronize();
			return ;
		}
	}
	void Parser::functionCount() {
		Token name = consume(IDENTIFIER, "Expect function name.");
		consume(LEFT_PAREN, "Expect '(' after function name.");

		std::list < Token > parameters;
		if (!check(RIGHT_PAREN)) {
			do {
				if (parameters.size() >= 8) {
					error(peek(), "Cannot have more than 8 parameters.");
				}
				parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
			} while (match(COMMA));
		}
		consume(RIGHT_PAREN, "Expect ')' after parameters.");
		consume(LEFT_BRACE, "Expect '{' before function body.");
		blockCount();
		return ;
	}
	void Parser::varDeclarationCount() {
		Token name = consume(IDENTIFIER, "Expect variable name.");

		if (match(EQUAL)) {
			expressionCount();
		}
		if (!match(NEW_LINE) && !(peek().getType() == ENOF)) {
			consume(SEMICOLON, "Expect ';' after variable declaration.");
		}
		return ;
	}
	void Parser::statementCount() {
		if (match(FOR)) return forStatementCount();
		if (match(WHILE)) return whileStatementCount();
		if (match(IF)) return ifStatementCount();
		if (match(PRINT)) return printStatementCount();
		if (match(RETURN)) return returnStatementCount();
		if (match(LEFT_BRACE)) {
			blockCount();
			return ;
		}
		return expressionStatementCount();
	}
	void Parser::forStatementCount() {
		consume(LEFT_PAREN, "Expect '(' after 'for'.");

		if (match(VAR)) {
			varDeclarationCount();
		}
		else {
			expressionStatementCount();
		}

		if (!check(SEMICOLON)) {
			 expressionCount();
		}
		consume(SEMICOLON, "Expect ';' after loop condition.");

		if (!check(RIGHT_PAREN)) {
			expressionCount();
		}
		consume(RIGHT_PAREN, "Expect ')' after for clauses.");
		statementCount();
	}
	void Parser::whileStatementCount() {
		consume(LEFT_PAREN, "Expect '(' after 'while'.");
		expressionCount();
		consume(RIGHT_PAREN, "Expect ')' after condition.");
		statementCount();
	}
	void Parser::returnStatementCount() {
		Token keyword = previous();
		if (!check(SEMICOLON)) {
			expressionCount();
		}
		if (!match(NEW_LINE) && !(peek().getType() == ENOF)) {
			consume(SEMICOLON, "Expect ';' after return value.");
		}
	}
	void Parser::ifStatementCount() {
		consume(LEFT_PAREN, "Expect '(' after 'if'.");
		expressionCount();
		consume(RIGHT_PAREN, "Expect ')' after if condition.");

		statementCount();
		if (match(ELSE)) {
			statementCount();
		}
	}
	void Parser::blockCount() {

		while (!check(RIGHT_BRACE) && !isAtEnd()) {
			declarationCount();
		}
		consume(RIGHT_BRACE, "Expect '}' after block.");
	}
	void Parser::printStatementCount() {
		expressionCount();
		if (!match(NEW_LINE) && !(peek().getType() == ENOF)) {
			consume(SEMICOLON, "Expect ';' after value.");
		}
	}
	void Parser::expressionStatementCount() {
		expressionCount();
		if (!match(NEW_LINE) && !(peek().getType() == ENOF)) {
			consume(SEMICOLON, "Expect ';' after expression.");
		}
	}
	void Parser::expressionCount() {
		return assignmentCount();
	}
	void Parser::assignmentCount() {
		orCount();
		if (match(EQUAL)) {
			previous();
			assignmentCount();
		}
	}
	void Parser::orCount() {
		andCount();
		while (match(OR)) {
			previous();
			andCount();
		}
	}
	void Parser::andCount() {
		equalityCount();

		while (match(AND)) {
			previous();
			equalityCount();
		}
	}
	void Parser::equalityCount() {
		comparisonCount();

		while (match({ BANG_EQUAL, EQUAL_EQUAL })) {
			previous();
			comparisonCount();
		}
	}
	void Parser::comparisonCount() {
		additionCount();

		while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL })) {
			previous();
			additionCount();
		}

	}
	void Parser::additionCount() {
		multiplicationCount();

		while (match({ MINUS, PLUS })) {
			previous();
			multiplicationCount();
		}
	}
	void Parser::multiplicationCount() {
		unaryCount();

		while (match({ SLASH, STAR })) {
			previous();
			unaryCount();
		}
	}
	void Parser::unaryCount() {
		if (match({ BANG, MINUS })) {
			previous();
			unaryCount();
			return;
		}

		return callCount();
	}
	void Parser::callCount() {
		primaryCount();

		while (true) {
			if (match(LEFT_PAREN)) {
				finishCallCount();
			}
			else if (match(DOT)) {
				consume(IDENTIFIER,
					"Expect property name after '.'.");
			}
			else {
				break;
			}
		}
	}
	void Parser::finishCallCount() {
		size_t arguments = 0;
		if (!check(RIGHT_PAREN)) {
			do {
				if (arguments >= 8) {
					error(peek(), "Cannot have more than 8 arguments.");
				}
				expressionCount();
				arguments++;
			} while (match(COMMA));
		}

		consume(RIGHT_PAREN, "Expect ')' after arguments.");
	}
	void Parser::functionExpressionCount() {
		consume(LEFT_PAREN, "Expect '(' after fun keyword");

		size_t parameters = 0;
		if (!check(RIGHT_PAREN)) {
			do {
				if (parameters >= 8) {
					error(peek(), "Cannot have more than 8 parameters.");
				}
				consume(IDENTIFIER, "Expect parameter name.");
				parameters++;
			} while (match(COMMA));
		}
		consume(RIGHT_PAREN, "Expect ')' after parameters.");
		consume(LEFT_BRACE, "Expect '{' before function body.");
		blockCount();
	}
	void Parser::primaryCount() {
		if (match(FALSE)) return ;
		if (match(TRUE)) return ;
		if (match(NIL)) return ;
		if (match({ NUMBER, STRING })) {
			return;
		}
		if (match(IDENTIFIER)) {
			return;
		}
		if (match(LEFT_PAREN)) {
			expressionCount();

			consume(RIGHT_PAREN, "Expect ')' after expression.");
			return;
		}
		if (match(FUN)) return functionExpressionCount();
		throw error(peek(), "Expect expression.");
	}
	StmtPtr Parser::declaration() {
		while (match(NEW_LINE));
		try {
			if (match(FUN)) return function("function");
			if (match(VAR)) return varDeclaration();

			return statement();
		}
		catch (const ParseError& error) {
			synchronize();
			return nullptr;
		}
	}
	StmtPtr Parser::function(const std::string& kind) {
		Token name = consume(IDENTIFIER, "Expect " + kind + " name.");
		consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");

		std::list < Token > parameters;
		if (!check(RIGHT_PAREN)) {
			do {
				if (parameters.size() >= 8) {
					error(peek(), "Cannot have more than 8 parameters.");
				}
				parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
			} while (match(COMMA));
		}
		consume(RIGHT_PAREN, "Expect ')' after parameters.");
		consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
		std::list < StmtPtr > body;
		block(body);
		return std::make_shared < FunctionStmt > (name, parameters, body);
	}
	StmtPtr Parser::varDeclaration() {
		Token name = consume(IDENTIFIER, "Expect variable name.");

		Expr* initializer = nullptr;
		if (match(EQUAL)) {
			initializer = expression();
		}
		if (!match(NEW_LINE) && !(peek().getType() == ENOF)) {
			consume(SEMICOLON, "Expect ';' after variable declaration.");
		}
		return std::make_shared < Var >(name, initializer);
	}
	StmtPtr Parser::statement() {
		if (match(FOR)) return forStatement();
		if (match(WHILE)) return whileStatement();
		if (match(IF)) return ifStatement();
		if (match(PRINT)) return printStatement();
		if (match(RETURN)) return returnStatement();
		if (match(LEFT_BRACE)) {
			std::list < StmtPtr > statements;
			block(statements);
			return std::make_shared < Block >(statements);
		}
		return expressionStatement();
	}
	StmtPtr Parser::forStatement() {
		consume(LEFT_PAREN, "Expect '(' after 'for'.");

		StmtPtr initializer = nullptr;
		if (match(SEMICOLON)) {
			initializer = nullptr;
		}
		else if (match(VAR)) {
			initializer = varDeclaration();
		}
		else {
			initializer = expressionStatement();
		}

		Expr* condition = nullptr;
		if (!check(SEMICOLON)) {
			condition = expression();
		}
		consume(SEMICOLON, "Expect ';' after loop condition.");

		Expr* increment = nullptr;
		if (!check(RIGHT_PAREN)) {
			increment = expression();
		}
		consume(RIGHT_PAREN, "Expect ')' after for clauses.");
		StmtPtr body = statement();

		if (increment != nullptr) {
			body = std::make_shared < Block > (std::list < StmtPtr >{ body, std::make_shared < Expression >(increment) });
		}

		if (condition == nullptr) condition = new LiteralExpr(Literal(true));
		body = std::make_shared < While > (condition, body);

		if (initializer != nullptr) {
			body = std::make_shared < Block > (std::list < StmtPtr >{ initializer, body });
		}

		return body;
	}
	StmtPtr Parser::whileStatement() {
		consume(LEFT_PAREN, "Expect '(' after 'while'.");
		Expr* condition = expression();
		consume(RIGHT_PAREN, "Expect ')' after condition.");
		StmtPtr body = statement();

		return std::make_shared < While >(condition, body);
	}
	StmtPtr Parser::returnStatement() {
		Token keyword = previous();
		Expr* value = nullptr;
		if (!check(SEMICOLON)) {
			value = expression();
		}
		if (!match(NEW_LINE) && !(peek().getType() == ENOF)) {
			consume(SEMICOLON, "Expect ';' after return value.");
		}
		return std::make_shared < Return >(keyword, value);
	}
	StmtPtr Parser::ifStatement() {
		consume(LEFT_PAREN, "Expect '(' after 'if'.");
		Expr* condition = expression();
		consume(RIGHT_PAREN, "Expect ')' after if condition.");

		StmtPtr thenBranch = statement();
		StmtPtr elseBranch = nullptr;
		if (match(ELSE)) {
			elseBranch = statement();
		}

		return std::make_shared < If >(condition, thenBranch, elseBranch);
	}
	void Parser::block(std::list < StmtPtr >& statements) {

		while (!check(RIGHT_BRACE) && !isAtEnd()) {
			statements.push_back(declaration());
		}

		consume(RIGHT_BRACE, "Expect '}' after block.");
	}
	StmtPtr Parser::printStatement() {
		Expr* value = expression();
		if (!match(NEW_LINE) && !(peek().getType() == ENOF)) {
			consume(SEMICOLON, "Expect ';' after value.");
		}
		return std::make_shared < Print >(value);
	}
	StmtPtr Parser::expressionStatement() {
		Expr* expr = expression();
		if (!match(NEW_LINE) && !(peek().getType() == ENOF)) {
			consume(SEMICOLON, "Expect ';' after expression.");
		}
		return std::make_shared < Expression >(expr);
	}
	Expr* Parser::expression() {
		return assignment();
	}
	Expr* Parser::assignment() {
		Expr* expr = or ();
		if (match(EQUAL)) {
			Token equals = previous();
			Expr* value = assignment();

			if (dynamic_cast < Variable* >(expr)) {
				Token name = ((Variable*)(expr))->getName();
				return new Assign(name, value);
			}

			error(equals, "Invalid assignment target.");
		}
		return expr;
	}
	Expr* Parser::or() {
		Expr* expr = and();
		while (match(OR)) {
			Token op = previous();
			Expr* right = and();

			expr = new Logical(expr, op, right);
		}

		return expr;
	}
	Expr* Parser:: and() {
		Expr* expr = equality();

		while (match(AND)) {
			Token op = previous();
			Expr* right = equality();
			expr = new Logical(expr, op, right);
		}

		return expr;
	}
	Expr* Parser::equality() {
		Expr* expr = comparison();

		while (match({ BANG_EQUAL, EQUAL_EQUAL })) {
			Token op = previous();
			Expr* right = comparison();
			expr = new Binary(expr, op, right);
		}

		return expr;
	}
	Expr* Parser::comparison() {
		Expr* expr = addition();

		while (match({ GREATER, GREATER_EQUAL, LESS, LESS_EQUAL })) {
			Token op = previous();
			Expr* right = addition();
			expr = new Binary(expr, op, right);
		}

		return expr;
	}

	Expr* Parser::addition() {
		Expr* expr = multiplication();

		while (match({ MINUS, PLUS })) {
			Token op = previous();
			Expr* right = multiplication();
			expr = new Binary(expr, op, right);
		}

		return expr;
	}

	Expr* Parser::multiplication() {
		Expr* expr = unary();

		while (match({ SLASH, STAR })) {
			Token op = previous();
			Expr* right = unary();
			expr = new Binary(expr, op, right);
		}

		return expr;
	}
	Expr* Parser::unary() {
		if (match({ BANG, MINUS })) {
			Token op = previous();
			Expr* right = unary();
			return new Unary(op, right);
		}

		return call();
	}
	Expr* Parser::call() {
		Expr* expr = primary();

		while (true) {
			if (match(LEFT_PAREN)) {
				expr = finishCall(expr);
			}
			else if (match(DOT)) {
				Token name = consume(IDENTIFIER,
					"Expect property name after '.'.");
				expr = new Get(expr, name);
			}
			else {
				break;
			}
		}

		return expr;
	}
	Expr* Parser::finishCall(Expr* callee) {
		std::list < Expr* > arguments;
		if (!check(RIGHT_PAREN)) {
			do {
				if (arguments.size() >= 8) {
					error(peek(), "Cannot have more than 8 arguments.");
				}
				arguments.push_back(expression());
			} while (match(COMMA));
		}

		Token paren = consume(RIGHT_PAREN, "Expect ')' after arguments.");

		return new Call(callee, paren, arguments);
	}
	Expr* Parser::functionExpression() {
		consume(LEFT_PAREN, "Expect '(' after fun keyword");

		std::list < Token > parameters;
		if (!check(RIGHT_PAREN)) {
			do {
				if (parameters.size() >= 8) {
					error(peek(), "Cannot have more than 8 parameters.");
				}
				parameters.push_back(consume(IDENTIFIER, "Expect parameter name."));
			} while (match(COMMA));
		}
		consume(RIGHT_PAREN, "Expect ')' after parameters.");
		consume(LEFT_BRACE, "Expect '{' before function body.");
		std::list < StmtPtr > body;
		block(body);
		return new FunctionExpr(parameters, body);
	}
	Expr* Parser::primary() {
		if (match(FALSE)) return new LiteralExpr(Literal(false));
		if (match(TRUE)) return new LiteralExpr(Literal(true));
		if (match(NIL)) return new LiteralExpr(Literal());
		if (match({ NUMBER, STRING })) {
			return new LiteralExpr(previous().getLiteral().copy());
		}
		if (match(IDENTIFIER)) {
			return new Variable(previous());
		}
		if (match(LEFT_PAREN)) {
			Expr* expr = expression();

			consume(RIGHT_PAREN, "Expect ')' after expression.");
			return new Grouping(expr);
		}
		if (match(FUN)) return functionExpression();
		error(peek(), "Expect expression.");
	}
	bool Parser::match(const std::vector < TokenType >& types) {
		for (TokenType type : types) {
			if (check(type)) {
				advance();
				return true;
			}
		}

		return false;
	}
	bool Parser::match(TokenType type) {
		if (check(type)) {
			advance();
			return true;
		}
		return false;
	}
	bool Parser::check(TokenType tokenType) {
		if (isAtEnd()) return false;
		return peek().getType() == tokenType;
	}
	Token Parser::advance() {
		if (!isAtEnd()) {
			m_current++;
		}
		return previous();
	}
	bool Parser::isAtEnd() const {
		return peek().getType() == ENOF;
	}

	Token Parser::peek() const {
		return m_tokens[m_current];
	}
	Token Parser::previous() const {
		return m_tokens[m_current - 1];
	}
	Token Parser::consume(TokenType type, const std::string& message) {
		if (check(type)) return advance();

		throw error(peek(), message);
	}
	ParseError Parser::error(Token token, const std::string& message) {
		printf("Error bla bla %s\n", message.c_str());
		m_hasError = true;
		m_error = message;
		return ParseError(message.c_str());
	}
	void Parser::synchronize() {
		advance();

		while (!isAtEnd()) {
			if (previous().getType() == SEMICOLON) return;

			switch (peek().getType()) {
			case CLASS:
			case FUN:
			case VAR:
			case FOR:
			case IF:
			case WHILE:
			case PRINT:
			case RETURN:
				return;
			}

			advance();
		}
	}
}
