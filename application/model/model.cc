#include "model.h"

#include <QOpenGLTexture>

namespace s21 {

Model::Model(QString path)
    : m_settings_{ModelSettings(path)}, info_{new ModelInfo(path)} {
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(
      path.toLocal8Bit(),
      aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |
          aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    emit Error(QString("ERROR::ASSIMP::" + QString(import.GetErrorString())));
    return;
  }

  info_->GetDirectory().cdUp();
  ProcessNode(scene->mRootNode, scene);
  for (auto it : info_->m_meshes) {
    info_->AddMeshVerices(it->GetInfo().vertices_count);
    info_->AddMeshFace(it->GetInfo().face_count);
  }
}

Model::~Model() {
  for (auto &it : info_->textures) {
    it->texture.destroy();
    delete it;
  }

  for (auto &it : info_->m_meshes) {
    delete it;
  }
  m_settings_.SaveModelSetting();
  delete info_;
}

void Model::TransformMatrix() {
  info_->m_matrix.setToIdentity();

  const TranslateSetting translate = m_settings_.GetTranslateSettings();
  info_->m_matrix.translate(translate.Tx, translate.Ty, translate.Tz);

  const RotateSetting rotate = m_settings_.GetRotateSettings();
  info_->m_matrix.rotate(rotate.Rx, 1.0f, 0.0f, 0.0f);
  info_->m_matrix.rotate(rotate.Ry, 0.0f, 1.0f, 0.0f);
  info_->m_matrix.rotate(rotate.Rz, 0.0f, 0.0f, 1.0f);

  const ScaleSetting scale = m_settings_.GetScaleSettings();
  info_->m_matrix.scale(scale.Sx, scale.Sy, scale.Sz);
  info_->m_matrix.scale(scale.STotal);
}

void Model::DrawTexture(QOpenGLShaderProgram &shader) {
  if (m_settings_.GetSurfaceSettings() == SurfaceType::kTexture) {
    TransformMatrix();
    for (Mesh *mesh : info_->m_meshes) {
      mesh->DrawTexture(m_settings_, shader);
    }
  }
}

void Model::DrawMaterial(QOpenGLShaderProgram &shader) {
  if (m_settings_.GetSurfaceSettings() == SurfaceType::kMaterial) {
    TransformMatrix();
    for (Mesh *mesh : info_->m_meshes) {
      mesh->DrawMaterial(m_settings_, shader);
    }
  }
}

void Model::DrawEdge(QOpenGLShaderProgram &shader) {
  TransformMatrix();
  for (Mesh *mesh : info_->m_meshes) {
    mesh->DrawEdge(m_settings_, shader);
  }
}

void Model::DrawVertex(QOpenGLShaderProgram &shader) {
  TransformMatrix();
  for (Mesh *mesh : info_->m_meshes) {
    mesh->DrawVertex(m_settings_, shader);
  }
}

void Model::ChangeTexture(QImage img, QString &path) {
  QOpenGLTexture texture(img);
  if (m_current_mesh_) {
    m_current_mesh_->ChangeTexture(img, path);
  } else {
    for (auto &it : info_->m_meshes) {
      it->ChangeTexture(img, path);
    }
  }
}

void Model::DelTexture() {
  if (m_current_mesh_) {
    m_current_mesh_->DelTexture();
  } else {
    for (auto &it : info_->m_meshes) {
      it->DelTexture();
    }
  }
}

void Model::MirrorTexture() {
  for (auto &it : info_->textures) {
    if (it->path != "") {
      it->texture.destroy();
      it->texture.create();
      QImage data(info_->GetDirectory().filePath(it->path));
      it->texture.setData(data.mirrored());
    }
  }
}

void Model::SetDefaultMaterial() {
  for (auto &it : info_->m_meshes) {
    it->SetDefaultMaterial();
  }
}

void Model::ChangeAmbient(QColor color) {
  if (m_current_mesh_) {
    m_current_mesh_->GetMaterial().Ka =
        QVector3D(color.redF(), color.greenF(), color.blueF());
  } else {
    for (auto &it : info_->m_meshes) {
      it->GetMaterial().Ka =
          QVector3D(color.redF(), color.greenF(), color.blueF());
    }
  }
}

void Model::ChangeDiffuse(QColor color) {
  if (m_current_mesh_) {
    m_current_mesh_->GetMaterial().Kd =
        QVector3D(color.redF(), color.greenF(), color.blueF());

  } else {
    for (auto &it : info_->m_meshes) {
      it->GetMaterial().Kd =
          QVector3D(color.redF(), color.greenF(), color.blueF());
    }
  }
}

void Model::ChangeSpecular(QColor color) {
  if (m_current_mesh_) {
    m_current_mesh_->GetMaterial().Ks =
        QVector3D(color.redF(), color.greenF(), color.blueF());

  } else {
    for (auto &it : info_->m_meshes) {
      it->GetMaterial().Ks =
          QVector3D(color.redF(), color.greenF(), color.blueF());
    }
  }
}

void Model::ChangeShine(double shine) {
  if (m_current_mesh_) {
    m_current_mesh_->GetMaterial().Ns = shine;
  } else {
    for (auto &it : info_->m_meshes) {
      it->GetMaterial().Ns = shine;
    }
  }
}

void Model::ChangeOpacity(double opacity) {
  if (m_current_mesh_) {
    m_current_mesh_->GetMaterial().d = opacity;
  } else {
    for (auto &it : info_->m_meshes) {
      it->GetMaterial().d = opacity;
    }
  }
}

void Model::ChangeReflection(double reflect) {
  if (m_current_mesh_) {
    m_current_mesh_->GetMaterial().reflection = reflect;
  } else {
    for (auto &it : info_->m_meshes) {
      it->GetMaterial().reflection = reflect;
    }
  }
}

void Model::ChangeRoughness(double roughness) {
  if (m_current_mesh_) {
    m_current_mesh_->GetMaterial().roughness = roughness;
  } else {
    for (auto &it : info_->m_meshes) {
      it->GetMaterial().roughness = roughness;
    }
  }
}

void Model::ChangeRefraction(double refract) {
  if (m_current_mesh_) {
    m_current_mesh_->GetMaterial().refraction = refract;
  } else {
    for (auto &it : info_->m_meshes) {
      it->GetMaterial().refraction = refract;
    }
  }
}

Mesh *Model::GetCurrentMesh() { return m_current_mesh_; }

void Model::ChangeCurentMesh(int i) {
  if (i < 0) {
    m_current_mesh_ = nullptr;
  } else {
    m_current_mesh_ = info_->m_meshes[i];
  }
}

const QMatrix4x4 Model::GetModelMatrix() const { return info_->m_matrix; }

ModelSettings &Model::GetSettings() { return m_settings_; }

ModelInfo Model::GetInfo() { return *info_; }

QStringList Model::GetMeshesName() {
  QStringList names;
  for (auto &it : info_->m_meshes) {
    names << it->GetInfo().name;
  }
  return names;
}

Model *Model::createModel(QString path) { return new Model(path); }

void Model::Destroy() { delete this; }

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    info_->m_meshes.push_back(ProcessMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

Mesh *Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
  QVector<Vertex> vertices;
  LoadVertexData(mesh, vertices);

  QVector<unsigned int> indices;
  LoadIndicesData(mesh, indices);

  QVector<Texture *> textures;
  LoadTextureData(scene, mesh, textures);

  Material material;
  LoadMaterial(scene, mesh, material);

  return new Mesh(mesh->mName.C_Str(), std::move(vertices), std::move(indices),
                  std::move(textures), std::move(material));
}

void Model::LoadVertexData(aiMesh *mesh, QVector<Vertex> &vertices) {
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;

    vertex.Position.setX(mesh->mVertices[i].x);
    vertex.Position.setY(mesh->mVertices[i].y);
    vertex.Position.setZ(mesh->mVertices[i].z);

    for (int j = 0; j < 3; ++j) {
      if (vertex.Position[j] < info_->min_value[j]) {
        info_->min_value[j] = vertex.Position[j];
      }
      if (vertex.Position[j] > info_->max_value[j]) {
        info_->max_value[j] = vertex.Position[j];
      }
    }

    if (mesh->mNormals) {
      vertex.Normal.setX(mesh->mNormals[i].x);
      vertex.Normal.setY(mesh->mNormals[i].y);
      vertex.Normal.setZ(mesh->mNormals[i].z);
    }

    if (mesh->mTextureCoords[0]) {
      vertex.TexCoords.setX(mesh->mTextureCoords[0][i].x);
      vertex.TexCoords.setY(mesh->mTextureCoords[0][i].y);
    } else {
      vertex.TexCoords = QVector2D(0, 0);
    }

    if (mesh->mTangents) {
      vertex.Tangent.setX(mesh->mTangents[i].x);
      vertex.Tangent.setY(mesh->mTangents[i].y);
      vertex.Tangent.setZ(mesh->mTangents[i].z);
    }

    if (mesh->mBitangents) {
      vertex.Bitangent.setX(mesh->mBitangents[i].x);
      vertex.Bitangent.setY(mesh->mBitangents[i].y);
      vertex.Bitangent.setZ(mesh->mBitangents[i].z);
    }

    vertices.push_back(vertex);
  }
}

