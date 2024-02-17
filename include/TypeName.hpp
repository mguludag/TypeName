/*
 * MIT License
 *
 * Copyright (c) 2023 Muhammed Galib Uludag
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <array>
#include <string_view>

namespace TypeUtils {
struct Type {
#if defined(_MSC_VER)
// NOLINTNEXTLINE [bugprone-reserved-identifier]
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
    template <typename T>
    static constexpr auto name() noexcept -> std::string_view {
        auto constexpr mangledName = std::string_view{__PRETTY_FUNCTION__};
        // endPos is static length according the compiler's name mangling
        // algorithm
        auto constexpr endPos = mangledName.size() - typeNameIndexes[0];
        // beginPos will start after last ':' character
        auto constexpr beginPos =
#if defined(_MSC_VER)
            mangledName.find(static_cast<char>(typeNameIndexes[1]),
                             typeNameIndexes[2]) +
#if defined(__clang__) && !defined(__GNUC__)
            2U;
#else
            1U;
#endif
#else
            mangledName.rfind(static_cast<char>(typeNameIndexes[1]), endPos) +
            1U + typeNameIndexes[2];
#endif

        auto constexpr resultSize = endPos - beginPos;
        auto constexpr resultName = mangledName.substr(beginPos, resultSize);
        // if type name in any scope return the name otherwise split name by
        // last '=' or first '<' character
        return resultName;
    }

    template <typename T>
    static constexpr auto name([[maybe_unused]] T type) noexcept
        -> std::string_view {
        return name<T>();
    }

   private:
    static constexpr std::array<unsigned, 3> typeNameIndexes
#if defined(__clang__)
        {1U, '=', 1U};
#elif defined(__GNUC__)
        {50U, '=', 1U};
#elif defined(_MSC_VER)
        {16U, '<', 65U};
#endif
};
}  // namespace TypeUtils
