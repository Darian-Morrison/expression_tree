/* expression_tree.cpp
   CSC 116 - Fall 2016
   Darian Morrison
*/

#include <string>
#include <cmath> //For exp(), log(), sin(), cos() and pow()
#include "expression_tree.hpp"

/* Constant */
double TreeNodeConstant::evaluate( double x ){
	return constant_value;
}
TreeNodePtr TreeNodeConstant::differentiate( ){
	//Derivative of a constant is 0
	TreeNodePtr constant_node = std::make_shared<TreeNodeConstant>(0);

	return constant_node;
}
TreeNodePtr TreeNodeConstant::duplicate( ){
	TreeNodePtr constant_node = std::make_shared<TreeNodeConstant>(constant_value);
	return constant_node;

}

/* Literal X */
double TreeNodeX::evaluate( double x ){
	return x;
}
TreeNodePtr TreeNodeX::differentiate( ){
	//Derivative of x is 1
	TreeNodePtr constant_node = std::make_shared<TreeNodeConstant>(1);

	return constant_node;
}
TreeNodePtr TreeNodeX::duplicate( ){
	
	TreeNodePtr x_node = std::make_shared<TreeNodeX>();

	return x_node;
}

/* Plus operator */
double TreeNodePlus::evaluate( double x ){
	double evaluation =(*left).evaluate(x)+(*right).evaluate(x);
	return evaluation;
}
TreeNodePtr TreeNodePlus::differentiate( ){
	//Derivative of a + b is the sum of the derivatives of each.
	TreeNodePtr sum_node = std::make_shared<TreeNodePlus>((*left).differentiate(), (*right).differentiate());
	return sum_node;
}
TreeNodePtr TreeNodePlus::duplicate( ){
	TreeNodePtr sum_node = std::make_shared<TreeNodePlus>(left,right);
	return sum_node;
}

/* Minus operator */
double TreeNodeMinus::evaluate( double x ){
	double evaluation = (*left).evaluate(x) - (*right).evaluate(x);
	return evaluation;
}
TreeNodePtr TreeNodeMinus::differentiate( ){
	//Derivative of a - b is the difference of the derivatives of each.
	TreeNodePtr differance_node = std::make_shared<TreeNodeMinus>((*left).differentiate(), (*right).differentiate());
	return differance_node;
}
TreeNodePtr TreeNodeMinus::duplicate( ){
	TreeNodePtr differance_node = std::make_shared<TreeNodeMinus>(left, right);
	return differance_node;
}

/* Multiplication operator */
double TreeNodeMultiply::evaluate( double x ){
	double evaluation = (*left).evaluate(x) * (*right).evaluate(x);
	return evaluation;
}
TreeNodePtr TreeNodeMultiply::differentiate() {
	//d/dx ( a*b ) = (d/dx a)*b +  a*(d/dx b)

	TreeNodePtr left_node = std::make_shared<TreeNodeMultiply>((*left).differentiate(), right);
	TreeNodePtr right_node = std::make_shared<TreeNodeMultiply>(left, (*right).differentiate());
	TreeNodePtr product_node = std::make_shared<TreeNodePlus>(left_node, right_node);
	return product_node;
}
TreeNodePtr TreeNodeMultiply::duplicate( ){
	TreeNodePtr product_node = std::make_shared<TreeNodeMinus>(left, right);
	return product_node;
}

/* Division operator */
double TreeNodeDivide::evaluate( double x ){
	double quotient = ((*left).evaluate(x) / (*right).evaluate(x));
	return quotient;
}
TreeNodePtr TreeNodeDivide::differentiate( ){
	//d/dx ( a/b ) = ((d/dx a)*b - a*(d/dx b))/b^2
	TreeNodePtr left_node = std::make_shared<TreeNodeMultiply>((*left).differentiate(), right);
	TreeNodePtr right_node = std::make_shared<TreeNodeMultiply>(left, (*right).differentiate());
	TreeNodePtr differance_node = std::make_shared<TreeNodeMinus>(left_node,right_node);
	TreeNodePtr square_node = std::make_shared<TreeNodeConstantPower>(right, 2);
	TreeNodePtr quotient_node = std::make_shared<TreeNodeDivide>(differance_node,square_node);

	return quotient_node;
}
TreeNodePtr TreeNodeDivide::duplicate( ){
	TreeNodePtr quotient_node = std::make_shared<TreeNodeDivide>(left, right);
	return quotient_node;
}

