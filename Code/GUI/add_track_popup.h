// Samuel Sutton - 2025
#pragma once
#include <QDialog>

namespace Ui {
    class AddTrackPopup;
}
class Core;

class AddTrackPopup : public QDialog {
    Q_OBJECT

public:
    AddTrackPopup(Core& core, QWidget* parent=nullptr);
    ~AddTrackPopup();

signals:  
    void tracks_added();

private slots:
    void add_music_file();
    void add_image_file();
    void add_track();

private:
    Ui::AddTrackPopup* ui;
    Core& core;
    bool date_modified;
};
