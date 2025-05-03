// Samuel Sutton - 2025
#pragma once
#include <QWidget>

namespace Ui {
    class TracksPage;
}
class Core;

class TracksPage : public QWidget {
    Q_OBJECT

public:
    TracksPage(Core& core, QWidget* parent=nullptr);
    ~TracksPage();

private slots:
    // buttons
    void add_track_files();
    void add_tracks_from_folder();
    void manually_add_track();
    void delete_library();

    // track popup
    void open_track_popup(int row, int column);
    void update_track(int track_id); 

private:
    // table stuff
    void setup_table();
    void update_table();
    void update_row(int row, int track_id);

    // helper functions
    int get_track_row(int track_id);
    int get_track_id(int row);

    // data
    Ui::TracksPage* ui;
    Core& core;
};