/* Constant power operator (the exponent will always be a fixed value) */
double TreeNodeConstantPower::evaluate( double x ){
	double power = pow((*left).evaluate(x), exponent);
	return power;
}
TreeNodePtr TreeNodeConstantPower::differentiate() {
	//d/dx ( a^c ) = c*(d/dx a)*(a)^(c-1)
	TreeNodePtr exponent_node = std::make_shared<TreeNodeConstant>(exponent);
	TreeNodePtr product_node1 = std::make_shared<TreeNodeMultiply>(exponent_node, (*left).differentiate());
	TreeNodePtr power_node = std::make_shared<TreeNodeConstantPower>(left, exponent-1);
	TreeNodePtr product_node2 = std::make_shared<TreeNodeMultiply>(product_node1, power_node);
	return product_node2;
}
TreeNodePtr TreeNodeConstantPower::duplicate( ){
	TreeNodePtr power_node = std::make_shared<TreeNodeConstantPower>(left, exponent);
	return power_node;
}

/* exp function */
double TreeNodeExpFunction::evaluate( double x ){
	double exponential = exp((*argument).evaluate(x));
	return exponential;
}
TreeNodePtr TreeNodeExpFunction::differentiate( ){
	//d/dx ( e^f(x) ) = (e^f(x))*f'(x)
	TreeNodePtr left_node = std::make_shared<TreeNodeExpFunction>(argument);
	TreeNodePtr product_node = std::make_shared<TreeNodeMultiply>(left_node,(*argument).differentiate());
	return product_node;
}
TreeNodePtr TreeNodeExpFunction::duplicate( ){
	TreeNodePtr exp_node = std::make_shared<TreeNodeExpFunction>(argument);
	return exp_node;
}

/* log function */
double TreeNodeLogFunction::evaluate( double x ){
	double logarithm = log((*argument).evaluate(x));
	return logarithm;
}
TreeNodePtr TreeNodeLogFunction::differentiate( ){
	//d/dx ( log( f(x) ) ) = (1/f(x))*f'(x) = f'(x)/f(x)
	TreeNodePtr quotient_node = std::make_shared<TreeNodeDivide>((*argument).differentiate(),argument);
	return quotient_node;
}
TreeNodePtr TreeNodeLogFunction::duplicate( ){
	TreeNodePtr log_node = std::make_shared<TreeNodeLogFunction>(argument);

	return log_node;
}

/* sin function */
double TreeNodeSinFunction::evaluate( double x ){
	double sinFunction = sin((*argument).evaluate(x));
	return sinFunction;
}
TreeNodePtr TreeNodeSinFunction::differentiate( ){
	//d/dx ( sin(f(x)) ) = cos(f(x))*f'(x)
	TreeNodePtr cos_node = std::make_shared<TreeNodeCosFunction>(argument);
	TreeNodePtr product_node = std::make_shared<TreeNodeMultiply>(cos_node, (*argument).differentiate());

	return product_node;
}
TreeNodePtr TreeNodeSinFunction::duplicate( ){
	TreeNodePtr sin_node = std::make_shared<TreeNodeCosFunction>(argument);

	return sin_node;
}

/* cos function */
double TreeNodeCosFunction::evaluate( double x ){
	double cosFunction = cos((*argument).evaluate(x));
	return cosFunction;
}
TreeNodePtr TreeNodeCosFunction::differentiate( ){
	//d/dx ( cos(f(x)) ) = -1*sin(f(x))*f'(x)
	TreeNodePtr sin_node = std::make_shared<TreeNodeSinFunction>(argument);
	TreeNodePtr constant_node = std::make_shared<TreeNodeConstant>(-1);
	TreeNodePtr product_node1 = std::make_shared<TreeNodeMultiply>(constant_node, sin_node);
	TreeNodePtr product_node2 = std::make_shared<TreeNodeMultiply>(product_node1, (*argument).differentiate());

	return product_node2;
}
TreeNodePtr TreeNodeCosFunction::duplicate( ){
	TreeNodePtr cos_node = std::make_shared<TreeNodeCosFunction>(argument);

	return cos_node;
}



