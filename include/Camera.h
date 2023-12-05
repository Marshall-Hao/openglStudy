#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Base.h"

enum CAMERA_MOVE
{
  MOVE_LEFT = 0,
  MOVE_RIGHT,
  MOVE_FORWARD,
  MOVE_BACKWARD,
};

class Camera
{
 private:
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;

  float m_speed;
  glm::mat4 m_viewMatrix;

 public:
  Camera()
  {
    m_position = glm::vec3(1.0f);
    m_front = glm::vec3(0.0f, 0.0f, 1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_speed = 0.01f;

    m_viewMatrix = glm::mat4(1.0f);
  }

  ~Camera() {}
  void setSpeed(float _speed) { m_speed = _speed; }
  glm::mat4 getViewMatrix() const { return m_viewMatrix; }
  void lookAt(const glm::vec3& _position, const glm::vec3& _front,
              const glm::vec3& _up);

  void update();
  void move(CAMERA_MOVE _mode);
};

#endif