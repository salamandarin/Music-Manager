// Samuel Sutton - 2025
#pragma once
#include "core.h"
#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(Core& core, QWidget* parent=nullptr);

private slots:
    void open_track_popup(int row, int column);
    void update_track(int track_id); 

private:
    void setup_gui();
    void update_table();
    void update_row(int row, int track_id);
    
    // helper functions
    int get_track_row(int track_id);
    int get_track_id(int row);

    // data
    Core& core;
    QTableWidget* tracks_table;
};
