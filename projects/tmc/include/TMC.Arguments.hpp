/// @file TMC.Arguments.hpp

#pragma once

#include <TMC.Common.hpp>

namespace tmc
{

    class TM_API Arguments
    {
    public:
        static void Capture (Int32 pArgCount, Char** pArgVector);
        static Boolean Has (const String& pKey, const Char& pShort);
        static const String& Get (const String& pKey, const Char& pShort, const String& pDefault = "");
        static const String& Get (const String& pKey, const Char& pShort, const Index& pIndex, const String& pDefault = "");

    private:
        static void InsertArgument (const String& pKey, const String& pValue);

    private:
        static Dictionary<List<String>> sArguments;

    };

}
