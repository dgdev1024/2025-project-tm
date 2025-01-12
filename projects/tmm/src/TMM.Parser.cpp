/// @file TMM.Parser.cpp

#include <TMM.Precompiled.hpp>
#include <TMM.Parser.hpp>

namespace tmm
{

    /* Public Methods - Parse Program *************************************************************/

    Program::Ptr Parser::ParseProgram (Lexer& pLexer)
    {
        Program::Ptr lProgram = Statement::Make<Program>();

        while (pLexer.HasMoreTokens() == true)
        {
            Token           lLeadToken = pLexer.TokenAt();
            Statement::Ptr  lStatement = ParseStatement(pLexer);

            if (lStatement != nullptr)
            {
                lProgram->Push(lStatement);
            }
            else
            {
                std::cerr   << "[Parser]   In file '" << lLeadToken.mFile.string() << ":"
                            << lLeadToken.mLine << "'." << std::endl;
                return nullptr;
            }
        }

        return lProgram;
    }

    /* Private Methods - Parse Statements *********************************************************/

    Statement::Ptr Parser::ParseStatement (Lexer& pLexer)
    {
        return ParseExpression(pLexer);
    }

    /* Private Methods - Parse Expressions ********************************************************/

    // Order of Expression Precedence:
    //  - Assignment
    //  - Member
    //  - Function Call
    //  - Logical
    //  - Comparison
    //  - Bitwise
    //  - Additive
    //  - Multiplicitive
    //  - Unary
    //  - Primary

    Expression::Ptr Parser::ParseExpression (Lexer& pLexer)
    {
        return ParseLogical(pLexer);
    }

    Expression::Ptr Parser::ParseLogical (Lexer& pLexer)
    {
        Expression::Ptr lLefthand = ParseComparison(pLexer);
        if (lLefthand == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsLogicalOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthand = ParseComparison(pLexer);
            if (lRighthand == nullptr) { return nullptr; }

            lLefthand = Expression::Make<BinaryExpression>(lLefthand, lRighthand, lOperatorToken);
        }

        return lLefthand;
    }

    Expression::Ptr Parser::ParseComparison (Lexer& pLexer)
    {
        Expression::Ptr lLefthand = ParseBitwise(pLexer);
        if (lLefthand == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsComparisonOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthand = ParseBitwise(pLexer);
            if (lRighthand == nullptr) { return nullptr; }

            lLefthand = Expression::Make<BinaryExpression>(lLefthand, lRighthand, lOperatorToken);
        }

        return lLefthand;
    }

    Expression::Ptr Parser::ParseBitwise (Lexer& pLexer)
    {
        Expression::Ptr lLefthand = ParseAdditive(pLexer);
        if (lLefthand == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsBitwiseOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthand = ParseAdditive(pLexer);
            if (lRighthand == nullptr) { return nullptr; }

            lLefthand = Expression::Make<BinaryExpression>(lLefthand, lRighthand, lOperatorToken);
        }

        return lLefthand;
    }

    Expression::Ptr Parser::ParseAdditive (Lexer& pLexer)
    {
        Expression::Ptr lLefthand = ParseMultiplicitive(pLexer);
        if (lLefthand == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsAdditiveOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthand = ParseMultiplicitive(pLexer);
            if (lRighthand == nullptr) { return nullptr; }

            lLefthand = Expression::Make<BinaryExpression>(lLefthand, lRighthand, lOperatorToken);
        }

        return lLefthand;
    }

    Expression::Ptr Parser::ParseMultiplicitive (Lexer& pLexer)
    {
        Expression::Ptr lLefthand = ParseUnary(pLexer);
        if (lLefthand == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsMultiplicitiveOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthand = ParseUnary(pLexer);
            if (lRighthand == nullptr) { return nullptr; }

            lLefthand = Expression::Make<BinaryExpression>(lLefthand, lRighthand, lOperatorToken);
        }

        return lLefthand;
    }

    Expression::Ptr Parser::ParseUnary (Lexer& pLexer)
    {
        if (pLexer.TokenAt().IsUnaryOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthand = ParsePrimaryExpression(pLexer);
            if (lRighthand == nullptr) { return nullptr; }

            return Expression::Make<UnaryExpression>(lRighthand, lOperatorToken);
        }
        else
        {
            return ParsePrimaryExpression(pLexer);
        }
    }

    /* Private Methods - Parse Primary Expressions ************************************************/

    Expression::Ptr Parser::ParsePrimaryExpression (Lexer& pLexer)
    {
        auto lToken = pLexer.DiscardToken();

        switch (lToken.mType)
        {
            case TokenType::Identifier:
            {
                return Expression::Make<Identifier>(lToken.mValue);
            } break;

            case TokenType::Char:
            {
                return Expression::Make<NumericLiteral>(lToken.mValue.at(0));
            } break;

            case TokenType::String:
            {
                return Expression::Make<StringLiteral>(lToken.mValue);
            } break;

            case TokenType::Number:
            {
                return Expression::Make<NumericLiteral>(std::stod(lToken.mValue));
            } break;

            case TokenType::Binary:
            {
                return Expression::Make<NumericLiteral>(std::stoul(lToken.mValue, nullptr, 2));
            } break;

            case TokenType::Octal:
            {
                return Expression::Make<NumericLiteral>(std::stoul(lToken.mValue, nullptr, 8));
            } break;

            case TokenType::Hexadecimal:
            {
                return Expression::Make<NumericLiteral>(std::stoul(lToken.mValue, nullptr, 16));
            } break;

            case TokenType::OpenParen:
            {
                auto lExpression = ParseExpression(pLexer);
                if (lExpression == nullptr) { return nullptr; }

                if (pLexer.DiscardToken().mType != TokenType::CloseParen)
                {
                    std::cerr   << "[Parser] Missing ')' at end of parenthesis-enclosed expression."
                                << std::endl;
                    return nullptr;
                }

                return lExpression;
            } break;

            case TokenType::EndOfFile:
            {
                std::cerr << "[Parser] Unexpected end-of-file reached during parsing." << std::endl;
                return nullptr;
            } break;

            default:
            {
                std::cerr << "[Parser] Unexpected '" << lToken.ToString() << "' token";

                if (lToken.mValue.empty())
                {
                    std::cerr << "." << std::endl;
                }
                else
                {
                    std::cerr << " = '" << lToken.mValue << "'." << std::endl;
                }

                return nullptr;
            } break;
        }
    }

}
