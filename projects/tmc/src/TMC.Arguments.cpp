/// @file TMC.Arguments.cpp

#include <TMC.Precompiled.hpp>
#include <TMC.Arguments.hpp>

namespace tmc
{

    /* Static Members *****************************************************************************/

    Dictionary<List<String>> Arguments::sArguments;

    /* Public Methods *****************************************************************************/

    void Arguments::Capture (Int32 pArgCount, Char** pArgVector)
    {
        for (Int32 lIndex = 0; lIndex < pArgCount; ++lIndex)
        {
            String lArgument = pArgVector[lIndex];
            if (lArgument.starts_with("--") && lArgument.size() > 2)
            {
                String  lKey = "";
                String  lValue = "";
                Index   lEqualsPos = lArgument.find('=');

                if (lEqualsPos != String::npos)
                {
                    lKey    = lArgument.substr(2, lEqualsPos - 2);
                    lValue  = lArgument.substr(lEqualsPos + 1);
                    InsertArgument(lKey, lValue);
                } 
                else if (lIndex + 1 < pArgCount && pArgVector[lIndex + 1][0] != '-')
                {
                    lKey    = lArgument.substr(2);

                    do
                    {
                        InsertArgument(lKey, pArgVector[++lIndex]);
                    } while (lIndex + 1 < pArgCount && pArgVector[lIndex + 1][0] != '-');
                }
                else
                {
                    lKey    = lArgument.substr(2);
                    InsertArgument(lKey, "");
                }
            }
            else if (lArgument.starts_with('-') && lArgument.size() == 2)
            {
                String  lKey = "";
                String  lValue = "";

                if (lIndex + 1 < pArgCount && pArgVector[lIndex + 1][0] != '-')
                {
                    lKey    = lArgument.substr(1);

                    do
                    {
                        InsertArgument(lKey, pArgVector[++lIndex]);
                    } while (lIndex + 1 < pArgCount && pArgVector[lIndex + 1][0] != '-');
                }
                else
                {
                    lKey    = lArgument.substr(1);
                    InsertArgument(lKey, "");
                }
            }
        }
    }

    Boolean Arguments::Has (const String& pKey, const Char& pShort)
    {
        return sArguments.contains(pKey) || sArguments.contains(String { pShort });
    }

    const String& Arguments::Get (const String& pKey, const Char& pShort, const String& pDefault)
    {
        auto lIter = sArguments.find(pKey);
        if (lIter != sArguments.end() && lIter->second.empty() == false)
        {
            return lIter->second.at(0);
        }
        
        auto lShortIter = sArguments.find(String { pShort });
        if (lShortIter != sArguments.end() && lShortIter->second.empty() == false)
        {
            return lShortIter->second.at(0);
        }

        return pDefault;
    }

    const String& Arguments::Get (const String& pKey, const Char& pShort, const Index& pIndex, const String& pDefault)
    {
        auto lIter = sArguments.find(pKey);
        if (lIter != sArguments.end() && lIter->second.size() < pIndex)
        {
            return lIter->second.at(pIndex);
        }

        auto lShortIter = sArguments.find(String { pShort });
        if (lShortIter != sArguments.end() && lShortIter->second.size() < pIndex)
        {
            return lShortIter->second.at(pIndex);
        }

        return pDefault;
    }

    /* Private Methods ****************************************************************************/

    void Arguments::InsertArgument (const String& pKey, const String& pValue)
    {
        auto lIter = sArguments.find(pKey);
        if (lIter != sArguments.end())
        {
            lIter->second.push_back(pValue);
        }
        else
        {
            sArguments[pKey] = {};
            sArguments[pKey].push_back(pValue);
        }
    }

}
