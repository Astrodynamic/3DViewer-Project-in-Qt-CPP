#include "mesh.h"

namespace s21 {

Mesh::Mesh(const char *name, QVector<Vertex> &&vertices,
           QVector<unsigned int> &&indices, QVector<Texture *> &&textures,
           Material &&material)
    : VBO(QOpenGLBuffer::VertexBuffer),
      EBO(QOpenGLBuffer::IndexBuffer),
      vertices(vertices),
      indices(indices),
      textures(textures),
      material(material),
      save_material(material) {
  SetupMesh();
  info.vertices_count = vertices.count();
  info.face_count = indices.count() / 3;
  info.name = name;
}

Mesh::~Mesh() {
  VBO.destroy();
  EBO.destroy();
  VAO.destroy();
}

MeshInfo Mesh::GetInfo() const { return info; }

Material &Mesh::GetMaterial() { return material; }

void Mesh::ChangeTexture(QImage img, const QString &path) {
  QImage data(1, 1, QImage::Format_RGB32);
  data.fill(Qt::black);
  for (auto &it : textures) {
    it->texture.destroy();
    it->texture.create();
    if (it->type == "texture_ambient") {
      it->texture.setData(img);
      it->path = path;
    } else if (it->type == "texture_diffuse") {
      it->texture.setData(img);
      it->path = path;
    } else {
      it->texture.setData(data);
      it->path = "";
    }
  }
}

void Mesh::DelTexture() {
  QImage data(1, 1, QImage::Format_RGB32);
  data.fill(Qt::black);
  for (auto &it : textures) {
    it->texture.destroy();
    it->texture.create();
    it->texture.setData(data);
    it->path = "";
  }
}

void Mesh::SetDefaultMaterial() { material = save_material; }

void Mesh::DrawTexture(const ModelSettings &settings,
                       QOpenGLShaderProgram &shader) {
  SetAttribute(shader);

  if (settings.GetTextureSettings().type != TextureType::kNo) {
    for (unsigned int i = 0; i < textures.size(); i++) {
      QString name = textures[i]->type;
      if (name == "texture_ambient") {
        shader.setUniformValue("material.ambient", i);
      } else if (name == "texture_diffuse") {
        shader.setUniformValue("material.diffuse", i);
      } else if (name == "texture_specular") {
        shader.setUniformValue("material.specular", i);
      } else if (name == "texture_normal") {
        shader.setUniformValue("material.normal", i);
      } else if (name == "texture_height") {
        shader.setUniformValue("material.height", i);
      }
      textures[i]->texture.bind(i);
    }

    if (settings.GetTextureSettings().type == TextureType::kSurface) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (settings.GetTextureSettings().type == TextureType::kWireFrame) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    shader.setUniformValue("material.Ns", material.Ns);
    shader.setUniformValue("material.Ni", material.Ni);
    shader.setUniformValue("material.d", material.d);
    shader.setUniformValue("material.roughness", material.roughness);
    shader.setUniformValue("material.reflection", material.reflection);
    shader.setUniformValue("material.refraction", material.refraction);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    for (unsigned int i = 0; i < textures.size(); i++) {
      textures[i]->texture.release(i);
    }

    DisibleAttribute(shader);
  }
}

void Mesh::DrawMaterial(const ModelSettings &settings,
                        QOpenGLShaderProgram &shader) {
  SetAttribute(shader);

  if (settings.GetTextureSettings().type != TextureType::kNo) {
    if (settings.GetTextureSettings().type == TextureType::kSurface) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (settings.GetTextureSettings().type == TextureType::kWireFrame) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    shader.setUniformValue("material.Ns", material.Ns);
    shader.setUniformValue("material.Ka", material.Ka);
    shader.setUniformValue("material.Kd", material.Kd);
    shader.setUniformValue("material.Ks", material.Ks);
    shader.setUniformValue("material.Ke", material.Ke);
    shader.setUniformValue("material.Ni", material.Ni);
    shader.setUniformValue("material.roughness", material.roughness);
    shader.setUniformValue("material.d", material.d);
    shader.setUniformValue("material.reflection", material.reflection);
    shader.setUniformValue("material.refraction", material.refraction);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    DisibleAttribute(shader);
  }
}

void Mesh::DrawEdge(const ModelSettings &settings,
                    QOpenGLShaderProgram &shader) {
  if (settings.GetEdgeSettings().type != EdgeType::kNo) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    SetAttribute(shader);

    shader.setUniformValue("u_thickness", settings.GetEdgeSettings().size);
    shader.setUniformValue("PointColor", settings.GetEdgeSettings().color);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    DisibleAttribute(shader);
  }
}

void Mesh::DrawVertex(const ModelSettings &settings,
                      QOpenGLShaderProgram &shader) {
  if (settings.GetVertexSettings().type != VertexType::kNo) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glEnable(GL_PROGRAM_POINT_SIZE);

    SetAttribute(shader);

    shader.setUniformValue("PointColor", settings.GetVertexSettings().color);
    shader.setUniformValue(
        "RoundPoint", settings.GetVertexSettings().type == VertexType::kCircle);
    shader.setUniformValue("PointSize", settings.GetVertexSettings().size);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    DisibleAttribute(shader);
  }
}

void Mesh::SetupMesh() {
  VAO.create();
  VAO.bind();

  VBO.create();
  VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

  VBO.bind();
  VBO.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

  EBO.create();
  EBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

  EBO.bind();
  EBO.allocate(indices.data(), indices.size() * sizeof(unsigned int));

  VAO.release();
  VBO.release();
  EBO.release();
}

void Mesh::SetAttribute(QOpenGLShaderProgram &shader) {
  VAO.bind();
  VBO.bind();
  EBO.bind();

  shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
  shader.enableAttributeArray(0);

  shader.setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, Normal), 3,
                            sizeof(Vertex));
  shader.enableAttributeArray(1);

  shader.setAttributeBuffer(2, GL_FLOAT, offsetof(Vertex, TexCoords), 2,
                            sizeof(Vertex));
  shader.enableAttributeArray(2);

  shader.setAttributeBuffer(3, GL_FLOAT, offsetof(Vertex, Tangent), 3,
                            sizeof(Vertex));
  shader.enableAttributeArray(3);

  shader.setAttributeBuffer(4, GL_FLOAT, offsetof(Vertex, Bitangent), 3,
                            sizeof(Vertex));
  shader.enableAttributeArray(4);

  shader.setUniformValue("skybox", 50);
}

void Mesh::DisibleAttribute(QOpenGLShaderProgram &shader) {
  shader.disableAttributeArray(0);
  shader.disableAttributeArray(1);
  shader.disableAttributeArray(2);
  shader.disableAttributeArray(3);
  shader.disableAttributeArray(4);

  EBO.release();
  VBO.release();
  VAO.release();
}

}  // namespace s21
