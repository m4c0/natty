#pragma leco add_framework CoreFoundation
#pragma leco add_framework CoreGraphics
#pragma leco add_framework CoreText
module;

#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

module natty;
import hai;
import stubby;
import traits;

namespace {
struct deleter {
  void operator()(CGColorSpaceRef r) { CGColorSpaceRelease(r); }
  void operator()(CGContextRef r) { CGContextRelease(r); }
  void operator()(CFTypeRef r) { CFRelease(r); }
};
template <typename T> using ref = hai::value_holder<T, deleter>;
} // namespace

namespace natty {
struct font {
  ref<CTFontRef> font;
  ref<CFMutableDictionaryRef> attrs;
};
font_t create_font(const char *name, unsigned size) {
  auto f = new font{
      .font{CTFontCreateWithName(CFSTR("Helvetica"), 48, nullptr)},
      .attrs{CFDictionaryCreateMutable(nullptr, 2,
                                       &kCFTypeDictionaryKeyCallBacks,
                                       &kCFTypeDictionaryValueCallBacks)},
  };

  CFDictionaryAddValue(*f->attrs, kCTFontAttributeName, *f->font);
  CFDictionaryAddValue(*f->attrs, kCTForegroundColorAttributeName,
                       CGColorGetConstantColor(kCGColorWhite));
  return font_t{f, [](auto x) { delete x; }};
}

struct surface {
  ref<CGContextRef> ctx;
  ref<CTLineRef> line{};
  hai::array<unsigned> data;
};
surface_t create_surface(unsigned w, unsigned h) {
  ref<CGColorSpaceRef> colour_space{
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB)};

  hai::array<unsigned> data{w * h};

  return surface_t{new surface{
                       .ctx{CGBitmapContextCreate(
                           data.begin(), w, h, 8, w * 4, *colour_space,
                           kCGImageAlphaPremultipliedLast)},
                       .data{traits::move(data)},
                   },
                   [](auto x) { delete x; }};
};

void surface_font(surface *s, font *f) {
  ref<CFAttributedStringRef> attr_str{
      CFAttributedStringCreate(nullptr, CFSTR("Olá!"), *(*f).attrs)};

  s->line = ref<CTLineRef>{CTLineCreateWithAttributedString(*attr_str)};
}

void surface_position(surface *s, int x, int y) {
  auto &ctx = s->ctx;
  CGContextSetTextPosition(*ctx, x, y);
}

void draw(natty::surface *surf, jute::view str) {
  auto &ctx = surf->ctx;
  auto &line = surf->line;

  // TODO: adjust position to be top-down
  CTLineDraw(*line, *ctx);
}

const hai::array<unsigned> &surface_data(surface *s) { return s->data; }
} // namespace natty
