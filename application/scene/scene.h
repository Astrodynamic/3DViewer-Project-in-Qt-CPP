#ifndef SCENE_H_
#define SCENE_H_

#include <QColor>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

#include "model_settings.h"

namespace s21 {

enum class ProjectionType { kOrtho = 0, kPerspective };
enum class SkyboxType { kNo = 0, kDraw };
enum class DrawSceneType { kNo = 0, kDraw };

struct SceneTransformMatrix {
 private:
  QMatrix4x4 matrix;
  TranslateSetting translate{0.0f, 0.0f, 0.0f};
  RotateSetting rotate{0.0f, 0.0f, 0.0f};
  ScaleSetting scale{1.0f, 1.0f, 1.0f, 1.0f};

 public:
  QMatrix4x4 GetMatrix();
  void SetTranslateX(float shift);
  void SetTranslateY(float shift);
  void SetTranslateZ(float shift);

  void SetRotateX(float rotate);
  void SetRotateY(float rotate);
  void SetRotateZ(float rotate);

  void SetScaleX(float rotate);
  void SetScaleY(float rotate);
  void SetScaleZ(float rotate);
  void SetScaleTotal(float rotate);

  TranslateSetting &GetTranslateSetting();
  RotateSetting &GetRotateSetting();
  ScaleSetting &GetScaleSetting();

 private:
  void TransformMatrix();
};

class Scene {
 private:
  QOpenGLVertexArrayObject VAO;
  QOpenGLBuffer VBO, EBO;

  QMatrix4x4 m_model;
  SceneTransformMatrix scene_transform;

  QMatrix4x4 m_projection;
  ProjectionType m_projection_type;
  float m_view_angle = 45.0f;
  float m_view_ratio = 1.0f;

  QOpenGLShaderProgram *m_program;
  QOpenGLShaderProgram *m_program_cube;

  float vertices[6] = {
      100.0f, 0.0f, 0.0f, -100.0f, 0.0f, 0.0f,
  };

  QColor x_axis_color;
  QColor y_axis_color;
  QColor z_axis_color;

  QColor grid_color;

  QColor m_background_color;
  SkyboxType m_skybox_type = SkyboxType::kDraw;
  DrawSceneType m_draw_type = DrawSceneType::kNo;

  QString skybox[6] = {":/right.jpg",  ":/left.jpg",  ":/top.jpg",
                       ":/bottom.jpg", ":/front.jpg", ":/back.jpg"};

  QOpenGLTexture m_cube_texture;
  QOpenGLVertexArrayObject cube_VAO;
  QOpenGLBuffer cube_VBO;

  const QVector3D cube_vertices[36]{
      {-1.0f, 1.0f, -1.0f},  {-1.0f, -1.0f, -1.0f}, {+1.0f, -1.0f, -1.0f},
      {+1.0f, -1.0f, -1.0f}, {+1.0f, +1.0f, -1.0f}, {-1.0f, +1.0f, -1.0f},

      {-1.0f, -1.0f, +1.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f, +1.0f, -1.0f},
      {-1.0f, +1.0f, -1.0f}, {-1.0f, +1.0f, +1.0f}, {-1.0f, -1.0f, +1.0f},

      {+1.0f, -1.0f, -1.0f}, {+1.0f, -1.0f, +1.0f}, {+1.0f, +1.0f, +1.0f},
      {+1.0f, +1.0f, +1.0f}, {+1.0f, +1.0f, -1.0f}, {+1.0f, -1.0f, -1.0f},

      {-1.0f, -1.0f, +1.0f}, {-1.0f, +1.0f, +1.0f}, {+1.0f, +1.0f, +1.0f},
      {+1.0f, +1.0f, +1.0f}, {+1.0f, -1.0f, +1.0f}, {-1.0f, -1.0f, +1.0f},

      {-1.0f, +1.0f, -1.0f}, {+1.0f, +1.0f, -1.0f}, {+1.0f, +1.0f, +1.0f},
      {+1.0f, +1.0f, +1.0f}, {-1.0f, +1.0f, +1.0f}, {-1.0f, +1.0f, -1.0f},

      {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, +1.0f}, {+1.0f, -1.0f, -1.0f},
      {+1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, +1.0f}, {+1.0f, -1.0f, +1.0f}};

 public:
  explicit Scene(QOpenGLShaderProgram *shader,
                 QOpenGLShaderProgram *cube_shader);
  ~Scene();

  void DrawAxis();
  void DrawGrid();
  void DrawSkyBox();

  QMatrix4x4 GetProjectionMat();
  QMatrix4x4 GetTransformMat();
  const QColor GetBackgroundColor() const;
  DrawSceneType GetDrawType() const;
  SkyboxType GetSkyboxType() const;
  const QOpenGLTexture &GetCubeTexture() const;

  SceneTransformMatrix &GetSceneMat();

  void SetProjectionType(ProjectionType type);
  void SetSkyboxType(SkyboxType type);
  void SetProjectionViewAngle(float angle);
  void SetProjectionViewRatio(float ratio);
  void SetBackgroundColor(QColor color);
  void SetDrawType(DrawSceneType type);

 private:
  void SetupMesh();
  void UpdateProjection();
  void LoadTextureCube(QString skybox[6]);
};

}  // namespace s21

#endif  // SCENE_H_
