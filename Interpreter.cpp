#include "Interpreter.h"
#include <time.h>
#include "ZScriptFunctions.h"


namespace ZScript {
	Literal wait(Interpreter* interpreter, const std::vector < Literal >& arguments) {
		if (arguments.size() >= 1) {
			Literal sessionPtr = interpreter->getEnvironment()->get("__session");
			if (sessionPtr.isLightUserData() && arguments[0].isNumber()) {
				((Session*)(sessionPtr.userData))->wait(arguments[0].number);
			}
		}
		return Literal();
	}
	void Session::setStatements(const std::vector < StmtPtr > &statements) {
		m_statements = statements;
	}
	void Session::update(Interpreter* interpreter) {
		while (/*m_timer.getTicks() >= m_waitTime && */!m_wait && m_cs < m_statements.size()) {
			try {
				interpreter->execute(m_statements[m_cs], m_environement);

			}
			catch (ZScript::RuntimeError& e) {
				printf("%s\n", e.what());
			}
			m_cs++;
		}
	}
	void Session::wait(float time) {
		m_waitTime = time;
		//m_timer.restart();
	}
	void Session::wait() {
		m_wait = true;
	}
	void Session::resume() {
		m_wait = false;
		m_waitTime = 0.0f;
	}
	Interpreter::Interpreter() {
		m_globals = new Environment(nullptr);
		m_regestry = new Environment(nullptr);
		m_environment = m_globals;
		m_globals->define("clock",
			new LoxCallable (
				[](Interpreter* interpreter, const std::vector < Literal >& statements) {
			return Literal(((double)clock() / 1000.0));
		}, 0));
		subscribeClass < Session >("Session")
			.addFunction("wait", (void(Session::*)(float))&Session::wait);

		subscribeCFunction("wait", wait);
	}
	Interpreter::~Interpreter() {
		delete m_globals;
		delete m_regestry;
		m_environment = nullptr;
		m_globals = nullptr;
		m_regestry = nullptr;
	}
	void Interpreter::updateSessions() {
		for (auto it = m_sessions.begin(); it != m_sessions.end();) {
			m_globals->define("__session", Literal(&(*it)));
			it->update(this);
			if (it->isFinished()) {
				m_globals->remove(it->getEnvironement());
				it = m_sessions.erase(it);
			}
			else {
				it++;
			}
		}
	}
	Session* Interpreter::createSession(const std::vector < StmtPtr >& statements) {
		m_sessions.push_back(Session(statements, m_globals->add()));
		return &m_sessions.back();
	}
	Session* Interpreter::createSession(const std::vector < StmtPtr >& statements, Environment* environment) {
		m_sessions.push_back(Session(statements, environment->add()));
		return &m_sessions.back();
	}
	void Interpreter::interpret(const std::vector <StmtPtr>& statements) {
		try {
			for (auto &it : statements) {
				execute(it);
			}
		}
		catch (RuntimeError& err) {
			error(err);
		}
	}
	void Interpreter::interpret(const std::vector < StmtPtr >& statements, Environment* environment) {
		Environment* previous = m_environment;
		m_environment = environment;
		interpret(statements);
		m_environment = previous;
	}
	/*Environment* Interpreter::interpretVariables(const std::vector < StmtPtr > &statements) {

	}*/
	void Interpreter::subscribeCFunction(const std::string& name, CFunction fn) {
		m_globals->define(name, new LoxCFunction(fn));
	}
	void Interpreter::subscribeFunction(const std::string&name, const Function& fn, size_t arity) {
		m_globals->define(name, new LoxCallable(fn, arity));
	}
	Literal Interpreter::visitLiteralExpr(LiteralExpr* expr) {
		return expr->getValue();
	}
	Literal Interpreter::visitLogicalExpr(Logical* expr) {
		Literal left = evaluate(expr->getLeft());
		if (expr->getOperator().getType() == TokenType::OR) {
			if (isTruthy(left)) return left;
		}
		else {
			if (!isTruthy(left)) return left;
		}

		return evaluate(expr->getRight());
	}
	Literal Interpreter::visitGroupingExpr(Grouping* expr) {
		return evaluate(expr->getExpression());
	}
	Literal Interpreter::visitGetExpr(Get* expr) {
		Literal object = evaluate(expr->getExpr());
		if (object.isLightUserData() || object.isUserData()) {
			CClass* cclass = getCClass(object.classId);
			if (cclass) {
				return Literal(cclass->get(expr->getName().getLexeme()), object.userData, object.classId);
			} else throw RuntimeError(expr->getName(),
				"c class not subscribed");
		}

		throw RuntimeError(expr->getName(),
			"Only instances and lightUserData have properties.");
	}
	Literal Interpreter::visitUnaryExpr(Unary* expr) {
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
	Literal Interpreter::visitVariableExpr(Variable* expr) {
		return m_environment->get(expr->getName());
	}
	Literal Interpreter::visitBinaryExpr(Binary* expr) {
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
				/*delete[] left.string;
				left.string = str;*/
				return str;
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
	Literal Interpreter::visitCallExpr(Call* expr) {
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


			return function->call(this, arguments);
		}
		else if (callee.type == VAR_CFUNCTION) {
			LoxCFunction* function = callee.cFunction;
			return function->call(this, arguments);
		}
		else {
			throw RuntimeError(expr->getParen(),
				"Can only call functions and classes.");
		}
	}
	Literal Interpreter::visitFunctionExpr(FunctionExpr* expr) {
		return Literal(new LoxFunction(expr->getParameters(), expr->getBody(), "unamed", m_environment));
	}
	void Interpreter::visitExpressionStmt(Expression* stmt) {
		evaluate(stmt->getExpression());
	}
	void Interpreter::visitFunctionStmt(FunctionStmt* stmt) {
		Literal function = Literal(new LoxFunction(stmt->getParameters(), stmt->getBody(), stmt->getName().getLexeme(), m_environment));
		m_environment->define(stmt->getName().getLexeme(), function);
	}
	void Interpreter::visitIfStmt(If* stmt) {
		if (isTruthy(evaluate(stmt->getCondition()))) {
			execute(stmt->getThenBranch());
		}
		else if (stmt->getElseBranch() != nullptr) {
			execute(stmt->getElseBranch());
		}
	}
	void Interpreter::visitPrintStmt(Print* stmt) {
		Literal value = evaluate(stmt->getExpression());
		printf("%s\n", value.toString().c_str());
	}
	void Interpreter::visitReturnStmt(Return* stmt) {
		Literal value;
		if (stmt->getValue() != nullptr) value = evaluate(stmt->getValue());

		throw ReturnException(value);
	}
	void Interpreter::visitVarStmt(Var* stmt) {
		Literal value;
		if (stmt->getInitializer() != nullptr) {
			value = evaluate(stmt->getInitializer());
		}
		m_environment->define(stmt->getName().getLexeme(), value);;
	}
	void Interpreter::visitWhileStmt(While* stmt) {
		while (isTruthy(evaluate(stmt->getCondition()))) {
			execute(stmt->getBody());
		}
	}
	Literal Interpreter::visitAssignExpr(Assign* expr) {
		Literal value = evaluate(expr->getValue());
		m_environment->assign(expr->getName(), value);
		return value;
	}
	void Interpreter::visitBlockStmt(Block* stmt) {
		executeBlock(stmt->getStatements(), m_environment->add());
	}
	void Interpreter::executeBlock(const std::list < StmtPtr >& statements, Environment* environment) {
		Environment* previous = m_environment;
		m_environment = environment;
		try {

			for (auto &it : statements) {
				execute(it);
			}
		}
		catch (RuntimeError& e) {
			error(e);
		}
		m_environment = previous;
	}
	void Interpreter::executeBlock(const std::vector < StmtPtr >& statements, Environment* environment) {
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
	Literal Interpreter::evaluateExpressionStmt(Expression* exprStmt) {
		return exprStmt->getExpression()->accept(this);
	}
	Literal Interpreter::evaluateExpressionStmt(Expression* exprStmt, Environment* environment) {
		Literal lit;
		Environment* previous = m_environment;
		m_environment = environment;
		lit = exprStmt->getExpression()->accept(this);
		m_environment = previous;
		return lit;
	}
	void Interpreter::execute(StmtPtr stmt) {
		stmt->accept(this);
	}
	void Interpreter::execute(StmtPtr stmt, Environment* environement) {
		Environment* previous = m_environment;
		m_environment = environement;
		stmt->accept(this);
		m_environment = previous;
	}
	bool Interpreter::isTruthy(const Literal& object) {
		if (object.isNil()) return false;
		if (object.isBoolean()) return object.boolean;
		if (object.isLightUserData() || object.isUserData()) return object.userData!= nullptr;
		return true;
	}
	Literal Interpreter::evaluate(Expr* expr) {
		return expr->accept(this);
	}
	bool Interpreter::isEqual(const Literal& a, const Literal& b) {
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
	void Interpreter::checkNumberOperand(const Token& op, const Literal& operand) {
		if (operand.type == VAR_NUMBER) return;
		throw RuntimeError(op, "Operand must be a number.");
	}
	void Interpreter::checkNumberOperands(const Token &op, const Literal& left, const Literal& right) {
		if (left.type == VAR_NUMBER && right.type == VAR_NUMBER) return;
		throw RuntimeError(op, "Operands must be numbers.");
	}
	std::string Interpreter::stringify(const Literal& object) {
		return object.toString();
	}
	void Interpreter::error(RuntimeError& err) {
		printf("%s\n", err.what());
	}
}
