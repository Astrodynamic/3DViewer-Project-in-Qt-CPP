#ifndef GLOBAL_SETTINGS_H
#define GLOBAL_SETTINGS_H

#include <QSettings>

namespace s21 {

class GlobalSetting {
 public:
  static const QVariant getSettings(const QString param) {
    QSettings settings{"Ajhin_team", "3D_viewer.v2"};
    return settings.value(param);
  }
  static void setSettings(const QString param, const QVariant value) {
    QSettings settings{"Ajhin_team", "3D_viewer.v2"};
    settings.setValue(param, value);
    settings.sync();
  }
  static bool haveSettings(const QString param) {
    QSettings settings{"Ajhin_team", "3D_viewer.v2"};
    return settings.contains(param);
  }
};

}  // namespace s21

#endif  // GLOBAL_SETTINGS_H
