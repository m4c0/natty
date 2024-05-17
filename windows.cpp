#pragma leco add_library GDI32 USER32

#define WIN32_MEAN_AND_LEAN
#include <windows.h>

import hai;
import stubby;

namespace {
struct deleter {
  void operator()(HDC h) { DeleteDC(h); }
  void operator()(HGDIOBJ h) { DeleteObject(h); }
};

template <typename T> using ref = hai::value_holder<T, deleter>;
} // namespace

void boosh(unsigned w, unsigned h, auto &data) {
  ref<HDC> dc{CreateCompatibleDC(GetDC(nullptr))};
  ref<HBITMAP> bmp{CreateBitmap(w, h, 1, 32, data.begin())};
  SelectObject(*dc, *bmp);

  RECT rect{
      .left = 0,
      .top = 0,
      .right = static_cast<long>(w),
      .bottom = static_cast<long>(h),
  };
  DrawText(*dc, "Olá!", -1, &rect, DT_SINGLELINE);
}

void boosh() {
  unsigned w = 1024;
  unsigned h = 1024;
  hai::array<stbi::pixel> data{w * h};
  boosh(w, h, data);
}
