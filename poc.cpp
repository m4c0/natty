#pragma leco tool

import hai;
import natty;
import stubby;

int main() {
  auto font = natty::create_font("Impact", 32);
  auto surf = natty::create_surface(256, 256);
  natty::surface_font(*surf, *font);

  boosh(*surf);
  stbi::write_rgba("out/test.png", 256, 256, natty::surface_data(*surf));
}
