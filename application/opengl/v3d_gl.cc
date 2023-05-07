#include "v3d_gl.h"

namespace s21 {

V3D_GL::V3D_GL(QWidget *parent)
    : QOpenGLWidget{parent}, m_camera_(QVector3D(1.0, 1.0, 1.0)) {}

V3D_GL::~V3D_GL() {
  for (auto &&it : m_models_) {
    it->Destroy();
  }
  m_models_.clear();
  delete m_scene_;
}

void V3D_GL::LoadModel(QString file) {
  m_current_obj_ = Model::createModel(file);
  connect(m_current_obj_, SIGNAL(Error(QString)), this, SIGNAL(Error(QString)));
  m_models_.push_back(m_current_obj_);
  emit curentObj(m_current_obj_);
}

void V3D_GL::ChangeCurentObj(int index) {
  if (index >= 0 && index <= m_models_.size()) {
    m_current_obj_ = m_models_[index];
  } else {
    m_current_obj_ = nullptr;
  }
  emit curentObj(m_current_obj_);
}

void V3D_GL::RemoveObj(int index) {
  if (index >= 0 && index <= m_models_.size()) {
    m_models_.remove(index);
  }
}

void V3D_GL::ModelFocus() {
  if (m_current_obj_) {
    QVector3D center = m_current_obj_->GetInfo().GetCenterModelVertex();
    m_camera_.ChangeFocus(center);
  }
}

float V3D_GL::GetModelRatioToIndentify() {
  float ratio = 1.0;
  if (m_current_obj_) {
    ratio = m_current_obj_->GetInfo().GetRatioToIndentify();
  }
  return ratio;
}

void V3D_GL::keyPress(QKeyEvent *event) {
  m_camera_.ProcessKeyboard(event->key(), 1.0f);
}

void V3D_GL::GetCurrentObj() { emit curentObj(m_current_obj_); }

SceneTransformMatrix &V3D_GL::GetSceneData() { return m_scene_->GetSceneMat(); }

void V3D_GL::mouseMoveEvent(QMouseEvent *event) {
  float xoffset = event->position().toPoint().x() - m_last_pos_.x();
  float yoffset = m_last_pos_.y() - event->position().toPoint().y();

  m_camera_.ProcessMouseMovement(xoffset, yoffset, true);
  m_last_pos_ = event->position().toPoint();
}

void V3D_GL::wheelEvent(QWheelEvent *event) {
  m_camera_.ProcessMouseScroll(event->pixelDelta().y());
  m_scene_->SetProjectionViewAngle(m_camera_.GetZoom());
}

void V3D_GL::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_last_pos_ = event->position().toPoint();
  }
  event->accept();
}

void V3D_GL::initializeGL() {
  initializeOpenGLFunctions();
  LoadShaderProgram(m_shader_program_, ":/shader.vert", ":/shader.frag");
  LoadShaderProgram(m_shader_scene_, ":/scene.vert", ":/scene.frag");
  LoadShaderProgram(m_shader_vertex_, ":/vertex.vert", ":/vertex.frag");
  LoadShaderProgram(m_shader_edge_, ":/edge.vert", ":/edge.frag",
                    ":/edge.geom");
  LoadShaderProgram(m_shader_material_, ":/material.vert", ":/material.frag");
  LoadShaderProgram(m_shader_material_flat_, ":/material_flat.vert",
                    ":/material_flat.frag");
  LoadShaderProgram(m_shader_program_flat_, ":/shader_flat.vert",
                    ":/shader_flat.frag");
  LoadShaderProgram(m_shader_cubemap, ":/cubemap.vert", ":/cubemap.frag");

  m_scene_ = new Scene(&m_shader_scene_, &m_shader_cubemap);
  m_scene_->SetProjectionViewAngle(m_camera_.GetZoom());
  m_scene_->SetProjectionViewRatio((float)this->width() /
                                   (float)this->height());

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_TEXTURE_CUBE_MAP);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_MULTISAMPLE);

  set_fps(m_timer_, 60.0f);
  emit getSceneData();
}

void V3D_GL::resizeGL([[maybe_unused]] int width, [[maybe_unused]] int height) {
  float ratio = (float)this->width() / (float)this->height();
  m_scene_->SetProjectionViewRatio(ratio);

  m_shader_edge_.bind();
  m_shader_edge_.setUniformValue(
      "u_viewportInvSize",
      QVector2D((float)this->width(), (float)this->height()));
  m_shader_edge_.release();
}

void V3D_GL::SetProjection(ProjectionType type) {
  if (m_scene_) m_scene_->SetProjectionType(type);
}

void V3D_GL::SetSkybox(SkyboxType type) {
  if (m_scene_) m_scene_->SetSkyboxType(type);
}

void V3D_GL::SetBackgroundColor(QColor color) {
  if (m_scene_) m_scene_->SetBackgroundColor(color);
}

void V3D_GL::OnSurfaceHardSmooth() { glDisable(GL_MULTISAMPLE); }

void V3D_GL::OnSurfaceSoftSmooth() { glEnable(GL_MULTISAMPLE); }

void V3D_GL::SetLightType(LightType type) {
  m_illumination_.SetLightType(type);
}

void V3D_GL::SetDrawSceneType(DrawSceneType type) {
  if (m_scene_) m_scene_->SetDrawType(type);
}

void V3D_GL::paintGL() {
  QColor color(m_scene_->GetBackgroundColor());
  glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (m_illumination_.GetLightType() == LightType::kSoft) {
    DrawModelsMaterial(m_shader_material_);
    DrawModelsTexture(m_shader_program_);
  } else {
    DrawModelsMaterial(m_shader_material_flat_);
    DrawModelsTexture(m_shader_program_flat_);
  }

  DrawModelsEdge(m_shader_edge_);
  DrawModelsVertex(m_shader_vertex_);

  DrawScene(m_shader_scene_);
  DrawSkyBox(m_shader_cubemap);
}

