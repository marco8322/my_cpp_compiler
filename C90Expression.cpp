// C90Expression.cpp
//
// Expression parsing for C90 
//

#include "C90Expression.hpp"

/**

    primary-expression:
        identifier
        constant
        string-literal
        ( expression )

 */
IRExprPtr C90Expression::primaryExpression()
{
    LexerTokenPtr nextToken = lexer->nextToken();
    switch(nextToken->getKind()) {
        case LexerToken::IDENTIFIER:
            return IRFactory::createIdExpr(nextToken);

        case LexerToken::INTEGER_LITERAL:
            return IRFactory::createIntLitExpr(nextToken);

        case LexerToken::STRING_LITERAL:
            return IRFactory::createStringLitExpr(nextToken);

        case LexerToken::LEFT_PARAR: {
            IRExprPtr subExpr = expression();
            lexer->acceptToken(LexerToken::RIGHT_PARAR);
            return subExpr;
        }

        default: 
            return nullptr;
            // nothing to do yet...
    }

    // Error
    //
    return nullptr;
}

/*

          postfix-expression:
                  primary-expression
                  postfix-expression [  expression ] 
                  postfix-expression (  argument-expression-list<opt> ) 
                  postfix-expression .   identifier
                  postfix-expression ->  identifier
                  postfix-expression ++ 
                  postfix-expression --


*/
IRExprPtr C90Expression::postfixExpression()
{
    IRExprPtr currExpr = primaryExpression();

    for(;;) {
        switch(lexer->peekToken()->getKind()) {
            case LexerToken::LEFT_BRACKET: {
                lexer->acceptToken(LexerToken::LEFT_BRACKET);
                IRExprPtr indexExpr = expression();
                lexer->acceptToken(LexerToken::RIGHT_BRACKET);
                currExpr = IRFactory::createArraySubscripting(currExpr, indexExpr);
                break;
            }

            case LexerToken::LEFT_PARAR: {
                lexer->acceptToken(LexerToken::LEFT_PARAR);
                std::vector<IRExprPtr> args;
                
                if( lexer->peekToken()->getKind() != LexerToken::RIGHT_PARAR ) {
                    args = argumentExpressionList();
                }

                lexer->acceptToken(LexerToken::RIGHT_PARAR);
                currExpr = IRFactory::createCallExpr(currExpr, args);
                break;
            }

            case LexerToken::DOT: {
                lexer->acceptToken(LexerToken::DOT);
                LexerTokenPtr fieldId = lexer->acceptToken(LexerToken::IDENTIFIER);
                currExpr = IRFactory::createStructFieldDirectAccess(currExpr, fieldId);
                break;
            }

            case LexerToken::LEFT_ARROW: {
                lexer->acceptToken(LexerToken::LEFT_ARROW);
                LexerTokenPtr ptrFieldId = lexer->acceptToken(LexerToken::IDENTIFIER);
                currExpr = IRFactory::createStructFieldIndirectAccess(currExpr, ptrFieldId);
                break;
            }

            case LexerToken::INCR: {
                lexer->acceptToken(LexerToken::INCR);
                currExpr = IRFactory::createPostIncrExpr(currExpr);
                break;
            }

            case LexerToken::DECR: {
                lexer->acceptToken(LexerToken::DECR);
                currExpr = IRFactory::createPostDecrExpr(currExpr);
                break;
            }

            default:
                goto exitLoop;
        }
    }

    exitLoop:
        return currExpr;
}

/*

          argument-expression-list:
                  assignment-expression
                  argument-expression-list ,  assignment-expression

 */
std::vector<IRExprPtr> C90Expression::argumentExpressionList()
{
    std::vector<IRExprPtr> args;

    args.push_back(assignmentExpression());
    while( lexer->peekToken()->getKind() == LexerToken::COMMA ) {
        args.push_back(assignmentExpression());
    }

    return args;
}

