#include "Camera.h"

#include "Base.h"

void Camera::lookAt(const glm::vec3& _position, const glm::vec3& _front,
                    const glm::vec3& _up)
{
  m_position = _position;
  // just a direction
  m_front = glm::normalize(_front);
  m_up = _up;
}

void Camera::update()
{
  // update view matrix
  m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::move(CAMERA_MOVE _mode)
{
  switch (_mode)
  {
    case MOVE_LEFT:
      m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed;
      break;
    case MOVE_RIGHT:
      m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed;
      break;
    case MOVE_FORWARD:
      m_position += m_front * m_speed;
      break;
    case MOVE_BACKWARD:
      m_position -= m_front * m_speed;
      break;
    default:
      break;
  }
}

void Camera::pitch(float _yOffset)
{
  // angle
  m_pitch += _yOffset * m_sensitive;
  // angle limit
  if (m_pitch > 89.0f) m_pitch = 89.0f;
  if (m_pitch < -89.0f) m_pitch = -89.0f;

  m_front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
  m_front.y = sin(glm::radians(m_pitch));
  m_front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

  // always the direction
  m_front = glm::normalize(m_front);
  std::cout << "m_front: " << m_front.x << " " << m_front.y << " " << m_front.z
            << std::endl;
  // update();
}

void Camera::yaw(float _xOffset)
{
  // angle
  m_yaw += _xOffset * m_sensitive;

  m_front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
  m_front.y = sin(glm::radians(m_pitch));
  m_front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

  m_front = glm::normalize(m_front);
  std::cout << "m_front: " << m_front.x << " " << m_front.y << " " << m_front.z
            << std::endl;
  // update();
}

void Camera::onMouseMove(double _xpos, double _ypos)
{
  if (m_firstMouse)
  {
    m_xpos = _xpos;
    m_ypos = _ypos;
    m_firstMouse = false;

    return;
  }

  float xOffset = _xpos - m_xpos;
  float yOffset = m_ypos - _ypos;

  m_xpos = _xpos;
  m_ypos = _ypos;

  pitch(yOffset);
  yaw(xOffset);
}