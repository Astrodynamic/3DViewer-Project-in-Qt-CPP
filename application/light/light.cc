#include "light.h"

namespace s21 {
Light::Light()
    : m_active_{true},
      m_type_{"dirLight"},
      m_direction_{QVector3D(-0.2f, -1.0f, -0.3f)},
      m_ambient_{QVector3D(0.05f, 0.05f, 0.05f)},
      m_diffuse_{QVector3D(0.4f, 0.4f, 0.4f)},
      m_specular_{QVector3D(0.5f, 0.5f, 0.5f)} {
  lightData.insert(".direction", QVariant::fromValue(&m_direction_));
  lightData.insert(".ambient", QVariant::fromValue(&m_ambient_));
  lightData.insert(".diffuse", QVariant::fromValue(&m_diffuse_));
  lightData.insert(".specular", QVariant::fromValue(&m_specular_));
}
}  // namespace s21