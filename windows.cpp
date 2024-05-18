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
  ref<HBITMAP> bmp{CreateCompatibleBitmap(*dc, w, h)};
  ref<HFONT> font{CreateFont(48, 0, 0, 0, FW_DONTCARE, false, false, false,
                             ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH, "Helvetica")};

  RECT rect{
      .left = 0,
      .top = 0,
      .right = static_cast<long>(w),
      .bottom = static_cast<long>(h),
  };

  SelectObject(*dc, *bmp);
  SelectObject(*dc, *font);
  SetBkColor(*dc, RGB(0, 0, 0));
  SetTextColor(*dc, RGB(255, 255, 255));
  DrawText(*dc, "Olá!", -1, &rect, DT_SINGLELINE);

  BITMAPINFOHEADER bmi{};
  bmi.biSize = sizeof(BITMAPINFOHEADER);
  bmi.biWidth = w;
  bmi.biHeight = h;
  bmi.biPlanes = 1;
  bmi.biBitCount = 32;
  bmi.biCompression = BI_RGB;

  hai::array<stbi::pixel> d{w * h};
  GetDIBits(*dc, *bmp, 0, h, d.begin(), reinterpret_cast<BITMAPINFO *>(&bmi),
            DIB_RGB_COLORS);

  for (auto y = 0; y < h; y++) {
    auto *rf = &d[y * w];
    auto *rt = &data[(h - y - 1) * w];
    for (auto x = 0; x < w; x++) {
      rt[x] = rf[x];
      rt[x].a = rt[x].r;
    }
  }
}

void boosh() {
  unsigned w = 1024;
  unsigned h = 1024;
  hai::array<stbi::pixel> data{w * h};
  boosh(w, h, data);

  stbi::write_rgba("out/test.png", w, h, data);
}
