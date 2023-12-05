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