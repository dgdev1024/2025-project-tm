/// @file TMM.Lexer.cpp

#include <TMM.Precompiled.hpp>
#include <TMM.Lexer.hpp>

namespace tmm
{

    /* Public Constructors and Destructor *********************************************************/

    Lexer::Lexer ()
    {

    }

    Lexer::~Lexer ()
    {

    }

    /* Public Methods *****************************************************************************/

    void Lexer::ListTokens () const
    {
        for (tmc::Index lIndex = 0; lIndex < mTokens.size(); ++lIndex)
        {
            const auto& lToken = mTokens.at(lIndex);
            std::cout << lIndex << ". " << lToken.ToString();

            if (lToken.mValue.empty() == false)
            {
                std::cout << " = '" << lToken.mValue << "'";
            }

            std::cout << '\n';
        }
    }

    tmc::Boolean Lexer::HasMoreTokens () const
    {
        return  mTokens.empty() == false &&
                mTokens.front().mType != TokenType::EndOfFile;
    }

    const Token& Lexer::TokenAt (const tmc::Index& pIndex) const
    {
        if (pIndex >= mTokens.size())
        {
            std::cerr << "[Lexer] Token index " << pIndex << " is out of range!" << std::endl;
            throw std::out_of_range { "Token index out of range!" };
        }

        return mTokens.at(pIndex);
    }

    Token Lexer::DiscardToken ()
    {
        Token lDiscardedToken = mTokens.front();
        if (mTokens.size() > 1 && lDiscardedToken.mType != TokenType::EndOfFile)
        {
            mTokens.erase(mTokens.begin());
        }

        return lDiscardedToken;
    }

    tmc::Boolean Lexer::DiscardNewLine ()
    {
        if (
            mTokens.front().mType == TokenType::NewLine ||
            mTokens.front().mType == TokenType::EndOfFile
        )
        {
            if (mTokens.size() > 1)
            {
                mTokens.erase(mTokens.begin());
            }

            return true;
        }

        return false;
    }

    tmc::Boolean Lexer::DiscardTokenIf (const TokenType& pType)
    {
        if (mTokens.front().mType == pType)
        {
            if (mTokens.size() > 1)
            {
                mTokens.erase(mTokens.begin());
            }

            return true;
        }

        return false;
    }

    tmc::Boolean Lexer::TokenizeFile (const tmc::Path& pPath)
    {
        if (pPath.empty() == true)
        {
            std::cerr << "[Lexer] No input file provided." << std::endl;
            return false;
        }
        
        tmc::Path lFullPath = fs::absolute(pPath).lexically_normal();
        if (mLexedPaths.contains(lFullPath))
        {
            return true;
        }
        else if (fs::exists(lFullPath) == false)
        {
            std::cerr << "[Lexer] File '" << lFullPath.string() << "' not found." << std::endl;
            return false;
        }
        else
        {
            mLexedPaths.insert(lFullPath);
        }

        std::fstream lFile { lFullPath, std::ios::in };
        if (lFile.is_open() == false)
        {
            std::cerr << "[Lexer] File '" << lFullPath.string() << "' could not be opened." << std::endl;
            return false;
        }

        mCurrentFile = lFullPath;
        mCurrentLine = 1;

        tmc::Boolean lResult = TokenizeStream(lFile);
        if (lResult == false)
        {
            std::cerr << "[Lexer]   In source file '" << lFullPath.string() << "'" << std::endl;
        }

        return lResult;
    }

    tmc::Boolean Lexer::TokenizeStream (std::istream& pStream)
    {
        tmc::Int32      lCharacter = 0;
        tmc::Boolean    lIsComment = false;
        tmc::Boolean    lIsGood = false;

        while (true)
        {
            lCharacter = pStream.get();

            if (lCharacter == std::char_traits<char>::eof())
            {
                return InsertToken(TokenType::EndOfFile);
            }

            if (lCharacter == '\n')
            {
                lIsComment = false;
                mCurrentLine++;
                InsertToken(TokenType::NewLine);
                continue;
            }

            if (lIsComment == true)
            {
                continue;
            }
            else if (lCharacter == ';')
            {
                lIsComment = true;
                continue;
            }
            else if (std::isspace(lCharacter))
            {
                continue;
            }

            if (lCharacter == '_' || std::isalpha(lCharacter))
                { lIsGood = TokenizeIdentifier(pStream, lCharacter); }
            else if (lCharacter == '\'')
                { lIsGood = TokenizeChar(pStream, lCharacter); }
            else if (lCharacter == '"')
                { lIsGood = TokenizeString(pStream, lCharacter); }
            else if (lCharacter == '@' || std::isdigit(lCharacter))
                { lIsGood = TokenizeNumber(pStream, lCharacter); }
            else
                { lIsGood = TokenizeSymbol(pStream, lCharacter); }

            if (lIsGood == false)
            {
                std::cerr << "[Lexer]   At line #" << mCurrentLine << std::endl;
                return false;
            }
        }
    }

