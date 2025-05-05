// Samuel Sutton - 2025
#pragma once
#include <QDialog>

namespace Ui {
    class AlbumPopup;
}
class Core;
class Album;

class AlbumPopup : public QDialog {
    Q_OBJECT
public:
    AlbumPopup(Core& core, Album& album, QWidget* parent=nullptr);
    ~AlbumPopup();

private:
    void set_data();
    
    Ui::AlbumPopup* ui;
    Core& core;
    Album& album;
};