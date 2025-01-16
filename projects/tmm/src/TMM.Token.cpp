/// @file TMM.Token.cpp

#include <TMM.Precompiled.hpp>
#include <TMM.Token.hpp>

namespace tmm
{

    /* Public Methods *****************************************************************************/

    const char* Token::ToString () const
    {
        switch (mType)
        {
            case TokenType::Keyword:                    return "Keyword";
            case TokenType::Identifier:                 return "Identifier";
            case TokenType::Char:                       return "Char";
            case TokenType::String:                     return "String";
            case TokenType::Number:                     return "Number";
            case TokenType::Binary:                     return "Binary";
            case TokenType::Octal:                      return "Octal";
            case TokenType::Hexadecimal:                return "Hexadecimal";
            case TokenType::Placeholder:                return "Placeholder";

            case TokenType::Plus:                       return "Plus";
            case TokenType::Minus:                      return "Minus";
            case TokenType::Concat:                     return "Concat";
            case TokenType::Times:                      return "Times";
            case TokenType::Divide:                     return "Divide";
            case TokenType::Modulo:                     return "Modulo";
            case TokenType::Increment:                  return "Increment";
            case TokenType::Decrement:                  return "Decrement";

            case TokenType::BitwiseAnd:                 return "Bitwise And";
            case TokenType::BitwiseOr:                  return "Bitwise Or";
            case TokenType::BitwiseXor:                 return "Bitwise Xor";
            case TokenType::BitwiseNot:                 return "Bitwise Not";
            case TokenType::BitwiseLeftShift:           return "Bitwise Left Shift";
            case TokenType::BitwiseRightShift:          return "Bitwise Right Shift";

            case TokenType::AssignEquals:               return "Assignment";
            case TokenType::AssignPlus:                 return "Plus Assignment";
            case TokenType::AssignMinus:                return "Minus Assignment";
            case TokenType::AssignTimes:                return "Times Assignment";
            case TokenType::AssignDivide:               return "Divide Assignment";
            case TokenType::AssignModulo:               return "Modulo Assignment";
            case TokenType::AssignBitwiseAnd:           return "Bitwise And Assignment";
            case TokenType::AssignBitwiseOr:            return "Bitwise Or Assignment";
            case TokenType::AssignBitwiseXor:           return "Bitwise Xor Assignment";
            case TokenType::AssignBitwiseLeftShift:     return "Bitwise Left Shift Assignment";
            case TokenType::AssignBitwiseRightShift:    return "Bitwise Right Shift Assignment";

            case TokenType::CompareEquals:              return "Equals";
            case TokenType::CompareStrictEquals:        return "Strictly Equals";
            case TokenType::CompareNotEquals:           return "Not Equals";
            case TokenType::CompareStrictNotEquals:     return "Not Strictly Equals";
            case TokenType::CompareGreater:             return "Greater Than";
            case TokenType::CompareLess:                return "Less Than";
            case TokenType::CompareGreaterEquals:       return "Greater Than Or Equals";
            case TokenType::CompareLessEquals:          return "Less Than Or Equals";

            case TokenType::LogicalAnd:                 return "Logical And";
            case TokenType::LogicalOr:                  return "Logical Or";
            case TokenType::LogicalNot:                 return "Logical Not";

            case TokenType::OpenParen:                  return "Open Parenthesis";
            case TokenType::CloseParen:                 return "Close Parenthesis";
            case TokenType::OpenBracket:                return "Open Bracket";
            case TokenType::CloseBracket:               return "Close Bracket";
            case TokenType::OpenBrace:                  return "Open Brace";
            case TokenType::CloseBrace:                 return "Close Brace";

            case TokenType::Comma:                      return "Comma";
            case TokenType::Colon:                      return "Colon";
            case TokenType::Period:                     return "Period";
            case TokenType::Arrow:                      return "Arrow";
            
            case TokenType::EndOfFile:                  return "End Of File";
            default:                                    return "Unknown";
        }
    }

