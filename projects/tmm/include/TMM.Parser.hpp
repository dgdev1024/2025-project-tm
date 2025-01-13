/// @file TMM.Parser.hpp

#pragma once
#include <TMM.Lexer.hpp>
#include <TMM.Syntax.hpp>

namespace tmm
{

    class Parser
    {
    public:
        Program::Ptr    ParseProgram (Lexer& pLexer);

    private:
        Statement::Ptr  ParseStatement (Lexer& pLexer);
        Statement::Ptr  ParseSection (Lexer& pLexer);
        Statement::Ptr  ParseLabel (Lexer& pLexer);
        Statement::Ptr  ParseInstruction (Lexer& pLexer);
        Statement::Ptr  ParseFunction (Lexer& pLexer);

    private:
        Expression::Ptr ParseExpression (Lexer& pLexer);
        Expression::Ptr ParseFunctionCall (Lexer& pLexer);
        Expression::Ptr ParseLogical (Lexer& pLexer);
        Expression::Ptr ParseComparison (Lexer& pLexer);
        Expression::Ptr ParseBitwise (Lexer& pLexer);
        Expression::Ptr ParseAdditive (Lexer& pLexer);
        Expression::Ptr ParseMultiplicitive (Lexer& pLexer);
        Expression::Ptr ParseUnary (Lexer& pLexer);

    private:
        Expression::Ptr ParsePrimaryExpression (Lexer& pLexer);

    };

}
