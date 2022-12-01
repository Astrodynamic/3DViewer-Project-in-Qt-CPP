#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "light.h"

namespace s21 {
class PointLight : public Light {
 public:
  PointLight()
      : Light(),
        m_position_{0.7f, 0.2f, 2.0f},
        m_constant_{1.f},
        m_linear_{0.0014f},
        m_quadratic_{0.000007f},
        m_curent_distance_{11},
        m_distance_{{1.f, 0.7f, 1.8f},      {1.f, 0.35f, 0.54f},
                    {1.f, 0.22f, 0.2f},     {1.f, 0.14f, 0.07f},
                    {1.f, 0.09f, 0.032f},   {1.f, 0.07f, 0.017f},
                    {1.f, 0.045f, 0.0075f}, {1.f, 0.027f, 0.0028f},
                    {1.f, 0.022f, 0.0019f}, {1.f, 0.014f, 0.0007f},
                    {1.f, 0.007f, 0.0002f}, {1.f, 0.0014f, 0.000007f}} {
    m_type_ = "pointLight";
    lightData.insert(".position", QVariant::fromValue(&m_position_));
    lightData.insert(".constant", QVariant::fromValue(&m_constant_));
    lightData.insert(".linear", QVariant::fromValue(&m_linear_));
    lightData.insert(".quadratic", QVariant::fromValue(&m_quadratic_));
  }

  void SetDistance(const int index) {
    m_constant_ = m_distance_[index].x();
    m_linear_ = m_distance_[index].y();
    m_quadratic_ = m_distance_[index].z();
    m_curent_distance_ = index;
  }
  const int &GetDistance() const { return m_curent_distance_; }

 protected:
  QVector3D m_position_;
  float m_constant_;
  float m_linear_;
  float m_quadratic_;
  int m_curent_distance_;
  QVector3D m_distance_[12];
};

}  // namespace s21
Q_DECLARE_METATYPE(s21::PointLight);
#endif  // POINT_LIGHT_H
