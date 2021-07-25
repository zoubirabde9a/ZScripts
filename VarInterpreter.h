#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include "RuntimeError.h"

namespace ZScript {
	class Interpreter;
	class VarInterpreter : public Expr::Visitor < Literal >, Stmt::Visitor < void >
	{
	public:
		VarInterpreter(Interpreter* interpreter);
		~VarInterpreter();

		void interpret(const std::vector < StmtPtr >& statements);

		Literal visitLiteralExpr(LiteralExpr* expr) override;
		Literal visitLogicalExpr(Logical* expr) override;
		Literal visitGroupingExpr(Grouping* expr) override;
		Literal visitGetExpr(Get* expr) override;
		Literal visitUnaryExpr(Unary* expr)override;
		Literal visitVariableExpr(Variable* expr) override;
		Literal visitBinaryExpr(Binary* expr) override;
		Literal visitCallExpr(Call* expr) override;
		Literal visitFunctionExpr(FunctionExpr* expr) override;
		Literal visitAssignExpr(Assign* expr) override;
		void visitExpressionStmt(Expression* stmt) override;
		void visitFunctionStmt(FunctionStmt* stmt) override;
		void visitIfStmt(If* stmt)override;
		void visitPrintStmt(Print* stmt) override;
		void visitReturnStmt(Return* stmt) override;
		void visitVarStmt(Var* stmt) override;
		void visitWhileStmt(While* stmt) override;
		void visitBlockStmt(Block* stmt) override;
		void executeBlock(const std::list <StmtPtr>& statements, Environment* environment);
		void executeBlock(const std::vector <StmtPtr>& statements, Environment* environment);


		Literal evaluateExpressionStmt(Expression* exprStmt);
		Literal evaluateExpressionStmt(Expression* exprStmt, Environment* environment);

		inline Environment* getGlobalEnvironement() { return m_globals; }
		inline Environment* getEnvironment() { return m_environment; }
	private:
		friend class Session;
		void execute(StmtPtr stmt);
		void execute(StmtPtr stmt, Environment* environement);
		bool isTruthy(const Literal& object);
		Literal evaluate(Expr* expr);
		bool isEqual(const Literal& a, const Literal& b);
		void checkNumberOperand(const Token &op, const Literal& operand);
		void checkNumberOperands(const Token &op, const Literal& left, const Literal& right);
		std::string stringify(const Literal& object);
		void error(RuntimeError& err);
	private:
		Interpreter* m_interpreter;
		Environment* m_environment;
		Environment* m_globals;
	};
}
