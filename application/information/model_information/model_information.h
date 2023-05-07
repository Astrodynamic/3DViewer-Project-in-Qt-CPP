#ifndef MODELINFO_H_
#define MODELINFO_H_

#include <QDir>

#include "mesh.h"

namespace s21 {

struct ModelInfo {
 private:
  QString kName;
  QDir kDirectory;

  QVector3D mid_value;

  unsigned int vertices_count = 0;
  unsigned int face_count = 0;

 public:
  QVector3D min_value;
  QVector3D max_value;

  QMatrix4x4 m_matrix;

  QVector<Mesh *> m_meshes;
  QVector<Texture *> textures;

  explicit ModelInfo(QString name)
      : kName{name},
        kDirectory(name),
        min_value{QVector3D(INFINITY, INFINITY, INFINITY)},
        max_value{QVector3D(-INFINITY, -INFINITY, -INFINITY)} {
    kDirectory.cdUp();
  }

  float GetRatioToIndentify() {
    QVector4D t_min(min_value, 1.0);
    QVector4D t_max(max_value, 1.0);

    t_min = m_matrix * t_min;
    t_max = m_matrix * t_max;

    float length = t_min.toVector3D().distanceToPoint(t_max.toVector3D());

    return ((length > qSqrt(3.0)) ? 1.0f / length : 1.0);
  }

  QString GetName() const { return kName; }

  QDir GetDirectory() const { return kDirectory; }

  QVector3D GetCenterModelVertex() {
    QVector4D t_min(min_value, 1.0);
    QVector4D t_max(max_value, 1.0);

    t_min = m_matrix * t_min;
    t_max = m_matrix * t_max;
    return (t_min + (t_max - t_min) / 2.0).toVector3D();
  }

  void AddMeshVerices(unsigned int count) { vertices_count += count; }

  void AddMeshFace(unsigned int count) { face_count += count; }

  unsigned int GetVerticeCount() const { return vertices_count; }

  unsigned int GetFaceCount() const { return face_count; }
};

}  // namespace s21

#endif  // MODELINFO_H_
