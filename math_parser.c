/**************************************
 * Math Expression Parser
 * I use binary tree for that.
 * Take token on mathstr.. and build
 * the tree with it.
 * Token is a digit or a operator
 *
 **************************************
 * eval_str_to_bt() is the entry point.
 *
 * He's the only one function how the
 * user can invoke.
 *
 * Other function and structure are
 * internal
 **************************************/

#include <stdlib.h> // atoi, free
#include <ctype.h> // isdigit
#include <string.h> // strcpy, strlen
#include <stdbool.h> // bool
#include <math.h> // pow

#include "math_parser.h"

/*---- Define elemntary element do i need ----*/
/**********************************************/

typedef union { // Token can be operator or digit
    char    op_value;
    double  elem_value;
} u_elem;

typedef struct s_node { // basic node
    bool            op; // Because without we can't know what's the elem value.
    u_elem          elem;
    struct s_node   *leftChild;
    struct s_node   *rightChild;
} t_tree;

typedef struct s_tokenstr { // basic token
    bool    op;
    u_elem  elem;
} t_token;


/*---- Global variable ----*/
/***************************/
static char    *mathstr; // mathematic expression
static t_token *token; // result of getNextToken

/*---- Prototype Function ----*/
/******************************/

// ---- string parser ----

// return the next token (int and op) of mathstr
static void getNextToken();

// ---- tree ----

// create new node
static t_tree*  build_node(u_elem elem, bool op, t_tree *leftchild, t_tree *rightchild);
// delete the tree
static void     deletetree(t_tree **head);
// eval the tree
static double   evaltree(t_tree *head);

// ---- build tree ----

/********************************************
 * For build, I use some recursive function.
 * Function are sort by operator precedence.
 ********************************************/

// first entrance -- operator +
static t_tree* first_to_bt();
// -> first invok second
// second entrance -- operator -
static t_tree* second_to_bt();
// -> second invok third
// third entrance -- operator *
static t_tree* third_to_bt();
// -> four invok five
// four entrance -- operator /
static t_tree* four_to_bt();
// -> four invok five
// five entrance -- operator ^
static t_tree* five_to_bt();
// -> five invok six
// last entrance -- digit, ( ) and ~
static t_tree* six_to_bt();


/*---- Declaration Function ----*/
/********************************/

/*---- string tools ----*/
// get next token on mathstr
// global variable token is set by this function
static void getNextToken()
{
    static int pos = 0; // current pos on str
    char *newpos;

    token->op = true;
    token->elem.op_value = mathstr[pos];

    if (token->elem.op_value  == '\0') { // Si fin de chaine
        return;
    }

    if (!isdigit(token->elem.op_value)) { // Si n'est pas un nombre

        if(token->elem.op_value  == '-' && pos-1 >= 0 && !isdigit(mathstr[pos-1])) { // check if "not .."
            token->elem.op_value  = '~';    // change value - to ~ (specify not)
        }

        pos++;

        return;
    }

    token->op = false;
    token->elem.elem_value = strtod(&mathstr[pos],&newpos); // take the double

    pos += (newpos - &mathstr[pos]); // place position to next token

}

/*---- Tree function ----*/
// build new node
static t_tree* build_node(u_elem elem, bool op, t_tree *leftchild, t_tree *rightchild)
{
    t_tree *newtree;

    newtree = (t_tree*)malloc(sizeof(t_tree));

    if (newtree == NULL) {
        return newtree;
    }

    if(op) {
        newtree->elem.op_value = elem.op_value;
    } else {
        newtree->elem.elem_value = elem.elem_value;
    }

    newtree->op = op;
    newtree->leftChild = leftchild;
    newtree->rightChild = rightchild;

    return newtree;
}

// calcul the result of tree
static double evaltree(t_tree *head)
{
    if (head == NULL) {
        return 0;
    }

    if (!head->op) { // if tree head is number
        return head->elem.elem_value;
    }

    switch (head->elem.op_value) {
    case '~': // inverse value
        return evaltree(head->leftChild) * -1;
        break;

    case '-':
        return evaltree(head->leftChild) - evaltree(head->rightChild);
        break;

    case '+':
        return evaltree(head->leftChild) + evaltree(head->rightChild);
        break;

    case '/':
        return evaltree(head->leftChild) / evaltree(head->rightChild);
        break;

    case '*':
        return evaltree(head->leftChild) * evaltree(head->rightChild);
        break;

    case '^': // power
        return pow(evaltree(head->leftChild),evaltree(head->rightChild));
        break;
    }

    return 0;
}

