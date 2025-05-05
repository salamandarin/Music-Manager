// Samuel Sutton - 2025
#pragma once
#include <QWidget>

namespace Ui {
    class AlbumsPage;
}
class Core;
class AlbumWidget;
class Album;

class AlbumsPage : public QWidget {
    Q_OBJECT
public:
    AlbumsPage(Core& core, QWidget* parent=nullptr);
    ~AlbumsPage();

private slots:
    void open_album_popup(Album& album);

private:
    void build_album_grid();
    AlbumWidget* create_album_widget(Album& album, QWidget* parent);

    Ui::AlbumsPage* ui;
    Core& core;
};