// Samuel Sutton - 2025
#pragma once
#include <QDialog>

namespace Ui {
    class AlbumPopup;
}
class Core;

class AlbumPopup : public QDialog {
    Q_OBJECT
public:
    AlbumPopup(Core& core, int album_id, QWidget* parent=nullptr);
    ~AlbumPopup();

private:
    void set_data();
    
    Ui::AlbumPopup* ui;
    Core& core;
    int album_id;
};