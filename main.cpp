#include "bib/AnalyticTableaux.cpp"
#include <bits/stdc++.h>

using namespace std;

typedef struct {
    int num;
    char op;
}message;

message checkOperator(string exp);
vector<Node*> applyRule(Node* node);
int checkSplit(string str2, int val2);
void checkContradictions(vector<Node*> insertedNodes);

int main() {

    int truth;
    char var;
    string tab;

    tab = "(X & Y)";
    truth = 1;

    Node tableau = Node(tab, truth);

    vector<Node*> appliableNodes = tableau.getAppliableNodes();
    while(!tableau.isClosed() && !appliableNodes.empty()) {

        for(int i = 0; i < appliableNodes.size(); i++) {
            vector<Node*> insertedNodes = applyRule(appliableNodes[i]);
            checkContradictions(insertedNodes);
        }

        appliableNodes = tableau.getAppliableNodes();
    }

    if(tableau.isClosed())
        cout << "Tableau fechado." << endl;

    tableau.printTree();

    return 0;
}

vector<Node*> applyRule(Node* node) {
    vector<Node*> insertedNodes;

    char operador;
    message msg;
    string left, right, aux1;
    string aux = node->getExpression();

    msg = checkOperator(aux);

    if(msg.op == '~'){
        aux1 = aux.substr(2 ,aux.size()-1);
    }
    else{
        left = aux.substr(1, msg.num-2);
        right = aux.substr(msg.num+2);
        right.pop_back();
    }

    if(operador == '>' && node->getTruthValue() == 0){
        insertedNodes = node->insertFront(left, 1, right , 0);
    }

    else if(operador == '>' && node->getTruthValue() == 1) {
        int choice = checkSplit(right, 1);
        if(choice == 1)
            insertedNodes = node->insertSides(left, 0, right, 1);
        else
            insertedNodes = node->insertSides(right, 1, left, 0);
    }

    else if(operador == '~' && node->getTruthValue() == 0) {
        insertedNodes = node->insertFront(aux1, 1);
    }

    else if(operador == '~' && node->getTruthValue() == 1) {
        insertedNodes = node->insertFront(aux1, 0);
    }

    else if(operador == '&' && node->getTruthValue() == 0){
        int choice = checkSplit(right, 0);
        if(choice == 1)
            insertedNodes = node->insertSides(left, 0, right, 0);
        else
            insertedNodes = node->insertSides(right, 0, left, 0);
    }

    else if(operador == '&' && node->getTruthValue() == 1){
        insertedNodes = node->insertFront(left, 1, right, 1);
    }

    else if(operador == 'v' && node->getTruthValue() == 0){
        insertedNodes = node->insertFront(left, 0, right, 0);
    }

    else if(operador == 'v' && node->getTruthValue() == 1){
        int choice = checkSplit(right, 1);
        if(choice == 1)
            insertedNodes = node->insertSides(left, 1, right, 1);
        else
            insertedNodes = node->insertSides(right, 1, left, 1);
    }

    node->markApplied();

    return insertedNodes;
}

void checkContradictions(vector<Node*> insertedNodes) {
    for (int i = 0; i < insertedNodes.size(); i++) {
        if(insertedNodes[i]->checkContradiction())
            insertedNodes[i]->markContradiction();
    }
}

message checkOperator(string exp){
    message msg;
    int left=0, right=0;

    for(int i=0; i<exp.size(); i++){
        if(exp[i] == '(')
            left++;
        else if(exp[i]==')')
            right++;

        if(left == 1 && right == 0 ){
            if(exp[i+1]=='~'){
                msg.num  = 1;
                msg.op = '~';
                break;
            }
        }
        if(left == right+1){
            if(exp[i+1] != '(' && exp[i+1] != ' '){
                msg.num = i+3;
                msg.op = exp[i+3];
                break;
            }
        }
    }
    return msg;
}

int checkSplit(string str2, int val2){
    char op2;
    message msg;

    msg =checkOperator(str2);
    op2 = msg.op;

    if(op2 == '~' || (op2 == '&' && val2 == 1) || (op2 == 'v' && val2 == 0) || (op2 == '>' && val2 == 0 )) {
        return 2;
    }

    return 1;

}