/*
          unary-expression:
                  postfix-expression
                  ++  unary-expression
                  --  unary-expression
                  unary-operator cast-expression
                  sizeof  unary-expression
                  sizeof (  type-name )

          unary-operator: one of
                  &  *  +  -  ~  !

*/
IRExprPtr C90Expression::unaryExpression()
{
    IRExprPtr unaryExpr;
    switch( lexer->peekToken()->getKind()) {
        case LexerToken::INCR:
            lexer->acceptToken(LexerToken::INCR);
            return IRFactory::createPreIncrExpr(unaryExpression());

        case LexerToken::DECR:
            lexer->acceptToken(LexerToken::DECR);
            return IRFactory::createPreDecrExpr(unaryExpression());

        case LexerToken::BIT_AND:
            lexer->acceptToken(LexerToken::BIT_AND);
            return IRFactory::createAddressOfExpr(castExpression());

        case LexerToken::MUL:
            lexer->acceptToken(LexerToken::MUL);
            return IRFactory::createDereferenceExpr(castExpression());

        case LexerToken::ADD:
            lexer->acceptToken(LexerToken::ADD);
            return IRFactory::createUnaryPlusExpr(castExpression());

        case LexerToken::SUB:
            lexer->acceptToken(LexerToken::SUB);
            return IRFactory::createUnaryMinusExpr(castExpression());

        case LexerToken::BIT_NOT:
            lexer->acceptToken(LexerToken::BIT_NOT);
            return IRFactory::createBitNotExpr(castExpression());

        case LexerToken::BOOL_NOT:
            lexer->acceptToken(LexerToken::BOOL_NOT);
            return IRFactory::createBoolNotExpr(castExpression());

        case LexerToken::SIZEOF: {
            // Might need to replace by
            //    sizeof ( expression )
            //    sizeof ( type-name )
            //    sizeof unaryExpression
            //
            // to complete...
            //
            lexer->acceptToken(LexerToken::SIZEOF);
            if( lexer->peekToken()->getKind() == LexerToken::LEFT_PARAR ) {
                lexer->acceptToken(LexerToken::LEFT_PARAR);
                IRTypePtr typeName = typeParser->typeName();
                IRExprPtr result;

                if( typeName ) {
                    result = IRFactory::createSizeofTypeExpr(typeName);
                } 
                else {
                    result = IRFactory::createSizeofExpr(expression());
                }

                lexer->acceptToken(LexerToken::RIGHT_PARAR);
                return result;
            }
            else {
                return IRFactory::createSizeofExpr(unaryExpression());
            }
        }

        default:
            return postfixExpression();
    }
}

/*

          cast-expression:
                  unary-expression
                  ( type-name )  cast-expression

*/
IRExprPtr C90Expression::castExpression()
{
    if( lexer->peekToken()->getKind() == LexerToken::LEFT_PARAR ) {
        lexer->acceptToken(LexerToken::LEFT_PARAR);
        IRTypePtr typeName = typeParser->typeName();
        if( typeName ) {
            lexer->acceptToken(LexerToken::RIGHT_PARAR);
            return IRFactory::createCastExpr(typeName, castExpression());
        }
        else {
            IRExprPtr result = expression();
            lexer->acceptToken(LexerToken::RIGHT_PARAR);
            return result;
        }
    }
    else {
        return unaryExpression();
    }
}

/*

        multiplicative-expression:
                  cast-expression
                  multiplicative-expression *  cast-expression
                  multiplicative-expression /  cast-expression
                  multiplicative-expression %  cast-expression

*/
IRExprPtr C90Expression::multiplicativeExpression()
{
    IRExprPtr currExpr = castExpression();
    for(;;) {
        switch( lexer->peekToken()->getKind() ) {
            case LexerToken::MUL:
                lexer->acceptToken(LexerToken::MUL);
                currExpr = IRFactory::createMulExpr(currExpr, castExpression());
                break;

            case LexerToken::DIV:
                lexer->acceptToken(LexerToken::DIV);
                currExpr = IRFactory::createDivExpr(currExpr, castExpression());
                break;

            case LexerToken::MOD:
                lexer->acceptToken(LexerToken::MOD);
                currExpr = IRFactory::createModExpr(currExpr, castExpression());
                break;

            default:
                goto exitLoop;
        }
    }

exitLoop:
    return currExpr;
}

/*

          additive-expression:
                  multiplicative-expression
                  additive-expression +  multiplicative-expression
                  additive-expression -  multiplicative-expression

*/
IRExprPtr C90Expression::additiveExpression()
{
    IRExprPtr currExpr = multiplicativeExpression();
    for(;;) {
        switch( lexer->peekToken()->getKind() ) {
            case LexerToken::ADD:
                lexer->acceptToken(LexerToken::ADD);
                currExpr = IRFactory::createAddExpr(currExpr, multiplicativeExpression());
                break;

            case LexerToken::SUB:
                lexer->acceptToken(LexerToken::SUB);
                currExpr = IRFactory::createSubExpr(currExpr, multiplicativeExpression());
                break;

            default:
                goto exitLoop;
        }
    }

exitLoop:
    return currExpr;
}

/*

          shift-expression:
                  additive-expression
                  shift-expression <<  additive-expression
                  shift-expression >>  additive-expression

*/
IRExprPtr C90Expression::shiftExpression()
{
    IRExprPtr currExpr = additiveExpression();
    for(;;) {
        switch( lexer->peekToken()->getKind()) {
            case LexerToken::SHIFT_LEFT:
                lexer->acceptToken(LexerToken::SHIFT_LEFT);
                currExpr = IRFactory::createShiftLeftExpr(currExpr, additiveExpression());
                break;

            case LexerToken::SHIFT_RIGHT:
                lexer->acceptToken(LexerToken::SHIFT_RIGHT);
                currExpr = IRFactory::createShiftRightExpr(currExpr, additiveExpression());
                break;

            default:
                goto exitLoop;
        }
    }

exitLoop:
    return currExpr;
}

