/// @file TMM.Main.cpp

#include <TMM.Precompiled.hpp>
#include <TMC.Arguments.hpp>

int main (int pArgCount, char** pArgVector)
{
    // Capture command-line arguments.
    tmc::Arguments::Capture(pArgCount, pArgVector);

    return 0;
}