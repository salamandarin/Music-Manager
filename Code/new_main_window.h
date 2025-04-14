#pragma once
#include <QMainWindow>

namespace Ui {
    class NewMainWindow;
}
class Core;
class TracksPage;
class AlbumsPage;
class ArtistsPage;

class NewMainWindow : public QMainWindow {
    Q_OBJECT

public:
    NewMainWindow(Core& core, QWidget* parent=nullptr);
    ~NewMainWindow();

private slots:
    void show_tracks_page();
    void show_albums_page();
    void show_artists_page();
    void show_settings();

private:
    Ui::NewMainWindow* ui;
    Core& core;
    TracksPage* tracks_page;
    AlbumsPage* albums_page;
    ArtistsPage* artists_page;
};