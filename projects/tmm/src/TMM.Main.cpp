/// @file TMM.Main.cpp

#include <TMM.Precompiled.hpp>
#include <TMM.Lexer.hpp>
#include <TMC.Arguments.hpp>

tmc::Int32 RunAssembler ()
{
    tmc::String     lInputFile  = tmc::Arguments::Get("input-file", 'i');
    tmc::Boolean    lListTokens = tmc::Arguments::Has("list-tokens", 'l');
    tmm::Lexer      lLexer;

    if (lInputFile.empty() == true)
    {
        std::cerr << "[RunAssembler] Missing parameter: --input-file, -i." << std::endl;
        return 1;
    }

    if (lLexer.TokenizeFile(lInputFile) == false)
    {
        return 3;
    }

    if (lListTokens == true)
    {
        lLexer.ListTokens();
        return 0;
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