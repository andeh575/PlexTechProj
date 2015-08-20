/*	Author:		Andrew Graham
/	Date:		20 August 2015
/	Purpose:	Solves simple mathematetical expressions - program accepts expressions in infix notation, converts them to postfix, 
/				and assembles them into a binary expression tree for solving. 
*/

#include <iostream>
#include <string>
#include <stack>
using namespace std;

struct node {
	char val;
	node* left;
	node* right;
};

string getInfix(void);		// Helper function to get initial infix expression
void pressToContinue();		// Helper function for console development
bool isOperator(char m);	// Helper function for identifying mathematical operators							
struct node* parseExpression(string m, struct node* root);	// Parse the input stack for insertion into the expression tree

int main(void) {
	string infix;					// Input string
	struct node* root = new node;	// The root of the expression tree

	cout << "Welcome to a Simple Infix Equation Solver" << endl;

	// Parsing the input stack for insertion onto the expression tree
	infix = getInfix();
	root = parseExpression(infix, root);

	// Solve the equation

	pressToContinue();
	return 0;
};

// Helper function to get initial expression - should be infix notation (We don't currently test)
string getInfix(void) {

	string infix;

	cout << "Please enter an equation in infix format (Acceptable Operators: + and *):" << endl;
	cin >> infix;

	return(infix);
};

// Helper function for console development
void pressToContinue(void) {
	cout << endl << "Press ENTER to continue..." << flush;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
};

// Helper function for identifying operators
bool isOperator(char val) {
	if (val == '+' || val == '*') // Current implementation only supports addition and multiplication
		return true;
	else
		return false;
};

// Parse the infix string for insertion into an expression tree
struct node* parseExpression(string infix, struct node* root) {
	char temp;				
	stack<char> input;		// Stack for input string
	stack<node*> operators;	// Stack for handling operators in the input stack
	stack<node*> treeNodes;	// Stack for arranging nodes for tree assembly

	// Let's get the initial input in infix notation and break it down onto the input stack
	for (int i = 0; i < infix.length(); i++)
		input.push(infix[i]);

	while (!input.empty()) {
		// Take first item off the input stack (Reminder: First in, last out)
		temp = input.top();
		input.pop();

		if (isdigit(temp))	// Is it an operand? 
			cout << "That was a digit" << endl; // Turn it into a node and push it onto the treeNode stack
		if (temp == ')')	// Is it a closing parenthesis?
			cout << "That was a closing parenthesis" << endl; // Turn it into a node and push it onto the operator stack
		if (isOperator(temp)) { // Is it an operator? Then it depends on...
			cout << "That was an operator" << endl;
			// Is the operator stack empty? Turn it into a node and push it onto the operator stack
			// Is the top of the stack a closing parenthesis ')'? Turn it into a node and and push it onto the operator stack
			// Is the top of the stack the same or lower priority than this item? If so, make it a node and push it onto the operator stack
			// Anything else and we'll pop the top off the operator stack and push it onto the treeNode stack
		}
		if (temp == '(')	// Is it an opening parenthesis '('? 
			cout << "That was an opening parenthesis" << endl; // Then we've got to pop operators from the operator stack until we find a closing parenthesis ')'
	}

	while (!operators.empty()) {
		// The input stack should now be empty so let's unstack the unused operators from the operator stack
	}

	pressToContinue();
	return root;
};