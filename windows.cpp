#pragma leco add_library GDI32 USER32

#define WIN32_MEAN_AND_LEAN
#include <windows.h>

import hai;

namespace {
struct deleter {
  void operator()(HDC hdc) { DeleteDC(hdc); }
};

template <typename T> using ref = hai::value_holder<T, deleter>;
} // namespace

void boosh() { ref<HDC> dc{CreateCompatibleDC(GetDC(nullptr))}; }
