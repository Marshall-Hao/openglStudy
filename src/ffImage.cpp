#include "ffImage.h"

#include <memory>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::unique_ptr<ffImage> ffImage::readFromFile(const char* _fileName)
{
  int _picType = 0;
  int _width = 0;
  int _height = 0;

  // upside down
  stbi_set_flip_vertically_on_load(true);

  unsigned char* bits =
      stbi_load(_fileName, &_width, &_height, &_picType, STBI_rgb_alpha);
  auto _image = std::make_unique<ffImage>(_width, _height, _picType,
                                          reinterpret_cast<ffRGBA*>(bits));

  stbi_image_free(bits);
  return _image;
}