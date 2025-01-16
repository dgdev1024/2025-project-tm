/// @file TMM.Environment.cpp

#include <TMM.Precompiled.hpp>
#include <TMM.Environment.hpp>

namespace tmm
{

    /* Public Constructors and Destructor *********************************************************/
    
    Environment::Environment () :
        mParent { *this }
    {

    }

    Environment::Environment (Environment& pParent, const EnvironmentScope& pScope) :
        mParent { pParent },
        mScope  { pScope }
    {

    }

}
