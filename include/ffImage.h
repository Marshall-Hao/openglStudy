#ifndef _FF_IMAGE_H_
#define _FF_IMAGE_H_
#include <memory>

#include "Base.h"

class ffImage
{
 private:
  int m_width;
  int m_height;
  int m_picType;
  std::unique_ptr<ffRGBA[]> m_data;

 public:
  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }
  int getPicType() const { return m_picType; }
  ffRGBA* getData() const { return m_data.get(); }

  ffRGBA getColor(int x, int y) const
  {
    if (x < 0 || x > m_width - 1 || y < 0 || y > m_height - 1)
    {
      return ffRGBA(0, 0, 0, 0);
    }
    return m_data[y * m_width + x];
  }

  ffImage(int _width = 0, int _height = 0, int _picType = 0,
          ffRGBA* _data = NULL)
      : m_width(_width), m_height(_height), m_picType(_picType)
  {
    int _sumSize = m_width * m_height;
    if (_data && _sumSize)
    {
      m_data = std::make_unique<ffRGBA[]>(_sumSize);
      memcpy(m_data.get(), _data, sizeof(ffRGBA) * _sumSize);
    }
  }

  // The default destructor is fine since unique_ptr automatically deallocates
  // memory ~ffImage() is not needed

  static std::unique_ptr<ffImage> readFromFile(
      const char*
          _fileName);  // Consider returning std::unique_ptr<ffImage> instead
};

#endif