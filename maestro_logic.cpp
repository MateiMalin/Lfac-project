#include "maestro_logic.h"
#include <cstdlib>

Value::Value() : type("silenzio"), iVal(0), fVal(0.0), bVal(false) {}

Symbol::Symbol(std::string _name, std::string _type, std::string _kind) : name(_name), type(_type), kind(_kind) {
    if (type == "basso") { value.type = "basso"; value.iVal = 0; }
    else if (type == "soprano") { value.type = "soprano"; value.fVal = 0.0; }
    else if (type == "verita") { value.type = "verita"; value.bVal = false; }
}

SymbolTable::SymbolTable(std::string _scope, SymbolTable* _parent) : scope(_scope), parent(_parent) {}

bool SymbolTable::addSymbol(std::string _name, std::string _type, std::string _kind) {
    if (symbols.find(_name) != symbols.end()) return false;
    symbols.insert({ _name, Symbol(_name, _type, _kind) });
    if (_kind == "method" || _kind == "function") lastAddedFunction = &symbols.at(_name);
    return true;
}

void SymbolTable::addParamToLastFunction(std::string paramType) {
    if (lastAddedFunction != nullptr) lastAddedFunction->paramTypes.push_back(paramType);
    else if (parent != nullptr) parent->addParamToLastFunction(paramType);
}

Symbol* SymbolTable::findSymbol(std::string name) {
    if (symbols.find(name) != symbols.end()) return &symbols.at(name);
    if (parent != nullptr) return parent->findSymbol(name);
    return nullptr;
}

void SymbolTable::printTable(std::ofstream& fout) {
    fout << "Symbol Table: " << scope << "\n____________________________________________\n";
    for (auto const& [key, val] : symbols) fout << val.name << "\t" << val.type << "\t" << val.kind << "\n";
    fout << "\n";
}

ASTNode::ASTNode(NodeType t) : nodeType(t), left(nullptr), right(nullptr) {}

Value ASTNode::evaluate(SymbolTable* currentScope) {
    Value res;
    switch (nodeType) {
    case NODE_LITERAL: return literalValue;
    case NODE_ID: {
        Symbol* symbol = currentScope->findSymbol(idName);
        if (!symbol) { std::cerr << "Semantic Error: " << idName << " undefined\n"; exit(1); }
        return symbol->value;
    }
    case NODE_ASSIGN: {
        Value val = right->evaluate(currentScope);
        Symbol* symbol = currentScope->findSymbol(idName);
        if (!symbol) { std::cerr << "Semantic Error: " << idName << " undefined\n"; exit(1); }
        if (symbol->type != val.type) { std::cerr << "Type Error: mismatch in assignment\n"; exit(1); }
        symbol->value = val;
        return val;
    }
    case NODE_PRINT: {
        Value val = left->evaluate(currentScope);
        if (val.type == "basso") std::cout << val.iVal << "\n";
        else if (val.type == "soprano") std::cout << val.fVal << "\n";
        else if (val.type == "libretto") std::cout << val.sVal << "\n";
        else if (val.type == "verita") std::cout << (val.bVal ? "vero" : "falso") << "\n";
        return val;
    }
    case NODE_OP: {
        Value l = left->evaluate(currentScope);
        Value r = right->evaluate(currentScope);
        if (l.type != r.type) { std::cerr << "Semantic Error: Operands must match type\n"; exit(1); }
        res.type = l.type;
        if (op == "+") {
            if (l.type == "basso") res.iVal = l.iVal + r.iVal;
            else res.fVal = l.fVal + r.fVal;
        }
        else if (op == "-") {
            if (l.type == "basso") res.iVal = l.iVal - r.iVal;
            else res.fVal = l.fVal - r.fVal;
        }
        else if (op == "*") {
            if (l.type == "basso") res.iVal = l.iVal * r.iVal;
            else res.fVal = l.fVal * r.fVal;
        }
        return res;
    }
    case NODE_OTHER: {
        Symbol* function = currentScope->findSymbol(idName);
        if (!function) { std::cerr << "Semantic Error: Function " << idName << " undefined\n"; exit(1); }
        if (function->paramTypes.size() != arguments.size()) { std::cerr << "Semantic Error: Arg count mismatch\n"; exit(1); }
        for (size_t i = 0; i < arguments.size(); i++) {
            Value argVal = arguments[i]->evaluate(currentScope);
            if (function->paramTypes[i] != argVal.type) { std::cerr << "Semantic Error: Arg type mismatch\n"; exit(1); }
        }
        res.type = function->type;
        return res;
    }
    }
    return res;
}