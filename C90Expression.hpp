// C90Expression.hpp
//
// Author: Marco Jacques
//
// Expression parsing for C90 
//

#pragma once

#include <memory>
#include "IR.hpp"
#include "Lexer.hpp"
#include "TypeParser.hpp"

class C90Expression {
protected:
    std::shared_ptr<Lexer> lexer;
    std::shared_ptr<TypeParser> typeParser;
    
public: 
    std::shared_ptr<IRExpr> primaryExpression();
    std::shared_ptr<IRExpr> postfixExpression();
    std::vector<IRExprPtr> argumentExpressionList();
    std::shared_ptr<IRExpr> unaryExpression();
    std::shared_ptr<IRExpr> unaryOperator();
    std::shared_ptr<IRExpr> castExpression();
    std::shared_ptr<IRExpr> multiplicativeExpression();
    std::shared_ptr<IRExpr> additiveExpression();
    std::shared_ptr<IRExpr> shiftExpression();
    std::shared_ptr<IRExpr> relationalExpression();
    std::shared_ptr<IRExpr> equalityExpression();
    std::shared_ptr<IRExpr> bitAndExpression();
    std::shared_ptr<IRExpr> bitXorExpression();
    std::shared_ptr<IRExpr> bitIorExpression();
    std::shared_ptr<IRExpr> logicalAndExpression();
    std::shared_ptr<IRExpr> logicalOrExpression();
    std::shared_ptr<IRExpr> conditionalExpression();
    std::shared_ptr<IRExpr> assignmentExpression();
    std::shared_ptr<IRExpr> expression();
    std::shared_ptr<IRExpr> constantExpression();

};