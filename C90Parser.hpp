
#pragma once

class C90Parser {
public: 
    bool primaryExpression();
    bool postfixExpression();
    bool argumentExpressionList();
    bool unaryExpression();
    bool unaryOperator();
    bool castExpression();
    bool multiplicativeExpression();
    bool additiveExpression();
    bool shiftExpression();
    bool relationalExpression();
    bool equalityExpression();
    bool bitAndExpression();
    bool bitXorExpression();
    bool bitIorExpression();
    bool logicalAndExpression();
    bool logicalOrExpression();
    bool conditionalExpression();
    bool assignmentExpression();
    bool expression();
    bool constantExpression();

    bool declaration();
    bool declarationSpecifiers();
    bool initDeclaratorList();
    bool initDeclarator();
    bool storageClassSpecifier();
    bool typeSpecifier();
    bool structOrUnionSpecifier();
    bool structDeclarationList();
    bool structDeclaration();
    bool specifierQualifierList();
    bool structDeclaratorList();
    bool structDeclarator();
    bool enumSpecifier();
    bool enumeratorList();
    bool enumerator();
    bool typeQualifier();
    bool functionSpecifier();

};