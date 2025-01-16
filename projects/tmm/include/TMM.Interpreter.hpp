/// @file TMM.Interpreter.hpp

#pragma once

#include <TMM.Lexer.hpp>
#include <TMM.Parser.hpp>
#include <TMM.Object.hpp>
#include <TMM.Environment.hpp>

namespace tmm
{

    class Interpreter
    {
    public:
        Interpreter (Lexer& pLexer, Parser& pParser, Object& pObject);

    public:
        tmc::Boolean Run (const Program::Ptr& pProgram);

    private:
        RuntimeValue::Ptr Evaluate (const Statement::Ptr& pStatement);

    private:
        Lexer&      mLexer;
        Parser&     mParser;
        Object&     mObject;
        Environment mEnvironment;

    };

}
