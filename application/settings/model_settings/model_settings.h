#ifndef MODEL_SETTINGS_H_
#define MODEL_SETTINGS_H_

#include <QColor>
#include <QMetaType>
#include <QString>
#include <QVariant>

#include "global_settings.h"

namespace s21 {

enum class VertexType { kNo = 0, kCircle, kSquare };

enum class EdgeType { kNo = 0, kLine, kDotted };

enum class TextureType { kNo = 0, kWireFrame, kSurface };

enum class SurfaceType { kMaterial = 0, kTexture };

struct EdgeSetting {
  QColor color;
  float size;
  EdgeType type;
};

struct VertexSetting {
  QColor color;
  float size;
  VertexType type;
};

struct TextureSetting {
  TextureType type;
};

struct TranslateSetting {
  float Tx, Ty, Tz;
};

struct RotateSetting {
  float Rx, Ry, Rz;
};

struct ScaleSetting {
  float Sx, Sy, Sz, STotal;
};

class ModelSettings : public GlobalSetting {
 private:
  EdgeSetting m_edge_;
  VertexSetting m_vertex_;
  TextureSetting m_texture_;
  TranslateSetting m_translate_;
  RotateSetting m_rotate_;
  ScaleSetting m_scale_;
  QString m_parent_name_;
  SurfaceType m_surface_ = SurfaceType::kMaterial;

 public:
  ModelSettings()
      : m_edge_{QColor(255, 255, 255), 1.0, EdgeType::kLine},
        m_vertex_{QColor(255, 255, 255), 1.0, VertexType::kNo},
        m_texture_{TextureType::kSurface},
        m_translate_{0.0f, 0.0f, 0.0f},
        m_rotate_{0.0f, 0.0f, 0.0f},
        m_scale_{1.0f, 1.0f, 1.0f, 1.0f} {};

  explicit ModelSettings(QString path)
      : m_edge_{QColor(255, 255, 255), 1.0, EdgeType::kLine},
        m_vertex_{QColor(255, 255, 255), 1.0, VertexType::kNo},
        m_texture_{TextureType::kSurface},
        m_translate_{0.0f, 0.0f, 0.0f},
        m_rotate_{0.0f, 0.0f, 0.0f},
        m_scale_{1.0f, 1.0f, 1.0f, 1.0f},
        m_parent_name_{path} {
    if (!m_parent_name_.isEmpty() && haveSettings(m_parent_name_)) {
      *this = qvariant_cast<ModelSettings>(getSettings(m_parent_name_));
    }
  };
  ~ModelSettings(){

  };

  ModelSettings operator=(const ModelSettings &other) {
    if (this != &other) {
      this->m_edge_ = other.m_edge_;
      this->m_vertex_ = other.m_vertex_;
      this->m_texture_ = other.m_texture_;
      this->m_translate_ = other.m_translate_;
      this->m_rotate_ = other.m_rotate_;
      this->m_scale_ = other.m_scale_;
      this->m_parent_name_ = other.m_parent_name_;
      this->m_surface_ = other.m_surface_;
    }
    return *this;
  };

  friend QDataStream &operator<<(QDataStream &arch,
                                 const ModelSettings &object) {
    arch << object.m_edge_.color;
    arch << object.m_edge_.size;
    arch << object.m_edge_.type;
    arch << object.m_vertex_.size;
    arch << object.m_vertex_.type;
    arch << object.m_vertex_.color;
    arch << object.m_texture_.type;
    arch << object.m_translate_.Tx;
    arch << object.m_translate_.Ty;
    arch << object.m_translate_.Tz;
    arch << object.m_rotate_.Rx;
    arch << object.m_rotate_.Ry;
    arch << object.m_rotate_.Rz;
    arch << object.m_scale_.Sx;
    arch << object.m_scale_.Sy;
    arch << object.m_scale_.Sz;
    arch << object.m_scale_.STotal;
    arch << object.m_parent_name_;
    arch << object.m_surface_;
    return arch;
  }

