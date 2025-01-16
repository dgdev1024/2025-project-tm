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
            Token lLeadToken = pLexer.TokenAt();
            Statement::Ptr lStatement = ParseStatement(pLexer);

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
                case LanguageType::LT_SECTION:      return ParseSection(pLexer);
                case LanguageType::LT_DB:
                case LanguageType::LT_DW:
                case LanguageType::LT_DL:
                case LanguageType::LT_DS:           return ParseData(pLexer);
                default:
                    std::cerr << "[Parser] Un-implemented language keyword: '" << lToken.mValue << "'." << std::endl;
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
        pLexer.DiscardToken();      // Discard the 'SECTION' token.

        // Discard the next token and store its keyword. It should be a memory section keyword.
        const Keyword& lSectionKeyword = pLexer.DiscardToken().GetKeyword();
        if (lSectionKeyword.mType != KeywordType::Section)
        {
            std::cerr << "[Parser] Expected memory section keyword in 'section' statement." << std::endl;
            return nullptr;
        }

        return Statement::Make<SectionStatement>(lSectionKeyword.mParamOne);
    }

    Statement::Ptr Parser::ParseLabel (Lexer& pLexer)
    {
        pLexer.DiscardToken();          // Discard the leading period token.

        // Parse the label expression.
        Expression::Ptr lExpression = ParseExpression(pLexer);
        if (lExpression == nullptr) { return nullptr; }

        // Expect a colon at the end of the expression.
        if (pLexer.DiscardTokenIf(TokenType::Colon) == false)
        {
            std::cerr << "[Parser] Expected ':' after expression in label statement." << std::endl;
            return nullptr;
        }

        return Statement::Make<LabelStatement>(lExpression);
    }

    Statement::Ptr Parser::ParseData (Lexer& pLexer)
    {
        // Discard the leading token, but keep track of its keyword.
        const Keyword& lDataKeyword = pLexer.DiscardToken().GetKeyword();

        // Create the data statement now.
        DataStatement::Ptr lStatement = Statement::Make<DataStatement>(lDataKeyword.mParamOne);

        // Loop, parsing expressions along the way.
        while (true)
        {
            Expression::Ptr lExpression = ParseExpression(pLexer);
            if (lExpression == nullptr)     { return nullptr; }
            else                            { lStatement->PushExpression(lExpression); }

            // Continue parsing expressions for this data statement until a separating comma is
            // not encountered.
            if (pLexer.DiscardTokenIf(TokenType::Comma) == false)
            {
                break;
            }
        }

        return lStatement;
    }

    Statement::Ptr Parser::ParseInstruction (Lexer& pLexer)
    {
        // Discard the leading token, but keep track of its keyword.
        const Keyword& lInstructionKeyword = pLexer.DiscardToken().GetKeyword();

        // The instruction keyword structures have a second parameter to them, which is the number
        // of arguments required by the instruction. Begin parsing through these arguments, if there
        // are to be any.
        if (lInstructionKeyword.mParamTwo == 0)
        {
            return Statement::Make<InstructionStatement>(lInstructionKeyword.mParamOne);
        }

        // Parse the first operand expression.
        Expression::Ptr lFirstOperandExpression = ParseExpression(pLexer);
        if (lFirstOperandExpression == nullptr) { return nullptr; }

        if (lInstructionKeyword.mParamTwo == 1)
        {
            return Statement::Make<InstructionStatement>(lInstructionKeyword.mParamOne,
                lFirstOperandExpression);
        }
        
        // For instructions with two parameters, expect a comma between the parameters.
        if (pLexer.DiscardTokenIf(TokenType::Comma) == false)
        {
            std::cerr << "[Parser] Expected ',' between arguments in instruction statement." << std::endl;
            return nullptr;
        }

        // Parse the second operand expression.
        Expression::Ptr lSecondOperandExpression = ParseExpression(pLexer);
        if (lSecondOperandExpression == nullptr) { return nullptr; }

        return Statement::Make<InstructionStatement>(lInstructionKeyword.mParamOne,
            lFirstOperandExpression, lSecondOperandExpression);
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
        Expression::Ptr lLefthandExpression = ParseComparison(pLexer);
        if (lLefthandExpression == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsLogicalOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthandExpression = ParseComparison(pLexer);
            if (lRighthandExpression == nullptr) { return nullptr; }

            lLefthandExpression = Expression::Make<BinaryExpression>(lLefthandExpression, 
                lRighthandExpression, lOperatorToken);
        }

        return lLefthandExpression;
    }

    Expression::Ptr Parser::ParseComparison (Lexer& pLexer)
    {
        Expression::Ptr lLefthandExpression = ParseBitwise(pLexer);
        if (lLefthandExpression == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsComparisonOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthandExpression = ParseBitwise(pLexer);
            if (lRighthandExpression == nullptr) { return nullptr; }

            lLefthandExpression = Expression::Make<BinaryExpression>(lLefthandExpression, 
                lRighthandExpression, lOperatorToken);
        }

        return lLefthandExpression;
    }

    Expression::Ptr Parser::ParseBitwise (Lexer& pLexer)
    {
        Expression::Ptr lLefthandExpression = ParseAdditive(pLexer);
        if (lLefthandExpression == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsBitwiseOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthandExpression = ParseAdditive(pLexer);
            if (lRighthandExpression == nullptr) { return nullptr; }

            lLefthandExpression = Expression::Make<BinaryExpression>(lLefthandExpression, 
                lRighthandExpression, lOperatorToken);
        }

        return lLefthandExpression;
    }

    Expression::Ptr Parser::ParseAdditive (Lexer& pLexer)
    {
        Expression::Ptr lLefthandExpression = ParseMultiplicitive(pLexer);
        if (lLefthandExpression == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsAdditiveOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthandExpression = ParseMultiplicitive(pLexer);
            if (lRighthandExpression == nullptr) { return nullptr; }

            lLefthandExpression = Expression::Make<BinaryExpression>(lLefthandExpression, 
                lRighthandExpression, lOperatorToken);
        }

        return lLefthandExpression;
    }

    Expression::Ptr Parser::ParseMultiplicitive (Lexer& pLexer)
    {
        Expression::Ptr lLefthandExpression = ParseUnary(pLexer);
        if (lLefthandExpression == nullptr) { return nullptr; }

        while (pLexer.TokenAt().IsMultiplicitiveOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthandExpression = ParseUnary(pLexer);
            if (lRighthandExpression == nullptr) { return nullptr; }

            lLefthandExpression = Expression::Make<BinaryExpression>(lLefthandExpression, 
                lRighthandExpression, lOperatorToken);
        }

        return lLefthandExpression;
    }

    Expression::Ptr Parser::ParseUnary (Lexer& pLexer)
    {
        if (pLexer.TokenAt().IsUnaryOperator() == true)
        {
            Token lOperatorToken = pLexer.DiscardToken();
            Expression::Ptr lRighthandExpression = ParsePrimaryExpression(pLexer);
            if (lRighthandExpression == nullptr) { return nullptr; }

            return Expression::Make<UnaryExpression>(lRighthandExpression, lOperatorToken);
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

            case TokenType::OpenBracket:
            {
                auto lExpression = ParseExpression(pLexer);
                if (lExpression == nullptr) { return nullptr; }

                if (pLexer.DiscardToken().mType != TokenType::CloseBracket)
                {
                    std::cerr   << "[Parser] Missing ']' at end of address expression."
                                << std::endl;
                    return nullptr;
                }

                return Expression::Make<AddressExpression>(lExpression); 
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
