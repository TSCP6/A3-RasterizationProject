#include <cstdio>
#include <cstdlib>
#include <iostream>

class ppm
{
private:
    int Px;
    int sizeX, sizeY;
    unsigned char maxColor;
    unsigned char *image;
public:
    ~ppm() {
        if (image != nullptr) {
            free(image);
        }
        image = nullptr;
    }
    void write_ppm(char *filename);
    void create_ppm();
};

void ppm::write_ppm(char *filename) {
    FILE *fp;
    fp = fopen(filename, "wb");
    fprintf(fp, "P%d\n%d %d\n%d\n", Px, sizeX, sizeY, maxColor);
    fwrite(image, sizeof(unsigned char), sizeX * sizeY * 3, fp);
    fclose(fp);
    fp = nullptr;
}

void ppm::create_ppm() {
    Px = 6;
    sizeX = 256, sizeY = 64;
    maxColor = 255;
    image = (unsigned char *)malloc(sizeof(unsigned char) * sizeX * sizeY * 3);
    for (int y = 0; y < sizeY; ++y) {
        for (int x = 0; x < sizeX; ++x) {
            int idx = (y * sizeX + x) * 3;
            unsigned char r = static_cast<unsigned char>((x * 255) / (sizeX - 1));
            unsigned char g = static_cast<unsigned char>((y * 255) / (sizeY - 1));
            unsigned char b = static_cast<unsigned char>(((sizeX - 1 - x) * 255) / (sizeX - 1));
            image[idx + 0] = r;
            image[idx + 1] = g;
            image[idx + 2] = b;
        }
    }
}

int main() {
    ppm img;
    img.create_ppm();
    img.write_ppm("output/pretest1/pretest1.ppm");
    std::cout << "Pretest1 finished: generate the gradient graph" << std::endl;
    return 0;
}