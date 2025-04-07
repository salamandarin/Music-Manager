// Samuel Sutton - 2025
#pragma once
#include "core.h"
#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(Core& core, QWidget* parent=nullptr);

private:
    void setup_gui();
    void load_tracks();

    Core& core;
    QTableWidget* tracks_table;
};