// delete the tree -- left / right / here
static void deletetree(t_tree **head)
{
    if (*head == NULL) {
        return;
    }

    deletetree(&((*head)->leftChild)); // delete left child
    deletetree(&((*head)->rightChild)); // delete right child
    free(*head); // delete node
}

/*---- Build Tree Function ----*/

// first entrance
// start the tree build -- lower priority operator
static t_tree* first_to_bt()
{
    t_tree *newtree;

    newtree = second_to_bt();

    if (token->op && token->elem.op_value == '+') { // If operator, continu to build
        u_elem elem;
        elem.op_value = token->elem.op_value;

        getNextToken();

        return build_node(elem, true, newtree, first_to_bt());

    } else {
        return newtree;
    }

}

// second entrance
// better priority than first_to_bt
static t_tree* second_to_bt()
{
    t_tree *newtree;

    newtree = third_to_bt();

    if (token->op && token->elem.op_value == '-') { // If operator, continu to build
        u_elem elem;
        elem.op_value = token->elem.op_value;

        getNextToken();

        return build_node(elem, true, newtree, second_to_bt());

    } else {
        return newtree;
    }

}

// third entrance
// better priority than second_to_bt
static t_tree* third_to_bt()
{
    t_tree *newtree;

    newtree = four_to_bt();

    if (token->op && token->elem.op_value == '*') { // If operator, continu to build
        u_elem elem;
        elem.op_value = token->elem.op_value;

        getNextToken();

        return build_node(elem, true, newtree, third_to_bt());

    } else {
        return newtree;
    }

}

// four entrance
// better priority than third_to_bt
static t_tree* four_to_bt()
{
    t_tree *newtree;

    newtree = five_to_bt();

    if (token->op && token->elem.op_value == '/') { // If operator, continu to build
        u_elem elem;
        elem.op_value = token->elem.op_value;

        getNextToken();

        return build_node(elem, true, newtree, four_to_bt());

    } else {
        return newtree;
    }

}

// five entrance
// better priority than four_to_bt
static t_tree* five_to_bt()
{
    t_tree *newtree;

    newtree = six_to_bt();

    if (token->op && token->elem.op_value == '^') { // If operator, continu to build
        u_elem elem;
        elem.op_value = token->elem.op_value;

        getNextToken();

        return build_node(elem, true, newtree, six_to_bt());

    } else {
        return newtree;
    }

}

// last entrance
// higher priority --
//                  build a leaf if digit
//                  manage parenthesis and not
static t_tree* six_to_bt()
{

    if (!token->op) { // if is not operator, it's a leaf
        u_elem elem;
        elem.elem_value = token->elem.elem_value;

        getNextToken();
        return build_node(elem, false, NULL, NULL);
    }


    switch(token->elem.op_value) {

    case '~': { // if is "not", build new tree in the left child
        // obligation car déclaration de variable
        u_elem elem;
        elem.op_value = token->elem.op_value;

        getNextToken();

        return build_node(elem,true, six_to_bt(), NULL);

        break;
    }

    case '(': // start of ( )

        getNextToken();

        t_tree *newtree = first_to_bt();

        if(token->op && token->elem.op_value == ')') { // if end of ( )
            getNextToken();

            return newtree;
        }
    }

    return NULL;
}


/**** ENTRY POINT !!!! ****/

// call this on main...
// set mathstr
// set token
// build the tree
// calcul the tree
// give the result on *result
// function return 0 if ok :
// function error :: -1 mathexpr or result null
//                   -2 internal malloc error
//                   -3 invalid mathexpr
int eval_str_to_bt(const char *mathexpr, double *result)
{
    t_tree *head;

    if (mathexpr == NULL || result == NULL) {
        return -1;
    }

    mathstr = malloc((strlen(mathexpr) + 1)*sizeof(*mathstr));
    if (mathstr == NULL) {
        return -2;
    }

    token = malloc(sizeof(*token));
    if (token == NULL) {
        free(mathstr);
        return -2;
    }

    strcpy(mathstr, mathexpr); // recopie dans global

    getNextToken(); // first iteration on mathstr -- absolutely needed

    head = first_to_bt(); // build tree

    if(!token->op || token->elem.op_value != '\0') { // check getNextToken parse
        free(mathstr);                              // the all str
        free(token);

        deletetree(&head);

        return -3;
    }


    *result = evaltree(head); // resolve the tree

    // Free tree & malloc
    free(mathstr);
    free(token);

    deletetree(&head);

    return 0;
}

