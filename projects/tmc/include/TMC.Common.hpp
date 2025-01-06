/// @file TMC.Common.hpp

#pragma once

#include <string>
#include <memory>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <initializer_list>
#include <functional>
#include <cstdint>
#include <cstddef>

#define TM_API

#define pure = 0

static_assert(std::is_same_v<std::size_t, std::uint64_t>);

namespace tmc
{
    using Int8          = std::int8_t;
    using Int16         = std::int16_t;
    using Int32         = std::int32_t;
    using Int64         = std::int64_t;
    using Uint8         = std::uint8_t;
    using Uint16        = std::uint16_t;
    using Uint32        = std::uint32_t;
    using Uint64        = std::uint64_t;
    using Float32       = float;
    using Float64       = double;
    using Index         = std::size_t;
    using Count         = std::size_t;
    using Size          = std::size_t;
    using Boolean       = bool;
    using Byte          = Uint8;
    using Char          = char;
    using String        = std::string;
    using StringView    = std::string_view;
    using Enum          = Int32;

    template <typename T>
    using Scope         = std::unique_ptr<T>;

    template <typename T>
    using Ref           = std::shared_ptr<T>;

    template <typename T, Count C>
    using Array         = std::array<T, C>;

    template <typename T>
    using List          = std::vector<T>;
    using ByteBuffer    = List<Byte>;

    template <typename T>
    using Set           = std::unordered_set<T>;

    template <typename T, typename U>
    using Map           = std::unordered_map<T, U>;

    template <typename T>
    using Dictionary    = Map<String, T>;

    template <typename T>
    using Queue         = std::queue<T>;

    template <typename T>
    using InitList      = std::initializer_list<T>;

    template <typename T, typename... Ps>
    using LVFunction    = std::function<T(Ps...)>;

    template <typename T, typename... Ps>
    using Function      = std::function<T(Ps&&...)>;

    constexpr Uint64 NPOS   = static_cast<Uint64>(-1);
    constexpr Uint32 NPOS32 = static_cast<Uint32>(-1);
    constexpr Uint16 NPOS16 = static_cast<Uint16>(-1);
    constexpr Uint8  NPOS8  = static_cast<Uint8>(-1);

    template <typename T, typename... As>
    inline Scope<T> MakeScope (As&&... pArgs)
    {
        return std::make_unique<T>(std::forward<As>(pArgs)...);
    }

    template <typename T, typename... As>
    inline Ref<T> MakeRef (As&&... pArgs)
    {
        return std::make_shared<T>(std::forward<As>(pArgs)...);
    }
}
