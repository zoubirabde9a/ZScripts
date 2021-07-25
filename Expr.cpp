#include "Expr.h"
#include "Stmt.h"

namespace ZScript {
	Assign::Assign(const Token& name, Expr* value) : m_name(name), m_value(value) {}
	Assign::~Assign() {
		m_name.free();
		if (m_value) delete m_value;
	}
	Expr* Assign::copy() const {
		if (m_value) {
			return new Assign(m_name.copy(), m_value->copy());
		}
		return new Assign(m_name.copy(), nullptr);
	}
	Literal Assign::accept(Expr::Visitor <Literal>* visitor) {
		return visitor->visitAssignExpr(this);
	}

	Binary::Binary(Expr* left, const Token &op, Expr* right) : m_left(left), m_operator(op), m_right(right) {}
	Binary::~Binary() {
		if (m_left) delete m_left;
		m_operator.free();
		if (m_right) delete m_right;
	}
	Expr* Binary::copy() const  {
		if (m_left && m_left) {
			return new Binary(m_left->copy(), m_operator.copy(), m_right->copy());
		}
		return new Binary(nullptr, m_operator.copy(), nullptr);
	}
	Literal Binary::accept(Expr::Visitor < Literal >* visitor) {
		return visitor->visitBinaryExpr(this);
	}
	Call::Call(Expr* callee, const Token &paren, const std::list < Expr* > &arguments)
		: m_callee(callee), m_paren(paren), m_arguments(arguments) {}
	Call::~Call() {
		if (m_callee) delete m_callee;
		m_paren.free();
		for (auto &it : m_arguments) {
			delete it;
		}
	}
	Expr* Call::copy() const {
		std::list < Expr* > arguments;
		for (auto &it : m_arguments) {
			if (it)arguments.push_back(it->copy());
		}
		if (m_callee) {
			return new Call(m_callee->copy(), m_paren.copy(), arguments);
		}
		return new Call(nullptr, m_paren.copy(), arguments);
	}
	Literal Call::accept(Expr::Visitor < Literal >* visitor) {
		return visitor->visitCallExpr(this);
	}
	FunctionExpr::FunctionExpr(const std::list < Token > &parameters, const std::list < StmtPtr >& body)
		: m_parameters(parameters), m_body(body) {}
	FunctionExpr::~FunctionExpr() {
		for (auto &it : m_parameters) {
			it.free();
		}
		/*for (auto &it : m_body) {
			delete it;
		}*/
	}
	Expr* FunctionExpr::copy() const {
		std::list < Token > parameters;
		//std::list < StmtPtr > body;
		for (auto &it : m_parameters) {
			parameters.push_back(it.copy());
		}
		/*for (auto &it : m_body) {
			if (it)body.push_back(it->copy());
		}*/
		return new FunctionExpr(parameters, m_body);
	}
	Literal FunctionExpr::accept(Expr::Visitor < Literal >* visitor) {
		return visitor->visitFunctionExpr(this);
	}

	Grouping::Grouping(Expr* expression) : m_expression(expression) {}
	Grouping::~Grouping() { if (m_expression) delete m_expression; }
	Expr* Grouping::copy() const {
		if (m_expression) {
			return new Grouping(m_expression->copy());
		}
		return nullptr;
	}
	Literal Grouping::accept(Expr::Visitor<Literal>* visitor) {
		return visitor->visitGroupingExpr(this);
	}

	LiteralExpr::LiteralExpr(const Literal& value) : m_value(value) {}
	LiteralExpr::~LiteralExpr() { m_value.free(); }
	Expr* LiteralExpr::copy() const {
		return new LiteralExpr(m_value.copy());
	}

	Literal LiteralExpr::accept(Expr::Visitor<Literal>* visitor) {
		return visitor->visitLiteralExpr(this);
	}


	Logical::Logical(Expr* left, const Token &op, Expr* right) : m_left(left), m_operator(op), m_right(right) {}
	Logical::~Logical() {
		if (m_left)delete m_left;
		m_operator.free();
		if (m_right) delete m_right;
	}
	Expr* Logical::copy() const {
		if (m_left && m_right) {
			return new Logical(m_left->copy(), m_operator.copy(), m_right->copy());
		}
		return new Logical(nullptr, m_operator.copy(), nullptr);
	}
	Literal Logical::accept(Expr::Visitor<Literal>* visitor) {
		return visitor->visitLogicalExpr(this);
	}
	Get::Get(Expr* expr, const Token &name) : m_expr(expr), m_name(name){

	}
	Get::~Get() {
		if (m_expr)delete m_expr;
		m_name.free();
	}
	Expr* Get::copy() const {
		if (m_expr) {
			return new Get(m_expr->copy(), m_name.copy());
		}
		return new Get(nullptr, m_name.copy());
	}
	Literal Get::accept(Expr::Visitor<Literal>* visitor) {
		return visitor->visitGetExpr(this);
	}
	Unary::Unary(const Token &op, Expr* right) : m_operator(op), m_right(right) {}
	Unary::~Unary() {
		m_operator.free();
		if (m_right)delete m_right;
	}
	Expr* Unary::copy() const {
		if (m_right) {
			return new Unary(m_operator.copy(), m_right->copy());
		}
		return new Unary(m_operator.copy(), nullptr);
	}
	Literal Unary::accept(Expr::Visitor<Literal>* visitor) {
		return visitor->visitUnaryExpr(this);
	}
	Variable::Variable(const Token &name) : m_name(name) {}
	Variable::~Variable() { m_name.free(); }
	Expr* Variable::copy() const {
		return new Variable(m_name.copy());
	}
	Literal Variable::accept(Expr::Visitor<Literal>* visitor) {
		return visitor->visitVariableExpr(this);
	}
}