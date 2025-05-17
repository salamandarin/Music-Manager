// Samuel Sutton - 2025
#pragma once
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}
class Core;
class TracksPage;
class AlbumsPage;
class ArtistsPage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(Core& core, QWidget* parent=nullptr);
    ~MainWindow();

signals:
    void albums_page_opened();
    void artists_page_opened();
    void library_deleted();

private slots:
    void show_tracks_page();
    void show_albums_page();
    void show_artists_page();
    void show_settings();

private:
    Ui::MainWindow* ui;
    Core& core;
    TracksPage* tracks_page;
    AlbumsPage* albums_page;
    ArtistsPage* artists_page;
};