/// @file TMM.Environment.hpp

#pragma once

#include <TMM.RuntimeValue.hpp>

namespace tmm
{

    enum class EnvironmentScope
    {
        Parent
    };

    class Environment
    {
    public:
        Environment ();
        Environment (Environment& pParent, const EnvironmentScope& pScope);

    private:
        Environment&        mParent;
        EnvironmentScope    mScope = EnvironmentScope::Parent;

    };

}
