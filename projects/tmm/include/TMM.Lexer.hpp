/// @file TMM.Lexer.hpp

#pragma once

#include <TMM.Token.hpp>

namespace tmm
{

    class Lexer
    {
    public:
        Lexer ();
        ~Lexer ();

    public:
        void                ListTokens () const;
        tmc::Boolean        HasMoreTokens () const;
        const Token&        TokenAt (const tmc::Index& pIndex = 0) const;
        Token               DiscardToken ();
        tmc::Boolean        DiscardNewLine ();
        tmc::Boolean        DiscardTokenIf (const TokenType& pType);
        tmc::Boolean        TokenizeFile (const tmc::Path& pPath);
        tmc::Boolean        TokenizeStream (std::istream& pStream);

    private:
        tmc::Boolean        InsertToken (const TokenType& pType, const tmc::String& pValue = "");
        tmc::Boolean        TokenizeIdentifier (std::istream& pStream, tmc::Int32& pCharacter);
        tmc::Boolean        TokenizeChar (std::istream& pStream, tmc::Int32& pCharacter);
        tmc::Boolean        TokenizeString (std::istream& pStream, tmc::Int32& pCharacter);
        tmc::Boolean        TokenizeNumber (std::istream& pStream, tmc::Int32& pCharacter);
        tmc::Boolean        TokenizeBinary (std::istream& pStream, tmc::Int32& pCharacter);
        tmc::Boolean        TokenizeOctal (std::istream& pStream, tmc::Int32& pCharacter);
        tmc::Boolean        TokenizeHex (std::istream& pStream, tmc::Int32& pCharacter);
        tmc::Boolean        TokenizeSymbol (std::istream& pStream, tmc::Int32& pCharacter);
    
    private:
        tmc::List<Token>    mTokens;
        tmc::Index          mTokenPointer = 0;
        tmc::Path           mCurrentFile = "";
        tmc::Index          mCurrentLine = 0;
        tmc::Set<tmc::Path> mLexedPaths;

    };

}
