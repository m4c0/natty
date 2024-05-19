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
  return font_t{static_cast<font *>(res), [](auto x) { DeleteObject(x); }};
}

struct surface {
  ref<HDC> dc;
  ref<HBITMAP> bmp;
  RECT rect;
};
surface_t create_surface(unsigned w, unsigned h) {
  auto s = new surface{
      .dc{CreateCompatibleDC(GetDC(nullptr))},
      .bmp{CreateCompatibleBitmap(GetDC(nullptr), w, h)},
      .rect{
          .left = 0,
          .top = 0,
          .right = static_cast<long>(w),
          .bottom = static_cast<long>(h),
      },
  };
  SelectObject(*(s->dc), *(s->bmp));
  SetBkColor(*(s->dc), RGB(0, 0, 0));
  SetTextColor(*(s->dc), RGB(255, 255, 255));
  return surface_t{s, [](auto x) { delete x; }};
}
} // namespace natty

void boosh(unsigned w, unsigned h, hai::array<stbi::pixel> &data) {
  auto font = natty::create_font("Helvetica", 48);
  auto surf = natty::create_surface(w, h);

  constexpr const auto buf_size = 1024;
  wchar_t buf[buf_size];
  MultiByteToWideChar(CP_UTF8, 0, "Olá!", -1, buf, buf_size);

  ref<HDC> &dc = (*surf)->dc;
  ref<HBITMAP> &bmp = (*surf)->bmp;
  RECT rect = (*surf)->rect;

  SelectObject(*dc, *font);
  DrawTextW(*dc, buf, -1, &rect, DT_SINGLELINE);

  // TODO: Set position

  BITMAPINFOHEADER bmi{};
  bmi.biSize = sizeof(BITMAPINFOHEADER);
  bmi.biWidth = w;
  bmi.biHeight = -h;
  bmi.biPlanes = 1;
  bmi.biBitCount = 32;
  bmi.biCompression = BI_RGB;

  GetDIBits(*dc, *bmp, 0, h, data.begin(), reinterpret_cast<BITMAPINFO *>(&bmi),
            DIB_RGB_COLORS);

  for (auto &r : data)
    r.a = 255;
}
