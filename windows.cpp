#pragma leco add_library GDI32 USER32
module;

#define WIN32_MEAN_AND_LEAN
#include <windows.h>

module natty;
import dotz;
import hai;
import hay;

namespace {
  using hbitmap = hay<HBITMAP, nullptr, DeleteObject>;
  using hdc     = hay<HDC,     nullptr, DeleteDC>;
}

namespace natty {
  struct font : HFONT__ {};
  font_t create_font(const char *name, unsigned size){
    auto res = CreateFont(size, 0, 0, 0, FW_DONTCARE, false, false, false,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, name);
    return font_t { static_cast<font *>(res), [](auto x) { DeleteObject(x); } };
  }

  struct surface {
    hdc dc;
    hbitmap bmp;
    dotz::ivec2 size;
    hai::array<unsigned> data;
  };
  surface_t create_surface(unsigned w, unsigned h) {
    auto s = new surface {
      .dc { CreateCompatibleDC(GetDC(nullptr)) },
      .bmp { CreateCompatibleBitmap(GetDC(nullptr), w, h) },
      .size { w, h },
      .data { w * h },
    };
    SelectObject(s->dc, static_cast<HBITMAP>(s->bmp));
    SetBkColor(s->dc, RGB(0, 0, 0));
    SetTextColor(s->dc, RGB(255, 255, 255));
    return surface_t{s, [](auto x) { delete x; }};
  }

  void draw(const draw_params & p) {
    HDC dc = (*p.surface)->dc;
    SelectObject(dc, *p.font);

    auto [w, h] = (*p.surface)->size;
    RECT rect {
      .left = p.position.x,
      .top = p.position.y,
      .right = w,
      .bottom = h,
    };

    hai::array<wchar_t> buf { static_cast<unsigned>(p.text.size()) };
    MultiByteToWideChar(CP_UTF8, 0, p.text.data(), p.text.size(), buf.begin(), buf.size());

    DrawTextW(dc, buf.begin(), buf.size(), &rect, DT_SINGLELINE);
  }

  const hai::array<unsigned> & surface_data(surface * s) {
    BITMAPINFOHEADER bmi{};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biWidth = s->size.x;
    bmi.biHeight = -s->size.y; // Secret API trick: negative flips Y
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biCompression = BI_RGB;

    GetDIBits(s->dc, s->bmp, 0, s->size.y, s->data.begin(),
        reinterpret_cast<BITMAPINFO *>(&bmi), DIB_RGB_COLORS);

    for (auto &pix : s->data) pix |= 0xFFU << 24; // add alpha channel
    return s->data;
  }
} // namespace natty
