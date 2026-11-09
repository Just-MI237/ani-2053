#include <iostream>
namespace nkref { class NkRefView { public: void PixelToWorld(float, float) const {} void GridSpacing() const {} void PanByPixels(float, float) {} }; }
int main() { const nkref::NkRefView vue{}; vue.PixelToWorld(1,2); vue.GridSpacing(); /* vue.PanByPixels(1,0); */ return 0; }