/*

          relational-expression:
                  shift-expression
                  relational-expression <   shift-expression
                  relational-expression >   shift-expression
                  relational-expression <=  shift-expression
                  relational-expression >=  shift-expression

*/
IRExprPtr C90Expression::relationalExpression()
{
    IRExprPtr currExpr = shiftExpression();
    for(;;) {
        switch( lexer->peekToken()->getKind() ) {
            case LexerToken::LT:
                lexer->acceptToken(LexerToken::LT);
                currExpr = IRFactory::createLessThanExpr(currExpr, shiftExpression());
                break;

            case LexerToken::GT:
                lexer->acceptToken(LexerToken::GT);
                currExpr = IRFactory::createGreaterThanExpr(currExpr, shiftExpression());
                break;

            case LexerToken::LE:
                lexer->acceptToken(LexerToken::LE);
                currExpr = IRFactory::createLessEqualExpr(currExpr, shiftExpression());
                break;

            case LexerToken::GE:
                lexer->acceptToken(LexerToken::GE);
                currExpr = IRFactory::createGreaterEqualExpr(currExpr, shiftExpression());
                break;

            default:
                goto exitLoop;
        }
    }

exitLoop:
    return currExpr;
}

/*

          equality-expression:
                  relational-expression
                  equality-expression ==  relational-expression
                  equality-expression !=  relational-expression

*/
IRExprPtr C90Expression::equalityExpression()
{
    IRExprPtr currExpr = relationalExpression();
    for(;;) {
        switch( lexer->peekToken()->getKind()) {
            case LexerToken::EQUAL:
                lexer->acceptToken(LexerToken::EQUAL);
                currExpr = IRFactory::createEqualExpr(currExpr, relationalExpression());
                break;

            case LexerToken::NOT_EQUAL:
                lexer->acceptToken(LexerToken::NOT_EQUAL);
                currExpr = IRFactory::createNotEqualExpr(currExpr, relationalExpression());
                break;
                
            default:
                goto exitLoop;
        }
    }

exitLoop:
    return currExpr;
}

/*

          AND-expression:
                  equality-expression
                  AND-expression &  equality-expression

*/

IRExprPtr C90Expression::bitAndExpression()
{
    IRExprPtr currExpr = equalityExpression();
    for(;;) {
        if( lexer->peekToken()->getKind() == LexerToken::BIT_AND ) {
            lexer->acceptToken(LexerToken::BIT_AND );
            currExpr = IRFactory::createBitAndExpr(currExpr, equalityExpression());
        }
        else {
            break;
        }
    }

    return currExpr;
}

/*

          exclusive-OR-expression:
                  AND-expression
                  exclusive-OR-expression ^  AND-expression

*/
IRExprPtr C90Expression::bitXorExpression()
{
    IRExprPtr currExpr = bitAndExpression();
    for(;;) {
        if( lexer->peekToken()->getKind() == LexerToken::BIT_XOR ) {
            lexer->acceptToken(LexerToken::BIT_XOR );
            currExpr = IRFactory::createBitXorExpr(currExpr, bitAndExpression());
        }
        else {
            break;
        }
    }

    return currExpr;

}

/*

          inclusive-OR-expression:
                  exclusive-OR-expression
                  inclusive-OR-expression |  exclusive-OR-expression

*/
IRExprPtr C90Expression::bitIorExpression()
{
    IRExprPtr currExpr = bitXorExpression();
    for(;;) {
        if( lexer->peekToken()->getKind() == LexerToken::BIT_IOR ) {
            lexer->acceptToken(LexerToken::BIT_IOR );
            currExpr = IRFactory::createBitIorExpr(currExpr, bitXorExpression());
        }
        else {
            break;
        }
    }

    return currExpr;

}

/*

          logical-AND-expression:
                  inclusive-OR-expression
                  logical-AND-expression &&  inclusive-OR-expression

*/
IRExprPtr C90Expression::logicalAndExpression()
{
    IRExprPtr currExpr = bitIorExpression();
    for(;;) {
        if( lexer->peekToken()->getKind() == LexerToken::BOOL_AND ) {
            lexer->acceptToken(LexerToken::BOOL_AND );
            currExpr = IRFactory::createBoolAndExpr(currExpr, bitIorExpression());
        }
        else {
            break;
        }
    }

    return currExpr;

}

