// Samuel Sutton - 2025
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

signals:
    void library_deleted();

private slots:
    void restore_defaults();
    void delete_library();

private:
    void update_checkboxes();
    
    Ui::SettingsPopup* ui;
    Core& core;
};