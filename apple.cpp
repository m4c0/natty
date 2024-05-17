#pragma leco add_framework CoreFoundation
#pragma leco add_framework CoreGraphics
#pragma leco add_framework CoreText

#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

import hai;

namespace {
struct deleter {
  void operator()(CGColorSpaceRef r) { CGColorSpaceRelease(r); }
  void operator()(CGContextRef r) { CGContextRelease(r); }
  void operator()(CFTypeRef r) { CFRelease(r); }
};
template <typename T> using ref = hai::value_holder<T, deleter>;
} // namespace

void boosh() {
  void *data = NULL;
  unsigned w = 1024;
  unsigned h = 1024;

  ref<CTFontRef> font{CTFontCreateWithName(CFSTR("Helvetica"), 12, nullptr)};

  ref<CGColorSpaceRef> colour_space{
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB)};

  ref<CGContextRef> ctx{CGBitmapContextCreate(
      data, w, h, 8, w * 4, *colour_space, kCGImageAlphaPremultipliedLast)};

  ref<CFMutableDictionaryRef> attrs{
      CFDictionaryCreateMutable(nullptr, 1, &kCFTypeDictionaryKeyCallBacks,
                                &kCFTypeDictionaryValueCallBacks)};
  CFDictionaryAddValue(*attrs, kCTFontAttributeName, *font);

  ref<CFAttributedStringRef> attr_str{
      CFAttributedStringCreate(nullptr, CFSTR("Olá!"), *attrs)};
}
