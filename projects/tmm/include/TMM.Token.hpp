/// @file TMM.Token.hpp

#pragma once

#include <TMM.Keyword.hpp>

namespace tmm
{

    enum class TokenType
    {
        Unknown,

        // Keywords
        Keyword,

        // Literal Tokens
        Identifier,
        Char,
        String,
        Number,
        Binary,
        Octal,
        Hexadecimal,

        // Arithmetic Operator Tokens
        Plus,
        Minus,
        Times,
        Exponent,
        Divide,
        Modulo,
        Increment,
        Decrement,

        // Bitwise Operator Tokens
        BitwiseAnd,
        BitwiseOr,
        BitwiseXor,
        BitwiseNot,
        BitwiseLeftShift,
        BitwiseRightShift,

        // Assignment Operator Tokens
        AssignEquals,
        AssignPlus,
        AssignMinus,
        AssignTimes,
        AssignExponent,
        AssignDivide,
        AssignModulo,
        AssignBitwiseAnd,
        AssignBitwiseOr,
        AssignBitwiseXor,
        AssignBitwiseLeftShift,
        AssignBitwiseRightShift,

        // Comparison Operator Tokens
        CompareEquals,
        CompareStrictEquals,
        CompareNotEquals,
        CompareStrictNotEquals,
        CompareGreater,
        CompareLess,
        CompareGreaterEquals,
        CompareLessEquals,

        // Logical Operator Tokens
        LogicalAnd,
        LogicalOr,
        LogicalNot,

        // Bracket Tokens
        OpenParen,
        CloseParen,
        OpenBracket,
        CloseBracket,
        OpenBrace,
        CloseBrace,

        // Other Tokens
        NewLine,
        EndOfFile
    };

    struct Token
    {
        TokenType       mType = TokenType::Unknown;
        tmc::String     mValue = "";
        tmc::Path       mFile = "";
        tmc::Index      mLine = 0;

    public:
        const char*     ToString () const;
        const Keyword&  GetKeyword () const;
        tmc::Boolean    IsOperator () const;
        tmc::Boolean    IsLogicalOperator () const;
        tmc::Boolean    IsComparisonOperator () const;
        tmc::Boolean    IsBitwiseOperator () const;
        tmc::Boolean    IsMultiplicitiveOperator () const;
        tmc::Boolean    IsAdditiveOperator () const;
        tmc::Boolean    IsUnaryOperator () const;

    };

}
