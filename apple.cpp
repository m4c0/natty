#pragma leco add_framework CoreFoundation
#pragma leco add_framework CoreGraphics
#pragma leco add_framework CoreText
module;

#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

module natty;
import hai;
import stubby;

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
  ref<CTFontRef> fnt;
  ref<CFMutableDictionaryRef> attrs;
};
font_t create_font(const char *name, unsigned size) {
  auto f = new font {
    .font{CTFontCreateWithName(CFSTR("Helvetica"), 48, nullptr)};

    .attrs{CFDictionaryCreateMutable(nullptr, 2, &kCFTypeDictionaryKeyCallBacks,
                                     &kCFTypeDictionaryValueCallBacks)};
  };

  CFDictionaryAddValue(*f->attrs, kCTFontAttributeName, *f->font);
  CFDictionaryAddValue(*f->attrs, kCTForegroundColorAttributeName,
                       CGColorGetConstantColor(kCGColorWhite));
  return font_t{f, [](auto x) { delete x; }};
}

struct surface {
  ref<CGContextRef> ctx;
  hai::array<stbi::pixel> data;
};
surface_t create_surface(unsigned w, unsigned h) {
  ref<CGColorSpaceRef> colour_space{
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB)};

  return surface_t{new surface{
                       .ctx = CGBitmapContextCreate(
                           data.begin(), w, h, 8, w * 4, *colour_space,
                           kCGImageAlphaPremultipliedLast),
                       .data{w * h},
                   },
                   [](auto x) { delete x; }};
};
} // namespace natty

void boosh(auto &data, auto w, auto h) {
  auto font = natty::create_font("Helvetica", 48);
  auto surf = natty::create_surface(w, h);

  ref<CFAttributedStringRef> attr_str{
      CFAttributedStringCreate(nullptr, CFSTR("Olá!"), *attrs)};

  ref<CTLineRef> line{CTLineCreateWithAttributedString(*attr_str)};

  // TODO: adjust position to be top-down
  CGContextSetTextPosition(*ctx, 10, 20);
  CTLineDraw(*line, *ctx);
}

void boosh() {
  unsigned w = 1024;
  unsigned h = 1024;
  hai::array<stbi::pixel> data{w * h};
  boosh(data, w, h);

  stbi::write_rgba("out/test.png", w, h, data);
}
