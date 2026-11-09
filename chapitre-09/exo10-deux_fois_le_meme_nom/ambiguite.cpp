#include <iostream>
namespace a { class NkTest { public: void hello() {} }; }
namespace b { class NkTest { public: void hello() {} }; }
int main() { /* NkTest t; */ a::NkTest ta; ta.hello(); return 0; }
