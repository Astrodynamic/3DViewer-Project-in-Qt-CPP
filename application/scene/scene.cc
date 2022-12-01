#include "scene.h"

namespace s21 {

Scene::Scene(QOpenGLShaderProgram *shader, QOpenGLShaderProgram *cube_shader)
    : VBO(QOpenGLBuffer::VertexBuffer),
      m_projection_type{ProjectionType::kPerspective},
      m_program{shader},
      m_program_cube{cube_shader},
      m_cube_texture(QOpenGLTexture::TargetCubeMap),
      cube_VBO(QOpenGLBuffer::VertexBuffer) {
  SetupMesh();
  x_axis_color = QColor(255, 0, 0);
  y_axis_color = QColor(0, 255, 0);
  z_axis_color = QColor(0, 0, 255);
  grid_color = QColor(125, 125, 125);
  m_background_color = QColor(255, 255, 255);
  LoadTextureCube(skybox);
}

Scene::~Scene() {
  VAO.destroy();
  VBO.destroy();
  cube_VAO.destroy();
  cube_VBO.destroy();
  m_cube_texture.release();
}

void Scene::DrawAxis() {
  VAO.bind();
  m_model.setToIdentity();

  m_program->setUniformValue("PointColor", x_axis_color);
  m_program->setUniformValue("model", m_model);
  glDrawArrays(GL_LINES, 0, 6);

  m_model.rotate(90.0f, 0.0f, 1.0f, 0.0f);
  m_program->setUniformValue("PointColor", y_axis_color);
  m_program->setUniformValue("model", m_model);
  glDrawArrays(GL_LINES, 0, 6);

  m_model.rotate(90.0f, 0.0f, 0.0f, 1.0f);
  m_program->setUniformValue("PointColor", z_axis_color);
  m_program->setUniformValue("model", m_model);
  glDrawArrays(GL_LINES, 0, 6);
  VAO.release();
}

void Scene::DrawGrid() {
  VAO.bind();
  m_program->setUniformValue("PointColor", grid_color);

  m_model.setToIdentity();
  m_model.translate(0.0f, 0.0f, -100.0f);
  for (int i = 0; i <= 200; ++i) {
    m_program->setUniformValue("model", m_model);
    glDrawArrays(GL_LINES, 0, 6);
    m_model.translate(0.0f, 0.0f, 1.0f);
  }

  m_model.setToIdentity();
  m_model.rotate(90.0, 0.0, 1.0, 0.0);
  m_model.translate(0.0f, 0.0f, -100.0f);

  for (int i = 0; i <= 200; ++i) {
    m_program->setUniformValue("model", m_model);
    glDrawArrays(GL_LINES, 0, 6);
    m_model.translate(0.0f, 0.0f, 1.0f);
  }
  VAO.release();
}

void Scene::DrawSkyBox() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  cube_VAO.bind();
  cube_VBO.bind();

  if (m_skybox_type == SkyboxType::kDraw) {
    m_cube_texture.bind(50);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  } else {
    m_cube_texture.release(50);
  }

  cube_VAO.release();
  cube_VBO.release();
}

QMatrix4x4 Scene::GetProjectionMat() {
  UpdateProjection();
  return m_projection;
}

QMatrix4x4 Scene::GetTransformMat() { return scene_transform.GetMatrix(); }

const QColor Scene::GetBackgroundColor() const { return m_background_color; }

DrawSceneType Scene::GetDrawType() const { return m_draw_type; }

SkyboxType Scene::GetSkyboxType() const { return m_skybox_type; }

const QOpenGLTexture &Scene::GetCubeTexture() const { return m_cube_texture; }

SceneTransformMatrix &Scene::GetSceneMat() { return scene_transform; }

void Scene::SetProjectionType(ProjectionType type) { m_projection_type = type; }

void Scene::SetSkyboxType(SkyboxType type) { m_skybox_type = type; }

void Scene::SetProjectionViewAngle(float angle) { m_view_angle = angle; }

void Scene::SetProjectionViewRatio(float ratio) { m_view_ratio = ratio; }

void Scene::SetBackgroundColor(QColor color) { m_background_color = color; }

void Scene::SetDrawType(DrawSceneType type) { m_draw_type = type; }

void Scene::UpdateProjection() {
  m_projection.setToIdentity();

  if (m_projection_type == ProjectionType::kPerspective) {
    m_projection.perspective(m_view_angle, m_view_ratio, 0.01f, 500.0f);
  } else if (m_projection_type == ProjectionType::kOrtho) {
    m_projection.ortho(-m_view_ratio, m_view_ratio, -1.0, 1.0, 0.01f, 500.0f);
  }
}

