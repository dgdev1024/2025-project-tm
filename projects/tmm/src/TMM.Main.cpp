/// @file TMM.Main.cpp

#include <TMM.Precompiled.hpp>
#include <TMM.Interpreter.hpp>
#include <TMC.Arguments.hpp>

tmc::Int32 RunAssembler ()
{
    tmc::String         lInputFile  = tmc::Arguments::Get("input-file", 'i');
    tmc::String         lOutputFile = tmc::Arguments::Get("output-file", 'o');
    tmc::Boolean        lLexOnly    = tmc::Arguments::Has("lex-only", 'l');
    tmm::Lexer          lLexer;
    tmm::Parser         lParser;
    tmm::Object         lObject;
    tmm::Interpreter    lInterpreter { lLexer, lParser, lObject };

    if (lInputFile.empty() == true)
    {
        std::cerr << "[RunAssembler] Missing parameter: --input-file, -i." << std::endl;
        return 1;
    }

    if (lLexer.TokenizeFile(lInputFile) == false)
    {
        return 2;
    }

    if (lLexOnly == true)
    {
        lLexer.ListTokens();
        return 0;
    }

    tmm::Program::Ptr lProgram = lParser.ParseProgram(lLexer);
    if (lProgram == nullptr)
    {
        return 4;
    }

    if (lInterpreter.Run(lProgram) == false)
    {
        return 5;
    }

    return 0;
}

int main (int pArgCount, char** pArgVector)
{
    // Capture command-line arguments.
    tmc::Arguments::Capture(pArgCount, pArgVector);
    
    if (tmc::Arguments::Has("assemble", 'a') == true)
    {
        return RunAssembler();
    }

    return 0;
}