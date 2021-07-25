#include "Stmt.h"
#include "Expr.h"

namespace ZScript {
	Block::Block(const std::list < StmtPtr >& statements) : m_statements(statements) {}
	Block::~Block() {
		/*for (auto &it : m_statements) {
			delete it;
		}*/
	}
	/*Stmt* Block::copy() const {
		std::list < Stmt* > statements;
		for (auto &it : m_statements) {
			if (it)statements.push_back(it->copy());
		}
		return new Block(statements);
	}*/
	void Block::accept(Visitor <void>* visitor) {
		return visitor->visitBlockStmt(this);
	}
	Expression::Expression(Expr* expression) : m_expression(expression) {}
	Expression::~Expression() { if (m_expression) delete m_expression; }
	/*Stmt* Expression::copy() const {
		if (m_expression) {
			return new Expression(m_expression->copy());
		}
		return nullptr;
	}*/
	void Expression::accept(Visitor <void>* visitor) {
		return visitor->visitExpressionStmt(this);
	}

	FunctionStmt::FunctionStmt(const Token& name, const std::list < Token > & parameters,
		const std::list < StmtPtr > &body) : m_name(name), m_parameters(parameters),
		m_body(body) { }
	FunctionStmt::~FunctionStmt() {
		m_name.free();
		for (auto &it : m_parameters) {
			it.free();
		}
		/*for (auto &it : m_body) {
			delete it;
		}*/
	}
	/*Stmt* FunctionStmt::copy() const {
		std::list < Token > parameters;
		std::list < Stmt* > body;
		for (auto &it : m_parameters) {
			parameters.push_back(it.copy());
		}
		for (auto &it : m_body) {
			if (it)body.push_back(it->copy());
		}
		return new FunctionStmt(m_name.copy(), parameters, body);
	}*/
	void FunctionStmt::accept(Visitor < void >* visitor) {
		return visitor->visitFunctionStmt(this);
	}


	If::If(Expr* condition, StmtPtr thenBranch, StmtPtr elseBranch) : m_condition(condition),
		m_thenBranch(thenBranch), m_elseBranch(elseBranch) {}
	If::~If() {
		if (m_condition) delete m_condition;
	/*	if (m_thenBranch) delete m_thenBranch;
		if (m_elseBranch) delete m_elseBranch;*/
	}
	/*Stmt* If::copy() const {
		if (m_condition) {
			if (m_thenBranch && m_elseBranch) {
				return new If(m_condition->copy(), m_thenBranch->copy(), m_elseBranch->copy());
			}
			return new If(m_condition->copy(), nullptr, nullptr);
		}
		if (m_thenBranch && m_elseBranch) {
			return new If(nullptr, m_thenBranch->copy(), m_elseBranch->copy());
		}
		return nullptr;
	}*/
	void If::accept(Visitor <void>* visitor) {
		return visitor->visitIfStmt(this);
	}

	Print::Print(Expr* expression) : m_expression(expression) {}
	Print::~Print() { if (m_expression) delete m_expression; }
	/*Stmt* Print::copy() const {
		if (m_expression) {
			return new Print(m_expression->copy());
		}
		return nullptr;
	}*/
	void Print::accept(Visitor <void>* visitor) {
		return visitor->visitPrintStmt(this);
	}

	Return::Return(const Token& keyword, Expr* value) : m_keyword(keyword), m_value(value) {}
	Return::~Return() {
		m_keyword.free();
		if (m_value) delete m_value;
	}
	/*Stmt* Return::copy() const {
		if (m_value) {
			return new Return(m_keyword.copy(), m_value->copy());
		}
		return new Return(m_keyword.copy(), nullptr);
	}*/
	void Return::accept(Visitor <void>* visitor) {
		return visitor->visitReturnStmt(this);
	}

	Var::Var(const Token& name, Expr* initializer) : m_name(name), m_initializer(initializer) {}
	Var::~Var() {
		m_name.free();
		if (m_initializer) delete m_initializer;
	}
	/*Stmt* Var::copy() const {
		if (m_initializer) {
			return new Var(m_name.copy(), m_initializer->copy());
		}
		return new Var(m_name.copy(), nullptr);
	}*/
	void Var::accept(Visitor<void>* visitor) {
		return visitor->visitVarStmt(this);
	}

	While::While(Expr* condition, StmtPtr body) : m_condition(condition), m_body(body) {}
	While::~While() {
		//if (m_body) delete m_body;
		if (m_condition) delete m_condition;
	}
	/*Stmt* While::copy() const {
		if (m_body) {
			if (m_condition) {
				return new While(m_condition->copy(), m_body->copy());
			}
			return new While(nullptr, m_body->copy());
		}
		if (m_condition) {
			return new While(m_condition->copy(), nullptr);
		}
		return nullptr;
	}*/
	void While::accept(Visitor<void>* visitor) {
		return visitor->visitWhileStmt(this);
	}
}