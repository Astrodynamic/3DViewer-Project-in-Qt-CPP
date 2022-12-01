#ifndef V3D_GL_H
#define V3D_GL_H

#include <QKeyEvent>
#include <QMatrix4x4>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShader>
#include <QOpenGLWidget>
#include <QTimer>
#include <QtMath>

#include "camera.h"
#include "illumination.h"
#include "model.h"
#include "scene.h"

namespace s21 {

class V3D_GL : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
  Q_OBJECT

 public:
  explicit V3D_GL(QWidget *parent = nullptr);
  virtual ~V3D_GL();

  void SetProjection(ProjectionType type);
  void SetSkybox(SkyboxType type);
  void SetBackgroundColor(QColor color);
  void OnSurfaceHardSmooth();
  void OnSurfaceSoftSmooth();

  void SetLightType(LightType type);
  void SetDrawSceneType(DrawSceneType type);

  void LoadModel(QString file);
  void keyPress(QKeyEvent *event);
  void GetCurrentObj();
  SceneTransformMatrix &GetSceneData();

  virtual void mouseMoveEvent(QMouseEvent *) override;
  virtual void wheelEvent(QWheelEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;

  void ChangeCurentObj(int index);
  void RemoveObj(int index);

  void ModelFocus();
  float GetModelRatioToIndentify();
  Illumination *GetIllumation() { return &m_illumination_; }

 protected:
  virtual void initializeGL() override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL() override;

 private:
  void LoadShaderProgram(QOpenGLShaderProgram &shader, QString vert,
                         QString frag, QString geom = nullptr);
  void DrawModelsMaterial(QOpenGLShaderProgram &shader);
  void DrawModelsTexture(QOpenGLShaderProgram &shader);
  void DrawModelsEdge(QOpenGLShaderProgram &shader);
  void DrawModelsVertex(QOpenGLShaderProgram &shader);
  void DrawScene(QOpenGLShaderProgram &shader);
  void DrawSkyBox(QOpenGLShaderProgram &shader);
  void set_fps(QTimer *timer, GLfloat fps);

  void LightsOn(QOpenGLShaderProgram &shader);

  QOpenGLShaderProgram m_shader_program_;
  QOpenGLShaderProgram m_shader_scene_;
  QOpenGLShaderProgram m_shader_vertex_;
  QOpenGLShaderProgram m_shader_edge_;
  QOpenGLShaderProgram m_shader_material_;
  QOpenGLShaderProgram m_shader_material_flat_;
  QOpenGLShaderProgram m_shader_program_flat_;
  QOpenGLShaderProgram m_shader_cubemap;

  Scene *m_scene_ = nullptr;
  Camera m_camera_;

  QVector<Model *> m_models_;
  Model *m_current_obj_ = nullptr;

  Illumination m_illumination_;

  QPoint m_last_pos_;
  QTimer *m_timer_;

 signals:
  void curentObj(Model *);
  void getSceneData();
  void Error(QString);

 public slots:
  void addLight(QString);
  void setEnableLight(QString, int);
  void setDisableLight(QString, int);
};

}  // namespace s21

#endif  // V3D_GL_H