void Model::LoadIndicesData(aiMesh *mesh, QVector<unsigned int> &indices) {
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }
}

void Model::LoadTextureData(const aiScene *scene, aiMesh *mesh,
                            QVector<Texture *> &textures) {
  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  if (material->GetTextureCount(aiTextureType_AMBIENT)) {
    QVector<Texture *> ambientMaps = LoadMaterialTextures(
        material, aiTextureType_AMBIENT, "texture_ambient");
    std::copy(ambientMaps.begin(), ambientMaps.end(),
              std::back_inserter(textures));
  } else {
    Texture *texture = new Texture;
    QImage data(1, 1, QImage::Format_RGB32);
    data.fill(Qt::white);
    texture->texture.setData(data);
    texture->type = "texture_ambient";
    texture->path = "";
    textures.push_back(texture);
    info_->textures.push_back(texture);
  }

  if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
    QVector<Texture *> diffuseMaps = LoadMaterialTextures(
        material, aiTextureType_DIFFUSE, "texture_diffuse");
    std::copy(diffuseMaps.begin(), diffuseMaps.end(),
              std::back_inserter(textures));
  } else {
    Texture *texture = new Texture;
    QImage data(1, 1, QImage::Format_RGB32);
    data.fill(Qt::white);
    texture->texture.setData(data);
    texture->type = "texture_diffuse";
    texture->path = "";
    textures.push_back(texture);
    info_->textures.push_back(texture);
  }

  if (material->GetTextureCount(aiTextureType_SPECULAR)) {
    QVector<Texture *> specularMaps = LoadMaterialTextures(
        material, aiTextureType_SPECULAR, "texture_specular");
    std::copy(specularMaps.begin(), specularMaps.end(),
              std::back_inserter(textures));
  } else {
    Texture *texture = new Texture;
    QImage data(1, 1, QImage::Format_RGB32);
    data.fill(Qt::black);
    texture->texture.setData(data);
    texture->type = "texture_specular";
    texture->path = "";
    textures.push_back(texture);
    info_->textures.push_back(texture);
  }

  if (material->GetTextureCount(aiTextureType_HEIGHT)) {
    QVector<Texture *> heightMaps =
        LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    std::copy(heightMaps.begin(), heightMaps.end(),
              std::back_inserter(textures));
  }
}

