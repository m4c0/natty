#pragma leco tool

#ifdef LECO_TARGET_APPLE
#pragma leco add_impl apple
#elif LECO_TARGET_WINDOWS
#pragma leco add_impl windows
#endif

void boosh();

int main() { boosh(); }
