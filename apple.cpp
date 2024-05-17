#pragma leco add_framework CoreGraphics
#include <CoreGraphics/CoreGraphics.h>

void boosh() {
  void * data = NULL;
  unsigned w = 1024;
  unsigned h = 1024;

  CGColorSpaceRef color_space = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
  CGContextRef ctx = CGBitmapContextCreate(data, w, h, 8, w * 4, color_space, kCGImageAlphaPremultipliedLast);

  CGColorSpaceRelease(color_space);
}
