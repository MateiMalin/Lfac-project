#ifndef MAESTRO_LOGIC_H
#define MAESTRO_LOGIC_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>

class Value {
public:
    std::string type;
    int iVal;
    float fVal;
    std::string sVal;
    bool bVal;
    Value(); // Constructor
};

class SymbolTable;

class Symbol {
public:
    std::string name, type, kind;
    std::vector<std::string> paramTypes;
    Value value;
    Symbol(std::string _name, std::string _type, std::string _kind); // Constructor
};

class SymbolTable {
public:
    std::string scope;
    SymbolTable* parent;
    std::map<std::string, Symbol> symbols;
    Symbol* lastAddedFunction = nullptr;

    SymbolTable(std::string _scope, SymbolTable* _parent = nullptr);
    bool addSymbol(std::string _name, std::string _type, std::string _kind); //
    void addParamToLastFunction(std::string paramType); //
    Symbol* findSymbol(std::string name); //
    void printTable(std::ofstream& fout); //
};

enum NodeType { NODE_LITERAL, NODE_ID, NODE_OP, NODE_ASSIGN, NODE_PRINT, NODE_OTHER };

class ASTNode {
public:
    NodeType nodeType;
    std::string op;
    Value literalValue;
    std::string idName;
    ASTNode* left, * right;
    std::vector<ASTNode*> arguments;

    ASTNode(NodeType t);
    Value evaluate(SymbolTable* scope); //
};

#endif