#include <string>

class Font {
    public:
        float scale = 1.0;
        void createTexture();
        void printAt(int penX, int penY, std::string const& text);
};