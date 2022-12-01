#ifndef MESH_H
#define MESH_H

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <assimp/Importer.hpp>

#include "mesh_information.h"
#include "model_settings.h"

namespace s21 {

struct Vertex {
  QVector3D Position;
  QVector3D Normal;
  QVector2D TexCoords;
  QVector3D Tangent;
  QVector3D Bitangent;
};

struct Texture {
  QOpenGLTexture texture;
  QString type;
  QString path;
  Texture() : texture(QOpenGLTexture::Target2D) {
    texture.create();
    texture.setWrapMode(QOpenGLTexture::DirectionS,
                        QOpenGLTexture::MirroredRepeat);
    texture.setWrapMode(QOpenGLTexture::DirectionT,
                        QOpenGLTexture::MirroredRepeat);
    texture.setMinMagFilters(QOpenGLTexture::LinearMipMapNearest,
                             QOpenGLTexture::Nearest);
  }
};

struct Material {
  QVector3D Ka;
  QVector3D Kd;
  QVector3D Ks;
  QVector3D Ke;
  float Ns = 32.0;
  float Ni = 1.0;
  float refraction = 0.0;
  float reflection = 0.0;
  float roughness = 0.0;
  float d = 1.0;
  unsigned int illum = 0;

  Material()
      : Ka(QVector3D(1.0, 1.0, 1.0)),
        Kd(QVector3D(1.0, 1.0, 1.0)),
        Ks(QVector3D(1.0, 1.0, 1.0)),
        Ke(QVector3D(0.0, 0.0, 0.0)) {}
};

struct Mesh {
 private:
  QOpenGLVertexArrayObject VAO;
  QOpenGLBuffer VBO, EBO;

  QVector<Vertex> vertices;
  QVector<unsigned int> indices;
  QVector<Texture *> textures;
  Material material;
  Material save_material;

  MeshInfo info;

 public:
  Mesh(const char *name, QVector<Vertex> &&vertices,
       QVector<unsigned int> &&indices, QVector<Texture *> &&textures,
       Material &&material);
  ~Mesh();

  MeshInfo GetInfo() const;
  Material &GetMaterial();

  void ChangeTexture(QImage img, QString &path);
  void DelTexture();
  void SetDefaultMaterial();

  void DrawTexture(const ModelSettings &settings, QOpenGLShaderProgram &shader);
  void DrawMaterial(const ModelSettings &settings,
                    QOpenGLShaderProgram &shader);
  void DrawEdge(const ModelSettings &settings, QOpenGLShaderProgram &shader);
  void DrawVertex(const ModelSettings &settings, QOpenGLShaderProgram &shader);

 private:
  void SetupMesh();

  void SetAttribute(QOpenGLShaderProgram &shader);
  void DisibleAttribute(QOpenGLShaderProgram &shader);
};

}  // namespace s21

#endif  // MESH_H
