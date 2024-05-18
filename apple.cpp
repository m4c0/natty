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

void boosh(auto &data, auto w, auto h) {
  ref<CTFontRef> font{CTFontCreateWithName(CFSTR("Helvetica"), 48, nullptr)};

  ref<CGColorSpaceRef> colour_space{
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB)};

  ref<CGContextRef> ctx{CGBitmapContextCreate(data.begin(), w, h, 8, w * 4,
                                              *colour_space,
                                              kCGImageAlphaPremultipliedLast)};

  ref<CFMutableDictionaryRef> attrs{
      CFDictionaryCreateMutable(nullptr, 2, &kCFTypeDictionaryKeyCallBacks,
                                &kCFTypeDictionaryValueCallBacks)};
  CFDictionaryAddValue(*attrs, kCTFontAttributeName, *font);
  CFDictionaryAddValue(*attrs, kCTForegroundColorAttributeName,
                       CGColorGetConstantColor(kCGColorWhite));

  ref<CFAttributedStringRef> attr_str{
      CFAttributedStringCreate(nullptr, CFSTR("Olá!"), *attrs)};

  ref<CTLineRef> line{CTLineCreateWithAttributedString(*attr_str)};

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
