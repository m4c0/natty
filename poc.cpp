#pragma leco tool

import hai;
import natty;
import stubby;

int main() {
  auto font = natty::create_font("Impact", 32);
  auto surf = natty::create_surface(256, 256);
  natty::surface_font(*surf, *font);
  natty::surface_position(*surf, 40, 80);

  boosh(*surf);
  stbi::write_rgba("out/test.png", 256, 256, natty::surface_data(*surf));
}
