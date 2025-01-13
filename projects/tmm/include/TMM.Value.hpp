/// @file TMM.Value.hpp

#pragma once

#include <TMM.Common.hpp>

namespace tmm
{

    /* Value Type Enumeration *********************************************************************/

    enum class ValueType
    {
        Void,
        Number
    };

    /* Runtime Value Base Class *******************************************************************/

    class RuntimeValue
    {
    protected:
        inline RuntimeValue (const ValueType& pType) :
            mType   { pType }
        {}

    public:
        inline const ValueType& GetType () const { return mType; }

    protected:
        ValueType mType;

    };

    /* Void Value *********************************************************************************/

    class VoidValue : public RuntimeValue
    {
    public:
        inline VoidValue () :
            RuntimeValue { ValueType::Void }
        {}

    };

    /* Number Value *******************************************************************************/

    class NumberValue : public RuntimeValue
    {
    public:
        inline NumberValue (const tmc::Float64& pValue) :
            RuntimeValue    { ValueType::Number },
            mValue          { pValue }
        {
            SplitNumber();
        }

    public:
        inline const tmc::Float64&  GetValue () const       { return mValue; }
        inline const tmc::Uint32&   GetInteger () const     { return mInteger; }
        inline const tmc::Uint32&   GetFractional () const  { return mFractional; }

    private:

        inline void SplitNumber ()
        {
            tmc::Float64 lInteger = 0.0;
            tmc::Float64 lFractional = std::modf(mValue, &lInteger);

            mInteger    = static_cast<tmc::Uint32>(lInteger);
            mFractional = static_cast<tmc::Uint32>(lFractional * std::numeric_limits<tmc::Uint32>::max());
        }

    private:
        tmc::Float64    mValue      = 0.0;
        tmc::Uint32     mInteger    = 0;
        tmc::Uint32     mFractional = 0;

    };

}
