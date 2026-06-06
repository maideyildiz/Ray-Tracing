#include "image_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void ImageLoader::LoadImage(const std::string &obj_file_name)
{
    // int width, height, channels;
    // unsigned char *img = stbi_load(obj_file_name, &width, &height, &channels, 4);

    // if (img)
    // {
    //     // r = img[(y * width + x) * 4 + 0];
    //     // g = img[(y * width + x) * 4 + 1];
    //     // b = img[(y * width + x) * 4 + 2];

    //     stbi_image_free(img);
    // }
}