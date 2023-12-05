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

  float m_yaw;
  float m_pitch;
  float m_sensitive;

  // mouse
  float m_xpos;
  float m_ypos;
  bool m_firstMouse;

 public:
  Camera()
  {
    m_position = glm::vec3(1.0f);
    m_front = glm::vec3(0.0f, 0.0f, 1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_speed = 0.01f;

    m_pitch = 0.0f;
    m_yaw = -90.0f;  // since -z is the front
    m_viewMatrix = glm::mat4(1.0f);

    m_xpos = 0.0f;
    m_ypos = 0.0f;
    m_firstMouse = true;
  }

  ~Camera() {}
  void setSpeed(float _speed) { m_speed = _speed; }
  glm::mat4 getViewMatrix() const { return m_viewMatrix; }
  void lookAt(const glm::vec3& _position, const glm::vec3& _front,
              const glm::vec3& _up);

  void update();
  void move(CAMERA_MOVE _mode);

  void pitch(float _yOffset);
  void yaw(float _xOffset);
  void setSensitive(float _sensitive) { m_sensitive = _sensitive; }
  void onMouseMove(double _xpos, double _ypos);
};

#endif