    /* Private Methods - Tokenization *************************************************************/

    tmc::Boolean Lexer::InsertToken (const TokenType& pType, const tmc::String& pValue)
    {
        mTokens.emplace(mTokens.begin() + (mTokenPointer++), pType, pValue, mCurrentFile,
            mCurrentLine);
        return true;
    }

    tmc::Boolean Lexer::TokenizeIdentifier (std::istream& pStream, tmc::Int32& pCharacter)
    {
        tmc::String lValue      = "";
        tmc::String lUppercase  = "";

        do
        {
            lValue      += static_cast<char>(pCharacter);
            lUppercase  += static_cast<char>(std::toupper(pCharacter));
            pCharacter  = pStream.get();
        } while (pCharacter == '_' || std::isalnum(pCharacter));

        pStream.unget();

        const Keyword& lKeyword = Keyword::Lookup(lUppercase);
        if (lKeyword.mType != KeywordType::None)
        {
            return InsertToken(TokenType::Keyword, lUppercase);
        }
        
        return InsertToken(TokenType::Identifier, lValue);
    }

    tmc::Boolean Lexer::TokenizeChar (std::istream& pStream, tmc::Int32& pCharacter)
    {
        tmc::String lValue      = "";

        pCharacter = pStream.get();

        lValue += static_cast<char>(pCharacter);
        pCharacter = pStream.get();

        if (pCharacter != '\'')
        {
            std::cerr << "[Lexer] More than one character found in character literal." << std::endl;
            return false;
        }

        return InsertToken(TokenType::Char, lValue);
    }

    tmc::Boolean Lexer::TokenizeString (std::istream& pStream, tmc::Int32& pCharacter)
    {
        tmc::String lValue      = "";

        pCharacter = pStream.get();
        while (pCharacter != '"')
        {
            if (pCharacter == std::char_traits<char>::eof())
            {
                std::cerr << "[Lexer] Unexpected end-of-file found while parsing string." << std::endl;
                return false;
            }

            lValue += static_cast<char>(pCharacter);
            pCharacter = pStream.get();
        }

        return InsertToken(TokenType::String, lValue);
    }

    tmc::Boolean Lexer::TokenizeNumber (std::istream& pStream, tmc::Int32& pCharacter)
    {
        if (pCharacter == '0')
        {
            tmc::Int32 lNext = pStream.get();
            
            switch (lNext)
            {
                case 'x':
                case 'X':   return TokenizeHex(pStream, pCharacter);
                case 'b':
                case 'B':   return TokenizeBinary(pStream, pCharacter);
                case 'o':
                case 'O':   return TokenizeOctal(pStream, pCharacter);
                default:    pStream.unget(); break;
            }
        }

        tmc::String     lValue          = "";
        tmc::Boolean    lIsDecimal      = false;
        tmc::Boolean    lIsPlaceholder  = (pCharacter == '@');

        if (lIsPlaceholder == true)
        {
            pCharacter = pStream.get();
        }

        do
        {
            if (pCharacter == '.')
            {
                if (lIsDecimal == true || lIsPlaceholder == true)   { break; }
                else                                                { lIsDecimal = true; }
            }

            lValue      += static_cast<char>(pCharacter);
            pCharacter  = pStream.get();
        } while (pCharacter == '.' || std::isdigit(pCharacter));
        
        pStream.unget();
        
        return InsertToken(
            (lIsPlaceholder == true) ? TokenType::Placeholder : TokenType::Number, 
            lValue
        );
    }

    tmc::Boolean Lexer::TokenizeBinary (std::istream& pStream, tmc::Int32& pCharacter)
    {
        tmc::String     lValue      = "";

        pCharacter = pStream.get();

        while (pCharacter == '0' || pCharacter == '1')
        {
            lValue += static_cast<char>(pCharacter);
            pCharacter = pStream.get();
        }

        if (lValue.empty() == true)
        {
            std::cerr << "[Lexer] Expected binary string after '0b' literal." << std::endl;
            return false;
        }

        pStream.unget();

        return InsertToken(TokenType::Binary, lValue);
    }

    tmc::Boolean Lexer::TokenizeOctal (std::istream& pStream, tmc::Int32& pCharacter)
    {
        tmc::String     lValue      = "";
        
        pCharacter = pStream.get();

        while (pCharacter >= '0' || pCharacter <= '7')
        {
            lValue += static_cast<char>(pCharacter);
            pCharacter = pStream.get();
        }

        if (lValue.empty() == true)
        {
            std::cerr << "[Lexer] Expected octal string after '0o' literal." << std::endl;
            return false;
        }

        pStream.unget();

        return InsertToken(TokenType::Octal, lValue);
    }

