#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

namespace s21 {

class Camera {
 private:
  QMatrix4x4 m_camera_;

  QVector3D m_position_;
  QVector3D m_front_;
  QVector3D m_up_;
  QVector3D m_right_;
  QVector3D m_world_up_;

  float m_yaw_ = -90.0f;
  float m_pitch_ = 0.0f;

  float m_movement_speed_ = 0.5f;
  float m_mouse_sensitivity_ = 0.1f;

  float m_zoom_ = 45.0f;

 public:
  Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f),
         QVector3D up = QVector3D(0.0f, 1.0f, 0.0f));

  QMatrix4x4 &GetViewMatrix();
  QVector3D GetPosition() const;
  QVector3D GetViewDiraction() const;
  float GetZoom() const;

  void ChangeFocus(QVector3D dir);

  void ProcessKeyboard(int direction, float delta_time);
  void ProcessMouseMovement(float xoffset, float yoffset,
                            bool constrain_pitch = true);
  void ProcessMouseScroll(float yoffset);

 private:
  void UpdateCameraVectors();

  float Angle2D(QVector2D v_1, QVector2D v_2) const;
};

}  // namespace s21
#endif
