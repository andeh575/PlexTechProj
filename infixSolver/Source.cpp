/*	Author:		Andrew Graham
/	Date:		20 August 2015
/	Purpose:	Solves simple mathematetical expressions - program accepts expressions in infix notation, converts them to postfix, 
/				and assembles them into a binary expression tree for solving. 
/
/	Requirements/Assumptions:	
/		- Input expressions are in infix notation
/		- Input operands are single-digit
/		- Input operations are addition: '+', and multiplication: '*'
*/

#include <iostream>
#include <string>
#include <stack>
#include "ExpressionTree.h"
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
void insertOp(stack<node*>& operators, stack<node*>& treeNodes);	// Build a tree node with an operator and two children
struct node* parseExpression(string m, struct node* root);			// Parse the input stack for insertion into the expression tree
int evaluate(struct node* node);									// Recursively calculate the value of the expression tree

void preOrder(struct node* node);	// Test function: Preorder Traversal - Recursively defined
void inOrder(struct node* node);	// Test function: Inorder Traversal - Recursively defined
void postOrder(struct node* node);	// Test Function: Postorder Traversal - Recursively defined

// Helper function to get initial expression - should be infix notation (We don't currently test)
string getInfix(void) {

	string infix;

	cout << "Please enter an equation in infix format (Acceptable Operators: + and *):" << endl;
	cin >> infix;

	return(infix);
}

// Helper function for console development
void pressToContinue(void) {
	system("PAUSE");
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

/*	Build a tree node with an operator and two children - Push onto treeNodes stack.
/	Valid children are: 
/		- Two Operands (ie: 1 and 2) - Can be leafs
/		- Two Operators (ie: + and *) - Need to have subtrees of their own
/		- An Operator and an Operand (ie: 1 and +) - Operand is a leaf and Operators have subtrees of their own
*/
void insertOp(stack<node*>& operators, stack<node*>& treeNodes) {
	node* opNode = operators.top();	// Take the first operator from the operators stack
	operators.pop();

	// Take first two items off of the treeNodes stack and make them the opNode's children - left precedence
	opNode->left = treeNodes.top();
	treeNodes.pop();
	opNode->right = treeNodes.top();
	treeNodes.pop();

	// Push the newly created tree on top of the treeNodes stack
	treeNodes.push(opNode);
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
			treeNodes.push(createNode(temp));	// Turn temp into a node and push it onto the treeNodes stack
		}
		if (temp == ')') {	// Is it a closing parenthesis ')'?
			operators.push(createNode(temp));	// Turn temp into a node and push it onto the operators stack
		}
		if (isOperator(temp)) {	// Is it an operator? Then it depends on...
			bool control = false;	// Prevents operators from getting lost in the event that the top of the operator stack has a higher priority than temp

			while (!control) {
				if (operators.empty()) {	// Is the operator stack empty?
					operators.push(createNode(temp));	// Turn temp into a node and push it onto the operators stack
					control = true;
				}
				else if (operators.top()->val == ')') {	// Is the top of the stack a closing parenthesis ')'? 
					operators.push(createNode(temp));	// Turn temp into a node and and push it onto the operator stack
					control = true;
				}
				else if (priority(operators.top()->val) <= priority(temp)) {	// Is the top of the stack the same or lower priority than this item? 
					operators.push(createNode(temp));	// Turn temp into a node and push it onto the operator stack
					control = true;
				}
				else {	// Anything else - ie: the priority of the top of the operators stack was higher priority
					insertOp(operators, treeNodes);	// we'll pop the top off the operator stack and insert it into the tree - don't forget about temp! It wasn't pushed!
				}
			}
		}

		if (temp == '(') {	// Is it an opening parenthesis '('? 
			while (operators.top()->val != ')') {	// Pop operators from the operator stack until we find a closing parenthesis ')'
				insertOp(operators, treeNodes);	// Insert operators into the tree
			}

			operators.pop(); // Discard the connecting ')'
		}
	}

	while (!operators.empty()) {	// The input stack should now be empty
		insertOp(operators, treeNodes);	// Unstack the waiting operators from the operator stack and insert them into the tree
	}

	// Assign the top of the treeNodes stack to the root and return to the caller
	root = treeNodes.top();

	return root;
}

// Recursively calculate the value of the expression tree
int evaluate(struct node* node) {
	int x, y, z;

	// The node contains an operator
	if (node->val == '+' || node->val == '*') {
		x = evaluate(node->left);
		y = evaluate(node->right);

		if (node->val == '+')
			z = x + y;
		else if (node->val == '*')
			z = x * y;
		
		return z;
	}
	// The node contains an operand
	else
		return(node->val - '0'); 
}

/*	Test Function: Preorder Traversal - Recursively Defined - Should print prefix notation
/	Reminder -- Preorder traversal: Root -> Left Child -> Right Child
*/
void preOrder(struct node* node) {
	if (node != NULL) {
		cout << node->val << " ";
		preOrder(node->left);
		preOrder(node->right);
	}
}

/*	Test Function: Inorder Traversal - Recursively Defined - Should print infix notation
/	Reminder -- Inorder traversal: Left Child -> Root -> Right Child
*/
void inOrder(struct node* node) {
	if (node != NULL) {
		inOrder(node->left);
		cout << node->val << " ";
		inOrder(node->right);
	}
}

/*	Test Function: Postorder Traversal - Recursively Defined - Should print postfix notation
/	Reminder -- Postorder traversal: Left Child -> Right Child -> Root
*/
void postOrder(struct node* node) {
	if (node != NULL) {
		postOrder(node->left);
		postOrder(node->right);
		cout << node->val << " ";
	}
}

int main(void) {
	string infix;	// Input string
	int solution;	// Solution to the expression tree
	struct node* root = new node;	// The root of the expression tree

	cout << "Welcome to a Simple Infix Equation Solver" << endl;

	// Parsing the input stack for insertion onto the expression tree
	infix = getInfix();
	root = parseExpression(infix, root);

	// Test for proper tree structure and printing
	/*
	cout << endl << "Preorder Traversal: ";
	preOrder(root);

	cout << endl << "Inorder Traversal: ";
	inOrder(root);

	cout << endl << "Postorder Traversal: ";
	postOrder(root);
	*/

	// Solve the equation
	solution = evaluate(root);
	cout << endl << "The solution to the expression is: " << solution << endl;

	pressToContinue();
	return 0;
}