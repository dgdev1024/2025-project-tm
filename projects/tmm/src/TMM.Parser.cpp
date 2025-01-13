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
            if (pLexer.DiscardNewLine() == true)
            {
                continue;
            }

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
        const auto& lToken      = pLexer.TokenAt();
        const auto& lKeyword    = lToken.GetKeyword();

        if (lKeyword.mType == KeywordType::Language)
        {
            switch (lKeyword.mParamOne)
            {
                case LanguageType::LT_SECTION:  return ParseSection(pLexer);
                case LanguageType::LT_FUNCTION: return ParseFunction(pLexer);
                default:
                    std::cerr   << "[Parser] Un-implemented language keyword: '"
                                << lToken.mValue << "'." << std::endl;
                    return nullptr;
            }
        }
        else if (lKeyword.mType == KeywordType::Instruction)
        {
            return ParseInstruction(pLexer);
        }
        else if (lToken.mType == TokenType::Period)
        {
            return ParseLabel(pLexer);
        }

        return ParseExpression(pLexer);
    }

    Statement::Ptr Parser::ParseSection (Lexer& pLexer)
    {
        pLexer.DiscardToken();

        auto lSection = pLexer.DiscardToken();
        const auto& lKeyword = lSection.GetKeyword();
        if (lKeyword.mType != KeywordType::Section)
        {
            std::cerr << "[Parser] Expected section name after 'section' statement." << std::endl;
            return nullptr;
        }

        if (pLexer.DiscardNewLine() == true)
        {
            return Statement::Make<SectionStatement>(lSection);
        }
        else if (pLexer.DiscardTokenIf(TokenType::Comma) == false)
        {
            std::cerr << "[Parser] Expected ',' between section and offset after 'section' statement." << std::endl;
            return nullptr;
        }

        Expression::Ptr lOffset = ParseExpression(pLexer);
        if (lOffset == nullptr) { return nullptr; }

        if (pLexer.DiscardNewLine() == false)
        {
            std::cerr << "[Parser] Expected end of line at end of 'section' statement." << std::endl;
            return nullptr;
        }

        return Statement::Make<SectionStatement>(lSection, lOffset);
    }

    Statement::Ptr Parser::ParseLabel (Lexer& pLexer)
    {
        pLexer.DiscardToken();

        Expression::Ptr lSymbol = ParseExpression(pLexer);
        if (lSymbol == nullptr) { return nullptr; }
        
        if (pLexer.DiscardTokenIf(TokenType::Colon) == false)
        {
            std::cerr << "[Parser] Expected ':' after symbol in label statement." << std::endl;
            return nullptr;
        }

        return Statement::Make<LabelStatement>(lSymbol);
    }

    Statement::Ptr Parser::ParseInstruction (Lexer& pLexer)
    {
        const auto& lKeyword = pLexer.DiscardToken().GetKeyword();

        if (pLexer.DiscardNewLine() == true)
        {
            return Statement::Make<InstructionStatement>(lKeyword.mParamOne);
        }

        Expression::Ptr lOperandOne = ParseExpression(pLexer);
        if (lOperandOne == nullptr) { return nullptr; }

        if (pLexer.DiscardNewLine() == true)
        {
            return Statement::Make<InstructionStatement>(lKeyword.mParamOne, lOperandOne);
        }
        else if (pLexer.DiscardTokenIf(TokenType::Comma) == false)
        {
            std::cerr   << "[Parser] Expected ',' between arguments of instruction." << std::endl;
            return nullptr;
        }

        Expression::Ptr lOperandTwo = ParseExpression(pLexer);
        if (lOperandTwo == nullptr) { return nullptr; }

        if (pLexer.DiscardNewLine() == false)
        {
            std::cerr   << "[Parser] Expected new line at end of instruction." << std::endl;
            return nullptr;
        }

        return Statement::Make<InstructionStatement>(lKeyword.mParamOne, lOperandOne, lOperandTwo);
    }

    Statement::Ptr Parser::ParseFunction (Lexer& pLexer)
    {
        pLexer.DiscardToken();

        Expression::Ptr lName = ParseExpression(pLexer);
        if (lName == nullptr) { return nullptr; }

        pLexer.DiscardNewLine();
        if (pLexer.DiscardTokenIf(TokenType::OpenBrace) == false)
        {
            std::cerr << "[Parser] Expected '{' after name in 'function' declaration." << std::endl;
            return nullptr;
        }

        tmc::Shared<FunctionStatement> lFunction = Statement::Make<FunctionStatement>(lName);

        while (pLexer.DiscardTokenIf(TokenType::CloseBrace) == false)
        {
            if (pLexer.DiscardNewLine() == true)
            {
                continue;
            }

            Statement::Ptr lStatement = ParseStatement(pLexer);
            if (lStatement == nullptr)  { return nullptr; }
            else                        { lFunction->Push(lStatement); }

            pLexer.DiscardNewLine();
        }

        return lFunction;
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
        return ParseFunctionCall(pLexer);
    }

    Expression::Ptr Parser::ParseFunctionCall (Lexer& pLexer)
    {
        Expression::Ptr lName = ParseLogical(pLexer);
        if (lName == nullptr) { return nullptr; }

        if (pLexer.DiscardTokenIf(TokenType::OpenParen) == false)
        {
            return lName;
        }
        
        tmc::Shared<FunctionCall> lCall = Expression::Make<FunctionCall>(lName);
        if (pLexer.DiscardTokenIf(TokenType::CloseParen) == true)
        {
            return lCall;
        }

        while (true)
        {
            Expression::Ptr lArgument = ParseExpression(pLexer);
            if (lArgument == nullptr)   { return nullptr; }
            else                        { lCall->PushArgument(lArgument); }

            if (pLexer.DiscardTokenIf(TokenType::CloseParen) == true)
            {
                return lCall;
            }
            else if (pLexer.DiscardTokenIf(TokenType::Comma) == false)
            {
                std::cerr << "[Parser] Expected ',' between arguments in function call." << std::endl;
                return nullptr;
            }
        }
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
            case TokenType::Keyword:
            {
                const auto& lKeyword = lToken.GetKeyword();
                switch (lKeyword.mType)
                {
                    case KeywordType::Register:
                        return Expression::Make<RegisterLiteral>(lKeyword.mParamOne);
                    case KeywordType::Condition:
                        return Expression::Make<ConditionLiteral>(lKeyword.mParamOne);
                    default: break;
                }
            }

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

            case TokenType::Placeholder:
            {
                return Expression::Make<PlaceholderLiteral>(std::stoul(lToken.mValue, nullptr, 10));
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
