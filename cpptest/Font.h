#include <string>

class Font {
    public:
        float scale = 1.0;
        void createTexture();
        void printAt(float penX, float penY, float kerning, std::string const& text);
};