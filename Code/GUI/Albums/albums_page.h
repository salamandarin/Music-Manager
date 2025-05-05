// Samuel Sutton - 2025
#pragma once
#include <QWidget>

namespace Ui {
    class AlbumsPage;
}
class Core;
class MainWindow;
class AlbumWidget;
class Album;

class AlbumsPage : public QWidget {
    Q_OBJECT
public:
    AlbumsPage(Core& core, MainWindow* parent=nullptr);
    ~AlbumsPage();

    void build_album_grid();

private slots:
    void open_album_popup(int album_id);

private:
    AlbumWidget* create_album_widget(Album& album, QWidget* parent);

    Ui::AlbumsPage* ui;
    Core& core;
};