  friend QDataStream &operator>>(QDataStream &arch, ModelSettings &object) {
    arch >> object.m_edge_.color;
    arch >> object.m_edge_.size;
    arch >> object.m_edge_.type;
    arch >> object.m_vertex_.size;
    arch >> object.m_vertex_.type;
    arch >> object.m_vertex_.color;
    arch >> object.m_texture_.type;
    arch >> object.m_translate_.Tx;
    arch >> object.m_translate_.Ty;
    arch >> object.m_translate_.Tz;
    arch >> object.m_rotate_.Rx;
    arch >> object.m_rotate_.Ry;
    arch >> object.m_rotate_.Rz;
    arch >> object.m_scale_.Sx;
    arch >> object.m_scale_.Sy;
    arch >> object.m_scale_.Sz;
    arch >> object.m_scale_.STotal;
    arch >> object.m_parent_name_;
    arch >> object.m_surface_;
    return arch;
  }

  void SaveModelSetting() {
    if (!m_parent_name_.isEmpty()) {
      const QString tmp = "ModelSettings";
      qRegisterMetaType<ModelSettings>(tmp.toStdString().c_str());
      setSettings(m_parent_name_, QVariant::fromValue(*this));
    }
  }

  const QVariant getSettings(const QString param) {
    QSettings settings{"Ajhin_team", "3D_viewer.v2"};
    const QString tmp = "ModelSettings";
    qRegisterMetaType<ModelSettings>(tmp.toStdString().c_str());
    return settings.value(param);
  }

  EdgeSetting GetEdgeSettings() const { return m_edge_; }

  VertexSetting GetVertexSettings() const { return m_vertex_; }

  TextureSetting GetTextureSettings() const { return m_texture_; }

  TranslateSetting GetTranslateSettings() const { return m_translate_; }

  RotateSetting GetRotateSettings() const { return m_rotate_; }

  ScaleSetting GetScaleSettings() const { return m_scale_; }

  SurfaceType GetSurfaceSettings() const { return m_surface_; }

  void SetEdgeColor(const QColor color) { m_edge_.color = color; }

  void SetEdgeSize(const float size) {
    if (size > 0.0f) {
      m_edge_.size = size;
    }
  }

  void SetEdgeType(const EdgeType type) { m_edge_.type = type; }

  void SetVertexColor(const QColor color) { m_vertex_.color = color; }

  void SetVertexSize(const float size) {
    if (size > 0.0f) {
      m_vertex_.size = size;
    }
  }

  void SetVertexType(const VertexType type) { m_vertex_.type = type; }

  void SetTextureType(const TextureType type) { m_texture_.type = type; }

  void SetSurfaceType(const SurfaceType type) { m_surface_ = type; }

  void SetTranslateX(const float shift) { m_translate_.Tx = shift; }

  void SetTranslateY(const float shift) { m_translate_.Ty = shift; }

  void SetTranslateZ(const float shift) { m_translate_.Tz = shift; }

  void SetRotateX(const float degree) {
    if (degree >= 0.0f && degree <= 360.0f) {
      m_rotate_.Rx = degree;
    }
  }

  void SetRotateY(const float degree) {
    if (degree >= 0.0f && degree <= 360.0f) {
      m_rotate_.Ry = degree;
    }
  }

  void SetRotateZ(const float degree) {
    if (degree >= 0.0f && degree <= 360.0f) {
      m_rotate_.Rz = degree;
    }
  }

  void SetScaleX(const float scale) {
    if (scale > 0.0f) {
      this->m_scale_.Sx = scale;
    }
  }

  void SetScaleY(const float scale) {
    if (scale > 0.0f) {
      this->m_scale_.Sy = scale;
    }
  }

  void SetScaleZ(const float scale) {
    if (scale > 0.0f) {
      this->m_scale_.Sz = scale;
    }
  }

  void SetScaleTotal(const float scale) {
    if (scale > 0.0f) {
      this->m_scale_.STotal = scale;
    }
  }
};

}  // namespace s21

Q_DECLARE_METATYPE(s21::ModelSettings *);

#endif  // MODEL_SETTINGS_H_
