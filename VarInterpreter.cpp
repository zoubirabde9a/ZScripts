#include "VarInterpreter.h"
#include "ZScriptFunctions.h"


namespace ZScript {
	VarInterpreter::VarInterpreter(Interpreter* interpreter) {
		m_interpreter = interpreter;
		m_globals = new Environment(nullptr);
		m_environment = m_globals;
	}
	VarInterpreter::~VarInterpreter() {
		delete m_globals;
		m_environment = nullptr;
		m_globals = nullptr;
	}
	void VarInterpreter::interpret(const std::vector <StmtPtr>& statements) {
		try {
			for (auto &it : statements) {
				execute(it);
			}
		}
		catch (RuntimeError& err) {
			error(err);
		}
	}
	Literal VarInterpreter::visitLiteralExpr(LiteralExpr* expr) {
		return expr->getValue();
	}
	Literal VarInterpreter::visitLogicalExpr(Logical* expr) {
		Literal left = evaluate(expr->getLeft());
		if (expr->getOperator().getType() == TokenType::OR) {
			if (isTruthy(left)) return left;
		}
		else {
			if (!isTruthy(left)) return left;
		}

		return evaluate(expr->getRight());
	}
	Literal VarInterpreter::visitGroupingExpr(Grouping* expr) {
		return evaluate(expr->getExpression());
	}
	Literal VarInterpreter::visitGetExpr(Get* expr) {
		Literal object = evaluate(expr->getExpr());
		if (object.isLightUserData() || object.isUserData()) {
			CClass* cclass = m_interpreter->getCClass(object.classId);
			if (cclass) {
				return Literal(cclass->get(expr->getName().getLexeme()), object.userData, object.classId);
			}
			else throw RuntimeError(expr->getName(),
				"c class not subscribed");
		}

		throw RuntimeError(expr->getName(),
			"Only instances and lightUserData have properties.");
	}
	Literal VarInterpreter::visitUnaryExpr(Unary* expr) {
		Literal right = evaluate(expr->getRight());

		switch (expr->getOperator().getType()) {
		case BANG:
			return Literal(!isTruthy(right));
		case MINUS:
			checkNumberOperand(expr->getOperator(), right);
			right.number = -right.number;
			return right;
		}

		// Unreachable.
		return Literal();
	}
	Literal VarInterpreter::visitVariableExpr(Variable* expr) {
		return m_environment->get(expr->getName());
	}
	Literal VarInterpreter::visitBinaryExpr(Binary* expr) {
		Literal left = evaluate(expr->getLeft());
		Literal right = evaluate(expr->getRight());

		switch (expr->getOperator().getType()) {
		case GREATER:
			checkNumberOperands(expr->getOperator(), left, right);
			return Literal(left.number > right.number);
		case GREATER_EQUAL:
			checkNumberOperands(expr->getOperator(), left, right);
			return Literal(left.number >= right.number);
		case LESS:
			checkNumberOperands(expr->getOperator(), left, right);
			return Literal(left.number < right.number);
		case LESS_EQUAL:
			checkNumberOperands(expr->getOperator(), left, right);
			return Literal(left.number <= right.number);
		case MINUS:
		{
			checkNumberOperands(expr->getOperator(), left, right);
			left.number = left.number - right.number;
			return left;
			break;
		}
		case BANG_EQUAL: return Literal((!isEqual(left, right))); break;
		case EQUAL_EQUAL: return Literal((isEqual(left, right))); break;
		case PLUS:
		{
			if (left.type == VAR_NUMBER && right.type == VAR_NUMBER) {
				left.number = left.number + right.number;
				return left;
			}

			if (left.type == VAR_STRING && right.type == VAR_STRING) {
				char* str = strcat(left.string, right.string);
				delete[] left.string;
				left.string = str;
				return left;
			}
			throw RuntimeError(expr->getOperator(), "Operands must be two numbers or two strings.");
			break;
		}
		case SLASH:
		{
			checkNumberOperands(expr->getOperator(), left, right);
			left.number = left.number / right.number;
			return left;
			break;
		}
		case STAR:
		{
			checkNumberOperands(expr->getOperator(), left, right);
			left.number = left.number * right.number;
			return left;
			break;
		}
		}

		// Unreachable.
		return Literal();
	}
	Literal VarInterpreter::visitCallExpr(Call* expr) {
		Literal callee = evaluate(expr->getCallee());
		std::vector < Literal > arguments;
		arguments.reserve(8);
		for (auto argument : expr->getArguments()) {
			arguments.push_back(evaluate(argument));
		}

		if (callee.type == VAR_CALLABLE || callee.type == VAR_FUNCTION) {
			const LoxCallable* function = callee.callable;

			if (arguments.size() != function->arity()) {
				throw RuntimeError(expr->getParen(), "Expected " +
					std::to_string(function->arity()) + " arguments but got " +
					std::to_string(arguments.size()) + ".");
			}

			if (callee.callPtr) {
				arguments.push_back(Literal(callee.callPtr, callee.classId));
			}


			return function->call(m_interpreter, arguments);
		}
		else if (callee.type == VAR_CFUNCTION) {
			LoxCFunction* function = callee.cFunction;
			return function->call(m_interpreter, arguments);
		}
		else {
			throw RuntimeError(expr->getParen(),
				"Can only call functions and classes.");
		}
	}
	Literal VarInterpreter::visitFunctionExpr(FunctionExpr* expr) {
		return Literal(new LoxFunction(expr->getParameters(), expr->getBody(), "unamed", m_environment));
	}
	void VarInterpreter::visitExpressionStmt(Expression* stmt) {
		evaluate(stmt->getExpression());
	}
	void VarInterpreter::visitFunctionStmt(FunctionStmt* stmt) {
		Literal function = Literal(new LoxFunction(stmt->getParameters(), stmt->getBody(), stmt->getName().getLexeme(), m_environment));
		m_environment->define(stmt->getName().getLexeme(), function);
	}
	void VarInterpreter::visitIfStmt(If* stmt) {
		if (isTruthy(evaluate(stmt->getCondition()))) {
			execute(stmt->getThenBranch());
		}
		else if (stmt->getElseBranch() != nullptr) {
			execute(stmt->getElseBranch());
		}
	}
	void VarInterpreter::visitPrintStmt(Print* stmt) {
		Literal value = evaluate(stmt->getExpression());
		/*printf("%s\n", value.toString().c_str());*/
	}
	void VarInterpreter::visitReturnStmt(Return* stmt) {
		Literal value;
		if (stmt->getValue() != nullptr) value = evaluate(stmt->getValue());

		throw ReturnException(value);
	}
	void VarInterpreter::visitVarStmt(Var* stmt) {
		Literal value;
		if (stmt->getInitializer() != nullptr) {
			value = evaluate(stmt->getInitializer());
		}
		m_environment->define(stmt->getName().getLexeme(), value);;
	}
	void VarInterpreter::visitWhileStmt(While* stmt) {
		while (isTruthy(evaluate(stmt->getCondition()))) {
			execute(stmt->getBody());
		}
	}
	Literal VarInterpreter::visitAssignExpr(Assign* expr) {
		Literal value = evaluate(expr->getValue());
		m_environment->assign(expr->getName(), value);
		return value;
	}
	void VarInterpreter::visitBlockStmt(Block* stmt) {
		executeBlock(stmt->getStatements(), m_environment->add());
	}
	void VarInterpreter::executeBlock(const std::list < StmtPtr >& statements, Environment* environment) {
		Environment* previous = m_environment;
		try {
			m_environment = environment;

			for (auto &it : statements) {
				execute(it);
			}
		}
		catch (RuntimeError& e) {
			error(e);
		}
		m_environment = previous;
	}
	void VarInterpreter::executeBlock(const std::vector < StmtPtr >& statements, Environment* environment) {
		Environment* previous = m_environment;
		try {
			m_environment = environment;

			for (auto &it : statements) {
				execute(it);
			}
		}
		catch (RuntimeError& e) {
			error(e);
		}
		m_environment = previous;
	}
	Literal VarInterpreter::evaluateExpressionStmt(Expression* exprStmt) {
		return exprStmt->getExpression()->accept(this);
	}
	Literal VarInterpreter::evaluateExpressionStmt(Expression* exprStmt, Environment* environment) {
		Literal lit;
		Environment* previous = m_environment;
		m_environment = environment;
		lit = exprStmt->getExpression()->accept(this);
		m_environment = previous;
		return lit;
	}
	void VarInterpreter::execute(StmtPtr stmt) {
		stmt->accept(this);
	}
	void VarInterpreter::execute(StmtPtr stmt, Environment* environement) {
		Environment* previous = m_environment;
		m_environment = environement;
		stmt->accept(this);
		m_environment = previous;
	}
	bool VarInterpreter::isTruthy(const Literal& object) {
		if (object.isNil()) return false;
		if (object.isBoolean()) return object.boolean;
		if (object.isLightUserData() || object.isUserData()) return object.userData != nullptr;
		return true;
	}
	Literal VarInterpreter::evaluate(Expr* expr) {
		return expr->accept(this);
	}
	bool VarInterpreter::isEqual(const Literal& a, const Literal& b) {
		// nil is only equal to nil.
		if (a.type != b.type) {
			if (a.type == VAR_BOOLEAN && b.type == VAR_NUMBER) {
				return (double)a.boolean == b.number;
			}
			if (a.type == VAR_NUMBER && b.type == VAR_BOOLEAN) {
				return a.number == (double)b.boolean;
			}
			return false;
		}

		if (a.type == VAR_BOOLEAN) {
			return a.boolean == b.boolean;
		}
		if (a.type == VAR_NUMBER) {
			return a.number == b.number;
		}
		if (a.type == VAR_STRING) {
			return strcmp(a.string, b.string) == 0;
		}
		return true;
	}
	void VarInterpreter::checkNumberOperand(const Token& op, const Literal& operand) {
		if (operand.type == VAR_NUMBER) return;
		throw RuntimeError(op, "Operand must be a number.");
	}
	void VarInterpreter::checkNumberOperands(const Token &op, const Literal& left, const Literal& right) {
		if (left.type == VAR_NUMBER && right.type == VAR_NUMBER) return;
		throw RuntimeError(op, "Operands must be numbers.");
	}
	std::string VarInterpreter::stringify(const Literal& object) {
		return object.toString();
	}
	void VarInterpreter::error(RuntimeError& err) {
		printf("%s\n", err.what());
	}
}
