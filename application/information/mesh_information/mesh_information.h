#ifndef MESH_INFORMATION_H_
#define MESH_INFORMATION_H_

#include <QString>

namespace s21 {

struct MeshInfo {
  QString name;

  unsigned int vertices_count;
  unsigned int face_count;
};

}  // namespace s21

#endif  // MESH_INFORMATION_H_
