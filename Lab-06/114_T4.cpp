#include <iostream>
using namespace std;

class PixelAlpha {
private:
    int red;
    int green;
    int blue;
    int alpha;
    int clamp(int value) const {
        if (value < 0) return 0;
        if (value > 255) return 255;
        return value;
    }
public:
    PixelAlpha() : red(0), green(0), blue(0), alpha(255) {}
    
    PixelAlpha(int r, int g, int b, int a = 255) {
        red = clamp(r);
        green = clamp(g);
        blue = clamp(b);
        alpha = clamp(a);
    }

    ~PixelAlpha() {}
    int getRed() const { return red; }
    int getGreen() const { return green; }
    int getBlue() const { return blue; }
    int getAlpha() const { return alpha; }

    void setRed(int r) { red = clamp(r); }
    void setGreen(int g) { green = clamp(g); }
    void setBlue(int b) { blue = clamp(b); }
    void setAlpha(int a) { alpha = clamp(a); }

    PixelAlpha operator*(float factor) const {
        int newAlpha = clamp(static_cast<int>(alpha * factor));
        return PixelAlpha(red, green, blue, newAlpha);
    }
    
    PixelAlpha operator+(const PixelAlpha& other) const {
        int newRed = clamp(red + other.red);
        int newGreen = clamp(green + other.green);
        int newBlue = clamp(blue + other.blue);
        int newAlpha = clamp(alpha + other.alpha);
        return PixelAlpha(newRed, newGreen, newBlue, newAlpha);
    }

    friend ostream& operator<<(ostream& os, const PixelAlpha& pixel) {
        os << "PixelAlpha(R: " << pixel.red 
           << ", G: " << pixel.green 
           << ", B: " << pixel.blue 
           << ", A: " << pixel.alpha << ")";
        return os;
    }
};

PixelAlpha operator*(float factor, const PixelAlpha& pixel) {
    return pixel * factor;
}

int main() {
    PixelAlpha p1(200, 150, 100, 200);
    cout << "p1: " << p1 << endl;

    PixelAlpha p2 = p1 * 0.5;
    cout << "p2 = p1 * 0.5: " << p2 << endl;

    PixelAlpha p3 = 0.25 * p1;
    cout << "p3 = 0.25 * p1: " << p3 << endl;

    PixelAlpha p4 = p2 + p3;
    cout << "p4 = p2 + p3: " << p4 << endl;

    cout << "\nTesting commutativity:" << endl;
    PixelAlpha p5 = p1 * 0.3;
    PixelAlpha p6 = 0.3 * p1;
    cout << "p1 * 0.3: " << p5 << endl;
    cout << "0.3 * p1: " << p6 << endl;

    cout << "\nTesting error handling:" << endl;
    PixelAlpha p7(300, 400, -50, 500);
    cout << "PixelAlpha(300, 400, -50, 500): " << p7 << endl;
    
    return 0;
}