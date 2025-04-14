#pragma once
#include <QDialog>

namespace Ui {
    class SettingsPopup;
}
class Core;

class SettingsPopup : public QDialog {
    Q_OBJECT

public:
    SettingsPopup(Core& core, QWidget* parent=nullptr);
    ~SettingsPopup();

private:
    Ui::SettingsPopup* ui;
    Core& core;
};