/// @file TMM.RuntimeValue.hpp

#pragma once

#include <TMM.Common.hpp>

namespace tmm
{

    /* Runtime Value Type Enumeration *************************************************************/

    enum class RuntimeValueType
    {
        Void
    };

    /* Runtime Value Base Class *******************************************************************/

    class RuntimeValue
    {
    public:
        using Ptr = tmc::Shared<RuntimeValue>;

    protected:
        inline RuntimeValue (const RuntimeValueType& pValueType) :
            mValueType  { pValueType }
        {}

    public:

        template <typename T, typename... As>
        inline static tmc::Shared<T> Make (As&&... pArgs)
        {
            static_assert(std::is_base_of_v<RuntimeValue, T>, "'T' must derive from 'tmm::RuntimeValue'.");
            return tmc::MakeShared<T>(std::forward<As>(pArgs)...);
        }

        template <typename T>
        inline static tmc::Shared<T> Cast (const RuntimeValue::Ptr& pValue)
        {
            static_assert(std::is_base_of_v<RuntimeValue, T>, "'T' must derive from 'tmm::RuntimeValue'.");
            return std::static_pointer_cast<T>(pValue);
        }

    public:
        inline const RuntimeValueType& GetValueType () const { return mValueType; }

    protected:
        RuntimeValueType mValueType;

    };

    /* Void Value Class ***************************************************************************/

    class VoidValue : public RuntimeValue
    {
    public:
        using Ptr = tmc::Shared<VoidValue>;

    public:
        inline VoidValue () :
            RuntimeValue    { RuntimeValueType::Void }
        {}

    };

}
