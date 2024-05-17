#pragma leco add_framework CoreGraphics
#pragma leco add_framework Foundation
#include <CoreGraphics/CoreGraphics.h>

import hai;

namespace {
struct error {};

struct deleter {
  void operator()(CGColorSpaceRef r) { CGColorSpaceRelease(r); }
  void operator()(CGContextRef r) { CGContextRelease(r); }
  void operator()(CGFontRef r) { CGFontRelease(r); }
};
} // namespace

void boosh() {
  void *data = NULL;
  unsigned w = 1024;
  unsigned h = 1024;

  hai::value_holder<CGFontRef, deleter> font{
      CGFontCreateWithFontName(CFSTR("Helvetica"))};
  if (!*font)
    throw error{};

  hai::value_holder<CGColorSpaceRef, deleter> colour_space{
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB)};
  if (!*colour_space)
    throw error{};

  hai::value_holder<CGContextRef, deleter> ctx{CGBitmapContextCreate(
      data, w, h, 8, w * 4, *colour_space, kCGImageAlphaPremultipliedLast)};
  if (!*ctx)
    throw error{};
}
