#pragma once
#include "Token.h"
#include <list>
#include <vector>
#include <exception>
#include "Stmt.h"
#include "Expr.h"

namespace ZScript {
	class ParseError : public std::exception {
	public:
		ParseError(const char* err) : m_error(err) {}
		virtual const char* what() const throw()
		{
			return m_error.c_str();
		}
	private:
		std::string m_error;
	};
	class Parser
	{
	public:
		Parser(const std::vector < Token > &tokens);
		void parse(std::vector < StmtPtr > &statements);
		StmtPtr parseExpressionStatement();
		bool hasError() const { return m_hasError; }

		const std::string& getError() const { return m_error; }
	private:
		void declarationCount();
		void functionCount();
		void varDeclarationCount();
		void statementCount();
		void forStatementCount();
		void whileStatementCount();
		void returnStatementCount();
		void ifStatementCount();
		void blockCount();
		void printStatementCount();
		void expressionStatementCount();
		void expressionCount();
		void assignmentCount();
		void orCount();
		void andCount();
		void equalityCount();
		void comparisonCount();
		void additionCount();
		void multiplicationCount();
		void unaryCount();
		void callCount();
		void finishCallCount();
		void functionExpressionCount();
		void primaryCount();


		StmtPtr declaration();
		StmtPtr function(const std::string& name);
		StmtPtr varDeclaration();
		StmtPtr statement();
		StmtPtr forStatement();
		StmtPtr whileStatement();
		StmtPtr returnStatement();
		StmtPtr ifStatement();
		void block(std::list < StmtPtr >& statements);
		StmtPtr printStatement();
		StmtPtr expressionStatement();
		Expr* expression();
		Expr* assignment();
		Expr* or ();
		Expr* and ();
		Expr* equality();
		Expr* comparison();
		Expr* addition();
		Expr* multiplication();
		Expr* unary();
		Expr* call();
		Expr* finishCall(Expr* callee);
		Expr* functionExpression();
		Expr* primary();
		bool match(const std::vector < TokenType >& types);
		bool match(TokenType type);
		bool check(TokenType tokenType);
		Token advance();
		bool isAtEnd() const;
		Token peek() const;
		Token previous() const;
		Token consume(TokenType type, const std::string& message);
		ParseError error(Token token, const std::string& message);
		void synchronize();
	private:
		std::vector < Token > m_tokens;
		size_t m_current = 0;
		bool m_hasError = false;
		std::string m_error;
	};
}

