/// @file TMM.Main.cpp

#include <TMM.Precompiled.hpp>
#include <TMM.Lexer.hpp>
#include <TMM.Parser.hpp>
#include <TMC.Arguments.hpp>

tmc::Int32 RunAssembler ()
{
    tmc::String     lInputFile  = tmc::Arguments::Get("input-file", 'i');
    tmc::String     lOutputFile = tmc::Arguments::Get("output-file", 'o');
    tmm::Lexer      lLexer;
    tmm::Parser     lParser;

    if (lInputFile.empty() == true)
    {
        std::cerr << "[RunAssembler] Missing parameter: --input-file, -i." << std::endl;
        return 1;
    }

    // if (lOutputFile.empty() == true)
    // {
    //     std::cerr << "[RunAssembler] Missing parameter: --output-file, -o." << std::endl;
    //     return 2;
    // }

    if (lLexer.TokenizeFile(lInputFile) == false)
    {
        return 3;
    }

    tmm::Program::Ptr lProgram = lParser.ParseProgram(lLexer);
    if (lProgram == nullptr)
    {
        return 4;
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