void Scene::LoadTextureCube(QString skybox[6]) {
  const QImage posx =
      QImage(skybox[0]).convertToFormat(QImage::Format_RGBA8888);
  const QImage negx =
      QImage(skybox[1]).convertToFormat(QImage::Format_RGBA8888);

  const QImage posy =
      QImage(skybox[2]).convertToFormat(QImage::Format_RGBA8888);
  const QImage negy =
      QImage(skybox[3]).convertToFormat(QImage::Format_RGBA8888);

  const QImage posz =
      QImage(skybox[4]).convertToFormat(QImage::Format_RGBA8888);
  const QImage negz =
      QImage(skybox[5]).convertToFormat(QImage::Format_RGBA8888);

  m_cube_texture.create();
  m_cube_texture.setSize(posx.width(), posx.height(), posx.depth());
  m_cube_texture.setFormat(QOpenGLTexture::RGBA8_UNorm);
  m_cube_texture.allocateStorage();

  m_cube_texture.setData(0, 0, QOpenGLTexture::CubeMapPositiveX,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         posx.constBits(), Q_NULLPTR);

  m_cube_texture.setData(0, 0, QOpenGLTexture::CubeMapPositiveY,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         posy.constBits(), Q_NULLPTR);

  m_cube_texture.setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         posz.constBits(), Q_NULLPTR);

  m_cube_texture.setData(0, 0, QOpenGLTexture::CubeMapNegativeX,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         negx.constBits(), Q_NULLPTR);

  m_cube_texture.setData(0, 0, QOpenGLTexture::CubeMapNegativeY,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         negy.constBits(), Q_NULLPTR);

  m_cube_texture.setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,
                         QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                         negz.constBits(), Q_NULLPTR);

  m_cube_texture.setWrapMode(QOpenGLTexture::ClampToEdge);
  m_cube_texture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  m_cube_texture.setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
}

void Scene::SetupMesh() {
  VAO.create();
  VAO.bind();

  VBO.create();
  VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

  VBO.bind();
  VBO.allocate(vertices, sizeof(vertices));

  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(float));

  VAO.release();
  VBO.release();

  cube_VAO.create();
  cube_VAO.bind();

  cube_VBO.create();
  VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);

  cube_VBO.bind();
  cube_VBO.allocate(cube_vertices, 36 * sizeof(QVector3D));

  m_program_cube->enableAttributeArray(0);
  m_program_cube->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

  m_program_cube->setUniformValue("skybox", 50);

  cube_VAO.release();
  cube_VBO.release();
}

QMatrix4x4 SceneTransformMatrix::GetMatrix() {
  TransformMatrix();
  return matrix;
}

void SceneTransformMatrix::SetTranslateX(float shift) { translate.Tx = shift; }

void SceneTransformMatrix::SetTranslateY(float shift) { translate.Ty = shift; }

void SceneTransformMatrix::SetTranslateZ(float shift) { translate.Tz = shift; }

void SceneTransformMatrix::SetRotateX(float rotate) {
  this->rotate.Rx = rotate;
}

void SceneTransformMatrix::SetRotateY(float rotate) {
  this->rotate.Ry = rotate;
}

void SceneTransformMatrix::SetRotateZ(float rotate) {
  this->rotate.Rz = rotate;
}

void SceneTransformMatrix::SetScaleX(float rotate) { this->scale.Sx = rotate; }

void SceneTransformMatrix::SetScaleY(float rotate) { this->scale.Sy = rotate; }

void SceneTransformMatrix::SetScaleZ(float rotate) { this->scale.Sz = rotate; }

void SceneTransformMatrix::SetScaleTotal(float rotate) {
  this->scale.STotal = rotate;
}

TranslateSetting &SceneTransformMatrix::GetTranslateSetting() {
  return translate;
}

RotateSetting &SceneTransformMatrix::GetRotateSetting() { return rotate; }

ScaleSetting &SceneTransformMatrix::GetScaleSetting() { return scale; }

void SceneTransformMatrix::TransformMatrix() {
  matrix.setToIdentity();

  matrix.translate(translate.Tx, translate.Ty, translate.Tz);

  matrix.rotate(rotate.Rx, 1.0f, 0.0f, 0.0f);
  matrix.rotate(rotate.Ry, 0.0f, 1.0f, 0.0f);
  matrix.rotate(rotate.Rz, 0.0f, 0.0f, 1.0f);

  matrix.scale(scale.Sx, scale.Sy, scale.Sz);
  matrix.scale(scale.STotal);
}

}  // namespace s21
