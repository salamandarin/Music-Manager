// Samuel Sutton - 2025
#pragma once
#include <QDialog>

namespace Ui {
    class AddAlbumPopup;
}
class Core;

class AddAlbumPopup : public QDialog {
    Q_OBJECT
public:
    AddAlbumPopup(Core& core, QWidget* parent=nullptr);
    ~AddAlbumPopup();

signals:  
    void album_added();

private slots:
    void add_image_file();
    void add_album();

private:
    Ui::AddAlbumPopup* ui;
    Core& core;
    bool date_modified;
};
