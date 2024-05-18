#pragma leco add_library GDI32 USER32
module;

#define WIN32_MEAN_AND_LEAN
#include <windows.h>

module natty;
import hai;
import stubby;

namespace {
struct deleter {
  void operator()(HDC h) { DeleteDC(h); }
  void operator()(HGDIOBJ h) { DeleteObject(h); }
};

template <typename T> using ref = hai::value_holder<T, deleter>;
} // namespace

namespace natty {
struct font : HFONT__ {};
font_t create_font(const char *name, unsigned size){
  auto res = CreateFont(size, 0, 0, 0, FW_DONTCARE, false, false, false,
                        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH, name);
  return hai::pimpl<font *>{static_cast<font *>(res),
                            [](auto x) { DeleteObject(x); }};
}
} // namespace natty

void boosh(unsigned w, unsigned h, auto &data) {
  ref<HFONT> font{CreateFont(48, 0, 0, 0, FW_DONTCARE, false, false, false,
                             ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH, "Helvetica")};

  constexpr const auto buf_size = 1024;
  wchar_t buf[buf_size];
  MultiByteToWideChar(CP_UTF8, 0, "Olá!", -1, buf, buf_size);

  ref<HDC> dc{CreateCompatibleDC(GetDC(nullptr))};
  ref<HBITMAP> bmp{CreateCompatibleBitmap(GetDC(nullptr), w, h)};

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
  DrawTextW(*dc, buf, -1, &rect, DT_SINGLELINE);

  // TODO: Set position

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
      rt[x].a = 255;
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
