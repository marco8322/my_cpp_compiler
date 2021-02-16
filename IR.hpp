// IR.hpp
//
// Author: Marco Jacques
//
// Intermediate representation
//

#pragma once

#include "LexerToken.hpp"
#include <vector>

class IRType {
public:
    virtual ~IRType() = 0;
};

class IRExpr {
public:
    virtual ~IRExpr() = 0;    
};

using IRExprPtr = std::shared_ptr<IRExpr>;
using IRTypePtr = std::shared_ptr<IRType>;

namespace IRFactory {
    IRExprPtr createIdExpr(const LexerTokenPtr& id);
    IRExprPtr createIntLitExpr(const LexerTokenPtr& intLiteral);
    IRExprPtr createStringLitExpr(const LexerTokenPtr& stringLiteral);
 
    IRExprPtr createArraySubscripting(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createCallExpr(const IRExprPtr& functor, const std::vector<IRExprPtr>& args);
    IRExprPtr createStructFieldDirectAccess(const IRExprPtr& structExpr, const LexerTokenPtr& id);
    IRExprPtr createStructFieldIndirectAccess(const IRExprPtr& structExpr, const LexerTokenPtr& id);
    IRExprPtr createPostIncrExpr(const IRExprPtr& expr);
    IRExprPtr createPostDecrExpr(const IRExprPtr& expr);

    IRExprPtr createPreIncrExpr(const IRExprPtr& expr);
    IRExprPtr createPreDecrExpr(const IRExprPtr& expr);
    IRExprPtr createAddressOfExpr(const IRExprPtr& castExpr);
    IRExprPtr createDereferenceExpr(const IRExprPtr& castExpr);
    IRExprPtr createUnaryPlusExpr(const IRExprPtr& castExpr);
    IRExprPtr createUnaryMinusExpr(const IRExprPtr& castExpr);
    IRExprPtr createBitNotExpr(const IRExprPtr& castExpr);
    IRExprPtr createBoolNotExpr(const IRExprPtr& castExpr);
    IRExprPtr createSizeofExpr(const IRExprPtr& unaryExpr);
    IRExprPtr createSizeofTypeExpr(const IRTypePtr& type);

    IRExprPtr createCastExpr(const IRTypePtr& type, const IRExprPtr& castExpr);

    IRExprPtr createMulExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createDivExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createModExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);

    IRExprPtr createAddExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createSubExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);

    IRExprPtr createShiftLeftExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createShiftRightExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);

    IRExprPtr createLessThanExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createGreaterThanExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createLessEqualExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createGreaterEqualExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);

    IRExprPtr createEqualExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createNotEqualExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);

    IRExprPtr createBitAndExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createBitXorExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createBitIorExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);

    IRExprPtr createBoolAndExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createBoolOrExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);

    IRExprPtr createCondExpr(const IRExprPtr& cond, const IRExprPtr& thenExpr, const IRExprPtr& elseExpr);
    
    IRExprPtr createAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createMulAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createDivAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createModAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createAddAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createSubAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createShiftLeftAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createShiftRightAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createBitAndAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createBitXorAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    IRExprPtr createBitIorAssignExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    
    IRExprPtr createCommaExpr(const IRExprPtr& leftExpr, const IRExprPtr& rightExpr);
    
    IRTypePtr getCharType();
    IRTypePtr getSignedCharType();
    IRTypePtr getUnsignedCharType();
    IRTypePtr getShortType();
    IRTypePtr getUnsignedShortType();
    IRTypePtr getIntType();
    IRTypePtr getUnsignedType();
    IRTypePtr getLongType();
    IRTypePtr getUnsignedLongType();
    
    IRTypePtr getVoidType();
    IRTypePtr getFloatType();
    IRTypePtr getDoubleType();

    IRTypePtr getPointerType(const IRTypePtr& targetType);
    IRTypePtr getArrayType(const IRTypePtr& elementType, uint64_t nbElements);
    IRTypePtr getFunctionType(const IRTypePtr& returnType, const std::vector<IRTypePtr>& argsType, bool isKandR, bool hasVarArgs);
}