#pragma leco add_framework CoreGraphics
#include <CoreGraphics/CoreGraphics.h>

import hai;

namespace {
struct deleter {
  void operator()(CGColorSpaceRef r) { CGColorSpaceRelease(r); }
};
} // namespace

void boosh() {
  void * data = NULL;
  unsigned w = 1024;
  unsigned h = 1024;

  hai::value_holder<CGColorSpaceRef, deleter> color_space{
      CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB)};

  CGContextRef ctx = CGBitmapContextCreate(data, w, h, 8, w * 4, *color_space,
                                           kCGImageAlphaPremultipliedLast);
}
