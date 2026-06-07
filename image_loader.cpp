#include "image_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"

void ImageLoader::LoadImage(const std::string &png_file_name, Texture &texture)
{
    int channels;

    texture.pixels = stbi_load(png_file_name.c_str(), &texture.width, &texture.height, &channels, 4);

    if (!texture.pixels)
    {
        std::cout << "Image file couldn't be read!" << std::endl;
    }
}