void V3D_GL::LoadShaderProgram(QOpenGLShaderProgram &shader, QString vert,
                               QString frag, QString geom) {
  if (!shader.addShaderFromSourceFile(QOpenGLShader::Vertex, vert)) {
    emit Error(QString("failed add shader vertex"));
  }

  if (geom != nullptr) {
    if (!shader.addShaderFromSourceFile(QOpenGLShader::Geometry, geom)) {
      emit Error(QString("failed add shader geometry"));
    }
  }

  if (!shader.addShaderFromSourceFile(QOpenGLShader::Fragment, frag)) {
    emit Error(QString("failed add shader fragment"));
  }

  if (!shader.link()) {
    emit Error(QString("failed link"));
  }

  if (!shader.bind()) {
    emit Error(QString("failed bind"));
  }
}

void V3D_GL::DrawModelsMaterial(QOpenGLShaderProgram &shader) {
  shader.bind();
  shader.setUniformValue("projection", m_scene_->GetProjectionMat());
  shader.setUniformValue("view", m_camera_.GetViewMatrix());
  shader.setUniformValue("viewPos", m_camera_.GetPosition());

  LightsOn(shader);

  for (auto &it : m_models_) {
    shader.setUniformValue("model",
                           m_scene_->GetTransformMat() * it->GetModelMatrix());
    it->DrawMaterial(shader);
  }

  shader.release();
}

void V3D_GL::DrawModelsTexture(QOpenGLShaderProgram &shader) {
  shader.bind();
  shader.setUniformValue("projection", m_scene_->GetProjectionMat());
  shader.setUniformValue("view", m_camera_.GetViewMatrix());
  shader.setUniformValue("viewPos", m_camera_.GetPosition());

  LightsOn(shader);

  for (auto &it : m_models_) {
    shader.setUniformValue("model",
                           m_scene_->GetTransformMat() * it->GetModelMatrix());
    it->DrawTexture(shader);
  }

  shader.release();
}

void V3D_GL::LightsOn(QOpenGLShaderProgram &shader) {
  QMap<QString, QVector<QVariant> *> m_lights = m_illumination_.GetAllLight();
  for (auto &vector : m_lights) {
    QString type_vector(m_lights.key(vector));
    int count = 0;
    for (auto &it : *vector) {
      if (m_illumination_.HasItem(m_lights.key(vector), vector->indexOf(it))) {
        if (m_illumination_.ItemIsActive(it)) {
          QMap<QString, QVariant> light_info = m_illumination_.GetLightInfo(
              m_lights.key(vector), vector->indexOf(it));

          QString item_name = type_vector + "[" + QString::number(count) + "]";
          QString uniform;
          for (auto &property : light_info) {
            uniform = light_info.key(property).prepend(item_name);
            if (QString(property.typeName()) == "QVector3D*") {
              shader.setUniformValue(uniform.toStdString().c_str(),
                                     *property.value<QVector3D *>());
            } else {
              shader.setUniformValue(uniform.toStdString().c_str(),
                                     *property.value<float *>());
            }
          }
          count++;
        }
      }
    }
    shader.setUniformValue(QString("Count" + type_vector).toStdString().c_str(),
                           count);
  }
}

void V3D_GL::addLight(QString type) { m_illumination_.addLight(type); }

void V3D_GL::setEnableLight(QString type, int index) {
  m_illumination_.SetItemStatus(type, index, true);
}

void V3D_GL::setDisableLight(QString type, int index) {
  m_illumination_.SetItemStatus(type, index, false);
}

void V3D_GL::DrawModelsEdge(QOpenGLShaderProgram &shader) {
  shader.bind();
  shader.setUniformValue("projection", m_scene_->GetProjectionMat());
  shader.setUniformValue("view", m_camera_.GetViewMatrix());

  for (auto &it : m_models_) {
    shader.setUniformValue("model",
                           m_scene_->GetTransformMat() * it->GetModelMatrix());
    it->DrawEdge(shader);
  }
}

void V3D_GL::DrawModelsVertex(QOpenGLShaderProgram &shader) {
  shader.bind();
  shader.setUniformValue("projection", m_scene_->GetProjectionMat());
  shader.setUniformValue("view", m_camera_.GetViewMatrix());

  for (auto &it : m_models_) {
    shader.setUniformValue("model",
                           m_scene_->GetTransformMat() * it->GetModelMatrix());
    it->DrawVertex(shader);
  }
}

void V3D_GL::DrawScene(QOpenGLShaderProgram &shader) {
  if (m_scene_->GetDrawType() == DrawSceneType::kDraw) {
    shader.bind();

    shader.setUniformValue("projection", m_scene_->GetProjectionMat());
    shader.setUniformValue("view", m_camera_.GetViewMatrix());

    m_scene_->DrawAxis();
    m_scene_->DrawGrid();
  }
}

void V3D_GL::DrawSkyBox(QOpenGLShaderProgram &shader) {
  glDepthFunc(GL_LEQUAL);
  shader.bind();

  shader.setUniformValue("projection", m_scene_->GetProjectionMat());

  shader.setUniformValue(
      "view", QMatrix4x4(m_camera_.GetViewMatrix().toGenericMatrix<3, 3>()));

  m_scene_->DrawSkyBox();
  glDepthFunc(GL_LESS);
}

void V3D_GL::set_fps(QTimer *timer, GLfloat fps) {
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(1000 / fps);
}

}  // namespace s21
