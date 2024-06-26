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
  RECT stencil;
  hai::array<unsigned> data;
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
      .data{w * h},
  };
  SelectObject(*(s->dc), *(s->bmp));
  SetBkColor(*(s->dc), RGB(0, 0, 0));
  SetTextColor(*(s->dc), RGB(255, 255, 255));
  return surface_t{s, [](auto x) { delete x; }};
}

void surface_font(surface *s, font *f) {
  ref<HDC> &dc = s->dc;
  SelectObject(*dc, f);
}

void surface_position(surface *s, int x, int y) {
  RECT rect = s->rect;
  rect.top = x;
  rect.left = y;
  s->stencil = rect;
}

void draw(natty::surface *surf, jute::view str) {
  ref<HDC> &dc = surf->dc;
  RECT rect = surf->stencil;

  hai::array<wchar_t> buf{static_cast<unsigned>(str.size())};
  MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), buf.begin(),
                      buf.size());

  DrawTextW(*dc, buf.begin(), buf.size(), &rect, DT_SINGLELINE);
}

const hai::array<unsigned> &surface_data(surface *s) {
  ref<HDC> &dc = s->dc;
  ref<HBITMAP> &bmp = s->bmp;

  auto w = s->rect.right - s->rect.left;
  auto h = s->rect.bottom - s->rect.top;

  BITMAPINFOHEADER bmi{};
  bmi.biSize = sizeof(BITMAPINFOHEADER);
  bmi.biWidth = w;
  bmi.biHeight = -h; // Secret API trick: negative flips Y
  bmi.biPlanes = 1;
  bmi.biBitCount = 32;
  bmi.biCompression = BI_RGB;

  GetDIBits(*dc, *bmp, 0, h, s->data.begin(),
            reinterpret_cast<BITMAPINFO *>(&bmi), DIB_RGB_COLORS);

  for (auto &pix : s->data) {
    pix |= 0xFFU << 24; // add alpha channel
  }
  return s->data;
}
} // namespace natty
