#include "camera.h"

namespace s21 {

Camera::Camera(QVector3D position, QVector3D up)
    : m_position_{position},
      m_front_(QVector3D(0.0f, 0.0f, -1.0f)),
      m_world_up_{up} {
  UpdateCameraVectors();
}

QMatrix4x4 &Camera::GetViewMatrix() {
  m_camera_.setToIdentity();
  m_camera_.lookAt(m_position_, m_position_ + m_front_, m_up_);
  return m_camera_;
}

QVector3D Camera::GetPosition() const { return m_position_; }

QVector3D Camera::GetViewDiraction() const { return m_front_; }

float Camera::GetZoom() const { return m_zoom_; }

void Camera::ChangeFocus(QVector3D dir) {
  QVector3D temp(dir - m_position_);
  temp.normalize();

  float x_angle = Angle2D(QVector2D(temp.x(), temp.z()),
                          QVector2D(m_front_.x(), m_front_.z()));
  float y_angle = Angle2D(QVector2D(temp.y(), temp.z()),
                          QVector2D(m_front_.y(), m_front_.z()));

  if (abs(x_angle) > 1e-6 && abs(y_angle) > 1e-6) {
    ProcessMouseMovement(x_angle, y_angle, true);
  }
}

float Camera::Angle2D(QVector2D v_1, QVector2D v_2) const {
  float dot = QVector2D::dotProduct(v_1, v_2);

  float sqrt = qSqrt(v_1.lengthSquared() * v_2.lengthSquared());

  float angle = 0.0;
  if (abs(sqrt) > 1e-6 && abs(dot) > 1e-6) {
    angle = qRadiansToDegrees(qAcos(dot / sqrt)) / m_mouse_sensitivity_;
  }

  return (v_1.x() > v_2.x()) ? angle : -angle;
}

void Camera::ProcessKeyboard(int direction, float delta_time) {
  float velocity = m_movement_speed_ * delta_time;
  if (direction == Qt::Key_W) m_position_ += m_front_ * velocity;
  if (direction == Qt::Key_S) m_position_ -= m_front_ * velocity;
  if (direction == Qt::Key_A) m_position_ -= m_right_ * velocity;
  if (direction == Qt::Key_D) m_position_ += m_right_ * velocity;
  if (direction == Qt::Key_R) m_position_ += m_up_ * velocity;
  if (direction == Qt::Key_F) m_position_ -= m_up_ * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset,
                                  bool constrain_pitch) {
  xoffset *= m_mouse_sensitivity_;
  yoffset *= m_mouse_sensitivity_;

  m_yaw_ += xoffset;
  m_pitch_ += yoffset;

  if (constrain_pitch) {
    if (m_pitch_ > 89.0f) m_pitch_ = 89.0f;
    if (m_pitch_ < -89.0f) m_pitch_ = -89.0f;
  }
  UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
  m_zoom_ -= (float)yoffset;
  if (m_zoom_ < 1.0f) m_zoom_ = 1.0f;
  if (m_zoom_ > 45.0f) m_zoom_ = 45.0f;
}

void Camera::UpdateCameraVectors() {
  m_front_.setX(qCos(qDegreesToRadians(m_yaw_)) *
                qCos(qDegreesToRadians(m_pitch_)));
  m_front_.setY(qSin(qDegreesToRadians(m_pitch_)));
  m_front_.setZ(qSin(qDegreesToRadians(m_yaw_)) *
                qCos(qDegreesToRadians(m_pitch_)));
  m_front_.normalize();

  m_right_ = QVector3D::normal(m_front_, m_world_up_);
  m_up_ = QVector3D::normal(m_right_, m_front_);
}

}  // namespace s21
