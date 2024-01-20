# TypeName
A small C++17 utility for get type names constexpr from compiler


## Usage
```C++
#include "TypeName.hpp"
#include <optional>
#include <print>

// create variadic template function 
template <typename... Args>
auto printTypes(Args&&... args) -> void {
    // NOLINTNEXTLINE [readability-identifier-length]
    auto c = 0U;
    std::print("types are: "),
        // NOLINTNEXTLINE [cppcoreguidelines-pro-bounds-array-to-pointer-decay]
        (std::print("{}{}", Type::name(std::forward<Args>(args)),
                    (++c == sizeof...(args) ? "\n" : ", ")),
         ...);  // NOLINT
}

// you can able to specialize TypeUtils::Type::name<> for desired output per type
template <>
auto constexpr TypeUtils::Type::name<std::string_view>() noexcept
    -> std::string_view {
    return "std::string_view";
}

auto main() -> int
{
  printTypes(5, 5.5, "aaa", 'a', TypeUtils::Type{}, name, std::nullopt);
  // prints: "types are: int, double, const char*, char, TypeUtils::Type, std::string_view, std::nullopt_t"
}
````
