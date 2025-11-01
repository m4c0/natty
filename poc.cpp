#pragma leco test

import hai;
import natty;
import stubby;

int main() {
  auto font = natty::create_font("Arial", 32);
  auto surf = natty::create_surface(256, 256);

  natty::draw({
    .surface = surf,
    .font = font,
    .position { 40, 10 },
    .text = "Olá, tudo bem?",
  });
  natty::draw({
    .surface = surf,
    .font = font,
    .position { 0, 0 },
    .text = ".",
  });

  stbi::write_rgba("out/test.png", 256, 256, natty::surface_data(*surf));
}
