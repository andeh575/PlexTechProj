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

string getInfix(void);				// Helper function to get initial infix expression
void pressToContinue();				// Helper function for console development
bool isOperator(char m);			// Helper function for identifying mathematical operators	
int priority(char m);				// Helper function to specify order of operations
struct node* createNode(char m);	// Helper function to create a new node
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
	pressToContinue();
	return 0;
}

// Helper function to get initial expression - should be infix notation (We don't currently test)
string getInfix(void) {

	string infix;

	cout << "Please enter an equation in infix format (Acceptable Operators: + and *):" << endl;
	cin >> infix;

	return(infix);
}

// Helper function for console development
void pressToContinue(void) {
	cout << endl << "Press ENTER to continue..." << flush;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper function for identifying operators
bool isOperator(char val) {
	if (val == '+' || val == '*') // Current implementation only supports addition and multiplication
		return true;
	else
		return false;
}

// Helper function to specify order of operations
int priority(char val) {
	int p = 0;

	if (val == '+')
		p = 1;
	else if (val == '*')
		p = 2;

	return p;
}

// Helper function to create a new node*
struct node* createNode(char data) {
	struct node* subNode = new node;

	subNode->val = data;
	subNode->left = NULL;
	subNode->right = NULL;

	return subNode;
}

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

		if (isdigit(temp)) {	// Is it an operand? 
			cout << "That was a digit - Push onto treeNodes stack" << endl; 
			treeNodes.push(createNode(temp));	// Turn temp into a node and push it onto the treeNodes stack
		}
		if (temp == ')') {	// Is it a closing parenthesis ')'?
			cout << "That was a closing parenthesis - Push onto operators stack" << endl; 
			operators.push(createNode(temp));	// Turn temp into a node and push it onto the operators stack
		}
		if (isOperator(temp)) {	// Is it an operator? Then it depends on...
			cout << "That was a " << temp << " operator... ";

			if (operators.empty()) {	// Is the operator stack empty?
				cout << "The operator stack was empty - Push temp onto operators stack" << endl; 
				operators.push(createNode(temp));	// Turn temp into a node and push it onto the operators stack
			}
			else if (operators.top()->val == ')') {	// Is the top of the stack a closing parenthesis ')'? 
				cout << "Top of the operator stack was closing parenthesis ')'" << endl; 
				operators.push(createNode(temp));	// Turn temp into a node and and push it onto the operator stack
			}
			else if (priority(operators.top()->val) <= priority(temp)) {	// Is the top of the stack the same or lower priority than this item? 
				cout << "Priority of operator stack top <= temp" << endl; 
				operators.push(createNode(temp));	// Turn temp into a node and push it onto the operator stack
			}
			else {	// Anything else - ie: the priority of the top of the operators stack was higher priority
				cout << "We encountered something else - ie: the priority of .top() was higher than temp" << endl; 
				// we'll pop the top off the operator stack and insert it into the tree
			}
		}

		if (temp == '(') {	// Is it an opening parenthesis '('? 
			cout << "That was an opening parenthesis" << endl;
			while (operators.top()->val != ')') {	// Pop operators from the operator stack until we find a closing parenthesis ')'
				// Insert operators into the tree
			}

			operators.pop(); // Discard the connecting ')'
		}
	}

	while (!operators.empty()) {	// The input stack should now be empty
		cout << "Emptying the operator stack here" << endl; 
		// Unstack the waiting operators from the operator stack and insert them into the tree
	}

	cout << endl << "All parsed - sending root back to caller" << endl;
	return root;
}