/*

          logical-OR-expression:
                  logical-AND-expression
                  logical-OR-expression ||  logical-AND-expression

*/
IRExprPtr C90Expression::logicalOrExpression()
{
    IRExprPtr currExpr = logicalAndExpression();
    for(;;) {
        if( lexer->peekToken()->getKind() == LexerToken::BOOL_OR ) {
            lexer->acceptToken(LexerToken::BOOL_OR );
            currExpr = IRFactory::createBoolOrExpr(currExpr, logicalAndExpression());
        }
        else {
            break;
        }
    }

    return currExpr;
}

/*

          conditional-expression:
                  logical-OR-expression
                  logical-OR-expression ?  expression :  conditional-expression

*/
IRExprPtr C90Expression::conditionalExpression()
{
    IRExprPtr currExpr = logicalOrExpression();

    if( lexer->peekToken()->getKind() == LexerToken::QUESTION_MARK ) {
        lexer->acceptToken(LexerToken::QUESTION_MARK);
        IRExprPtr thenExpr = expression();
        lexer->acceptToken(LexerToken::COLON);
        IRExprPtr elseExpr = conditionalExpression();
        currExpr = IRFactory::createCondExpr(currExpr, thenExpr, elseExpr);
    }

    return currExpr;
}

/*

          assignment-expression:
                  conditional-expression
                  unary-expression assignment-operator assignment-expression

          assignment-operator: one of
                  =  *=  /=  %=  +=  -=  <<=  >>=  &=  ^=  |=

*/
IRExprPtr C90Expression::assignmentExpression()
{
    IRExprPtr currExpr = conditionalExpression();

    // TODO: need to check if currExpr may be assigned... (unary-expression)
    //
    IRExprPtr (*factoryExprFunc)(const IRExprPtr&, const IRExprPtr&);
    switch( lexer->peekToken()->getKind() ) {
        case LexerToken::ASSIGN:
            lexer->acceptToken(LexerToken::ASSIGN);
            factoryExprFunc = IRFactory::createAssignExpr;
            break;

        case LexerToken::MUL_ASSIGN:
            lexer->acceptToken(LexerToken::MUL_ASSIGN);
            factoryExprFunc = IRFactory::createMulAssignExpr;
            break;

        case LexerToken::DIV_ASSIGN:
            lexer->acceptToken(LexerToken::DIV_ASSIGN);
            factoryExprFunc = IRFactory::createDivAssignExpr;
            break;

        case LexerToken::MOD_ASSIGN:
            lexer->acceptToken(LexerToken::MOD_ASSIGN);
            factoryExprFunc = IRFactory::createModAssignExpr;
            break;

        case LexerToken::ADD_ASSIGN:
            lexer->acceptToken(LexerToken::ADD_ASSIGN);
            factoryExprFunc = IRFactory::createAddAssignExpr;
            break;

        case LexerToken::SUB_ASSIGN:
            lexer->acceptToken(LexerToken::SUB_ASSIGN);
            factoryExprFunc = IRFactory::createSubAssignExpr;
            break;

        case LexerToken::SHIFT_LEFT_ASSIGN:
            lexer->acceptToken(LexerToken::SHIFT_LEFT_ASSIGN);
            factoryExprFunc = IRFactory::createShiftLeftAssignExpr;
            break;

        case LexerToken::SHIFT_RIGHT_ASSIGN:
            lexer->acceptToken(LexerToken::SHIFT_RIGHT_ASSIGN);
            factoryExprFunc = IRFactory::createShiftRightAssignExpr;
            break;

        case LexerToken::BIT_AND_ASSIGN:
            lexer->acceptToken(LexerToken::BIT_AND_ASSIGN);
            factoryExprFunc = IRFactory::createBitAndAssignExpr;
            break;

        case LexerToken::BIT_XOR_ASSIGN:
            lexer->acceptToken(LexerToken::BIT_XOR_ASSIGN);
            factoryExprFunc = IRFactory::createBitXorAssignExpr;
            break;

        case LexerToken::BIT_IOR_ASSIGN:
            lexer->acceptToken(LexerToken::BIT_IOR_ASSIGN);
            factoryExprFunc = IRFactory::createBitIorAssignExpr;
            break;

        default:
            return currExpr;   
    }

    return factoryExprFunc(currExpr, assignmentExpression());
}

/*

          expression:
                  assignment-expression
                  expression ,  assignment-expression

*/
IRExprPtr C90Expression::expression()
{
    IRExprPtr currExpr = assignmentExpression();
    for(;;) {
        if( lexer->peekToken()->getKind() == LexerToken::COMMA ) {
            lexer->acceptToken(LexerToken::BOOL_OR );
            currExpr = IRFactory::createCommaExpr(currExpr, assignmentExpression());
        }
        else {
            break;
        }
    }

    return currExpr;
}

/*

          constant-expression:
                  conditional-expression

*/
IRExprPtr C90Expression::constantExpression()
{
    return conditionalExpression();
}