void Model::LoadMaterial(const aiScene *scene, aiMesh *mesh,
                         Material &material) {
  aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
  aiColor3D color(0.0f, 0.0f, 0.0f);

  mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
  material.Ka = QVector3D(color.r, color.g, color.b);

  mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
  material.Kd = QVector3D(color.r, color.g, color.b);

  mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
  material.Ks = QVector3D(color.r, color.g, color.b);

  mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
  material.Ke = QVector3D(color.r, color.g, color.b);

  mat->Get(AI_MATKEY_SHININESS, material.Ns);

  mat->Get(AI_MATKEY_OPACITY, material.d);
}

QVector<Texture *> Model::LoadMaterialTextures(aiMaterial *mat,
                                               aiTextureType type,
                                               QString typeName) {
  QVector<Texture *> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
    aiString str;
    mat->GetTexture(type, i, &str);

    bool skip = false;
    for (unsigned int j = 0; j < info_->textures.size(); ++j) {
      if (std::strcmp(info_->textures[j]->path.toStdString().c_str(),
                      str.C_Str()) == 0) {
        textures.push_back(info_->textures[j]);
        skip = true;
        break;
      }
    }
    if (!skip) {
      Texture *texture = new Texture;
      QImage data(info_->GetDirectory().filePath(str.C_Str()));
      if (!data.isNull()) {
        texture->texture.setData(data);
        texture->type = typeName;
        texture->path = str.C_Str();
        textures.push_back(texture);
        info_->textures.push_back(texture);
      } else {
        emit Error(QString("Не удалось успешно загружать текстуру:" +
                           info_->GetDirectory().filePath(str.C_Str())));
      }
    }
  }
  return textures;
}

}  // namespace s21