    const Keyword& Token::GetKeyword () const
    {
        return Keyword::Lookup(mValue);
    }

    tmc::Boolean Token::IsOperator () const
    {
        switch (mType)
        {
            case TokenType::Plus:
            case TokenType::Minus:
            case TokenType::Times:
            case TokenType::Divide:
            case TokenType::Modulo:
            case TokenType::BitwiseAnd:
            case TokenType::BitwiseOr:
            case TokenType::BitwiseXor:
            case TokenType::BitwiseNot:
            case TokenType::BitwiseLeftShift:
            case TokenType::BitwiseRightShift:
            case TokenType::AssignEquals:
            case TokenType::AssignPlus:
            case TokenType::AssignMinus:
            case TokenType::AssignTimes:
            case TokenType::AssignDivide:
            case TokenType::AssignModulo:
            case TokenType::AssignBitwiseAnd:
            case TokenType::AssignBitwiseOr:
            case TokenType::AssignBitwiseXor:
            case TokenType::AssignBitwiseLeftShift:
            case TokenType::AssignBitwiseRightShift:
            case TokenType::CompareEquals:
            case TokenType::CompareStrictEquals:
            case TokenType::CompareNotEquals:
            case TokenType::CompareStrictNotEquals:
            case TokenType::CompareGreater:
            case TokenType::CompareLess:
            case TokenType::CompareGreaterEquals:
            case TokenType::CompareLessEquals:
            case TokenType::LogicalAnd:
            case TokenType::LogicalOr:
            case TokenType::LogicalNot: return true;
            default: return false;
        }
    }

    tmc::Boolean Token::IsAssignmentOperator () const
    {
        switch (mType)
        {
            case TokenType::AssignEquals:
            case TokenType::AssignPlus:
            case TokenType::AssignMinus:
            case TokenType::AssignTimes:
            case TokenType::AssignDivide:
            case TokenType::AssignModulo:
            case TokenType::AssignBitwiseAnd:
            case TokenType::AssignBitwiseOr:
            case TokenType::AssignBitwiseXor:
            case TokenType::AssignBitwiseLeftShift:
            case TokenType::AssignBitwiseRightShift: return true;
            default: return false;
        }
    }

    tmc::Boolean Token::IsLogicalOperator () const
    {
        switch (mType)
        {
            case TokenType::LogicalAnd:
            case TokenType::LogicalOr: return true;
            default: return false;
        }
    }

    tmc::Boolean Token::IsComparisonOperator () const
    {
        switch (mType)
        {
            case TokenType::CompareEquals:
            case TokenType::CompareStrictEquals:
            case TokenType::CompareNotEquals:
            case TokenType::CompareStrictNotEquals:
            case TokenType::CompareGreater:
            case TokenType::CompareLess:
            case TokenType::CompareGreaterEquals:
            case TokenType::CompareLessEquals:  return true;
            default: return false;
        }
    }

    tmc::Boolean Token::IsBitwiseOperator () const
    {
        switch (mType)
        {
            case TokenType::BitwiseAnd:
            case TokenType::BitwiseOr:
            case TokenType::BitwiseXor:
            case TokenType::BitwiseNot:
            case TokenType::BitwiseLeftShift:
            case TokenType::BitwiseRightShift: return true;
            default: return false;
        }
    }

    tmc::Boolean Token::IsAdditiveOperator () const
    {
        switch (mType)
        {
            case TokenType::Plus:
            case TokenType::Minus: 
            case TokenType::Concat: return true;
            default: return false;
        }
    }

    tmc::Boolean Token::IsMultiplicitiveOperator () const
    {
        switch (mType)
        {
            case TokenType::Times:
            case TokenType::Divide:
            case TokenType::Modulo: return true;
            default: return false;
        }
    }

    tmc::Boolean Token::IsUnaryOperator () const
    {
        switch (mType)
        {
            case TokenType::LogicalNot:
            case TokenType::BitwiseNot:
            case TokenType::Plus:
            case TokenType::Minus: return true;
            default: return false;
        }
    }

}
