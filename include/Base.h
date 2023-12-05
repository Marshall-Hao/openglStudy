#ifndef __BASE_H__
#define __BASE_H__
// clang-format off
#include <glad/glad.h>// GLAD should be included first
#include <GLFW/glfw3.h>
// clang-format on
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

typedef unsigned int uint;
typedef unsigned char uchar;

struct ffRGBA
{
  uchar m_r;
  uchar m_g;
  uchar m_b;
  uchar m_a;

  ffRGBA(uchar _r = 255, uchar _g = 255, uchar _b = 255, uchar _a = 255)
  {
    m_r = _r;
    m_g = _g;
    m_b = _b;
    m_a = _a;
  }
};

template <typename T>
struct tVec3
{
  T m_x;
  T m_y;
  T m_z;
  tVec3(T _x = 0, T _y = 0, T _z = 0)
  {
    m_x = _x;
    m_y = _y;
    m_z = _z;
  }
};
template <typename T>
struct tVec2
{
  T m_x;
  T m_y;

  tVec2(T _x = 0, T _y = 0)
  {
    m_x = _x;
    m_y = _y;
  }
};

#endif