    tmc::Boolean Lexer::TokenizeHex (std::istream& pStream, tmc::Int32& pCharacter)
    {
        tmc::String     lValue      = "";
        
        pCharacter = pStream.get();

        while (std::isxdigit(pCharacter))
        {
            lValue += static_cast<char>(pCharacter);
            pCharacter = pStream.get();
        }

        if (lValue.empty() == true)
        {
            std::cerr << "[Lexer] Expected hexadecimal string after '0x' literal." << std::endl;
            return false;
        }

        pStream.unget();

        return InsertToken(TokenType::Hexadecimal, lValue);
    }

    tmc::Boolean Lexer::TokenizeSymbol (std::istream& pStream, tmc::Int32& pCharacter)
    {
        switch (pCharacter)
        {
            case '=':
                switch (pStream.get())
                {
                    case '=':
                    {
                        switch (pStream.get())
                        {
                            case '=':   return InsertToken(TokenType::CompareStrictEquals);
                            default:    pStream.unget(); break;
                        }

                        return InsertToken(TokenType::CompareEquals);
                    } break;
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::AssignEquals);
            case '!':
                switch (pStream.get())
                {
                    case '=':
                    {
                        switch (pStream.get())
                        {
                            case '=':   return InsertToken(TokenType::CompareStrictNotEquals);
                            default:    pStream.unget(); break;
                        }

                        return InsertToken(TokenType::CompareNotEquals);
                    } break;
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::LogicalNot);
            case '+':
                switch (pStream.get())
                {
                    case '+':   return InsertToken(TokenType::Increment);
                    case '=':   return InsertToken(TokenType::AssignPlus);
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::Plus);
            case '-':
                switch (pStream.get())
                {
                    case '-':   return InsertToken(TokenType::Decrement);
                    case '=':   return InsertToken(TokenType::AssignMinus);
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::Minus);
            case '*':
                switch (pStream.get())
                {
                    case '=':   return InsertToken(TokenType::AssignTimes);
                    case '*':
                    {
                        switch (pStream.get())
                        {
                            case '=':   return InsertToken(TokenType::AssignExponent);
                            default:    pStream.unget(); break;
                        }

                        return InsertToken(TokenType::Exponent);
                    } break;
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::Times);
            case '/':
                switch (pStream.get())
                {
                    case '=':   return InsertToken(TokenType::AssignDivide);
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::Divide);
            case '%':
                switch (pStream.get())
                {
                    case '=':   return InsertToken(TokenType::AssignModulo);
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::Modulo);
            case '&':
                switch (pStream.get())
                {
                    case '=':   return InsertToken(TokenType::AssignBitwiseAnd);
                    case '&':   return InsertToken(TokenType::LogicalAnd);
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::BitwiseAnd);
            case '|':
                switch (pStream.get())
                {
                    case '=':   return InsertToken(TokenType::AssignBitwiseOr);
                    case '|':   return InsertToken(TokenType::LogicalOr);
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::BitwiseOr);
            case '^':
                switch (pStream.get())
                {
                    case '=':   return InsertToken(TokenType::AssignBitwiseXor);
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::BitwiseXor);
            case '>':
                switch (pStream.get())
                {
                    case '=':   return InsertToken(TokenType::CompareGreaterEquals);
                    case '>':
                    {
                        switch (pStream.get())
                        {
                            case '=':   return InsertToken(TokenType::AssignBitwiseRightShift);
                            default:    pStream.unget(); break;
                        }

                        return InsertToken(TokenType::BitwiseRightShift);
                    }
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::CompareGreater);
            case '<':
                switch (pStream.get())
                {
                    case '=':   return InsertToken(TokenType::CompareLessEquals);
                    case '<':
                    {
                        switch (pStream.get())
                        {
                            case '=':   return InsertToken(TokenType::AssignBitwiseLeftShift);
                            default:    pStream.unget(); break;
                        }

                        return InsertToken(TokenType::BitwiseLeftShift);
                    }
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::CompareLess);
            case '(':
                return InsertToken(TokenType::OpenParen);
            case ')':
                return InsertToken(TokenType::CloseParen);
            case '[':
                return InsertToken(TokenType::OpenBracket);
            case ']':
                return InsertToken(TokenType::CloseBracket);
            case '{':
                return InsertToken(TokenType::OpenBrace);
            case '}':
                return InsertToken(TokenType::CloseBrace);
            case ',':
                return InsertToken(TokenType::Comma);
            case ':':
                return InsertToken(TokenType::Colon);
            case '.':
                switch (pStream.get())
                {
                    case '.':   return InsertToken(TokenType::Concat);
                    default:    pStream.unget(); break;
                }

                return InsertToken(TokenType::Period);
            default:
                std::cerr   << "[Lexer] Unexpected character '" << static_cast<char>(pCharacter)
                            << "'." << std::endl;
                return false;
        }
    }

}
