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
#include <fstream>
#include <filesystem>
#include <cstdint>
#include <cstddef>

#define TM_API

#define pure = 0

static_assert(std::is_same_v<std::size_t, std::uint64_t>);

namespace fs = std::filesystem;

namespace tmc
{

    using Int8      = std::int8_t;
    using Int16     = std::int16_t;
    using Int32     = std::int32_t;
    using Int64     = std::int64_t;

    using Uint8     = std::uint8_t;
    using Uint16    = std::uint16_t;
    using Uint32    = std::uint32_t;
    using Uint64    = std::uint64_t;

    using Float32   = float;
    using Float64   = double;

    using Byte      = Uint8;
    using Word      = Uint16;
    using Long      = Uint32;
    using Quad      = Uint64;
    using Address   = Uint32;

    using Boolean   = bool;
    using Char      = char;
    using String    = std::string;
    using Index     = std::size_t;

    using File      = std::fstream;
    using Path      = fs::path;

    template <typename T>               using Unique        = std::unique_ptr<T>;
    template <typename T>               using Shared        = std::shared_ptr<T>;
    template <typename T, Index N>      using Array         = std::array<T, N>;
    template <typename T>               using List          = std::vector<T>;
    template <typename T>               using UniqueList    = List<Unique<T>>;
    template <typename T>               using SharedList    = List<Shared<T>>;
    template <typename T, typename U>   using Map           = std::unordered_map<T, U>;
    template <typename T>               using Set           = std::unordered_set<T>;
    template <typename T>               using Dictionary    = Map<String, T>;
                                        using ByteBuffer    = List<Byte>;

    constexpr Index     NPOS                                = static_cast<Index>(-1);
    constexpr Address   ROM_START                           = 0x00000000;
    constexpr Address   PROGRAM_METADATA_START              = 0x00000000;
    constexpr Address   PROGRAM_METADATA_END                = 0x00000FFF;
    constexpr Address   RESTART_VECTOR_START                = 0x00001000;
    constexpr Address   RESTART_VECTOR_END                  = 0x00001FFF;
    constexpr Address   INTERRUPT_VECTOR_START              = 0x00002000;
    constexpr Address   INTERRUPT_VECTOR_END                = 0x00002FFF;
    constexpr Address   PROGRAM_START                       = 0x00003000;
    constexpr Address   PROGRAM_END                         = 0x7FFFFFFF;
    constexpr Address   ROM_END                             = 0x7FFFFFFF;
    constexpr Address   RAM_START                           = 0x80000000;
    constexpr Address   STACK_START                         = 0xFFFD0000;
    constexpr Address   STACK_END                           = 0xFFFDFFFF;
    constexpr Address   CALL_STACK_START                    = 0xFFFE0000;
    constexpr Address   CALL_STACK_END                      = 0xFFFEFFFF;
    constexpr Address   QRAM_START                          = 0xFFFF0000;
    constexpr Address   QRAM_END                            = 0xFFFFFFFF;
    constexpr Address   IO_START                            = 0xFFFFFF00;
    constexpr Address   IO_END                              = 0xFFFFFFFF;
    constexpr Address   RAM_END                             = 0xFFFFFFFF;

    template <typename T, typename... As>
    TM_API inline Unique<T> MakeUnique (As&&... pArgs)
    {
        return std::make_unique<T>(std::forward<As>(pArgs)...);
    }

    template <typename T, typename... As>
    TM_API inline Shared<T> MakeShared (As&&... pArgs)
    {
        return std::make_shared<T>(std::forward<As>(pArgs)...);
    }

}
