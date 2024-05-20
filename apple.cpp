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
  hai::array<stbi::pixel> data;
  ref<CGContextRef> ctx;
};
surface_t create_surface(unsigned w, unsigned h) {
  ref<CGColorSpaceRef> colour_space{
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB)};

  return surface_t{new surface{
                       .data{w * h},
                       .ctx = CGBitmapContextCreate(
                           data.begin(), w, h, 8, w * 4, *colour_space,
                           kCGImageAlphaPremultipliedLast),
                   },
                   [](auto x) { delete x; }};
};
} // namespace natty

void boosh(unsigned w, unsigned h, hai::array<stbi::pixel> &data) {
  auto font = natty::create_font("Helvetica", 48);
  auto surf = natty::create_surface(w, h);

  ref<CFAttributedStringRef> attr_str{
      CFAttributedStringCreate(nullptr, CFSTR("Olá!"), *(*font)->attrs)};

  ref<CTLineRef> line{CTLineCreateWithAttributedString(*attr_str)};

  // TODO: adjust position to be top-down
  CGContextSetTextPosition(*(*surf)->ctx, 10, 20);
  CTLineDraw(*line, *(*surf)->ctx);
}
