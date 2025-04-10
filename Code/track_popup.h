// Samuel Sutton - 2025
#pragma once
#include <QDialog>

namespace Ui {
    class TrackPopup;
}
class Core;


class TrackPopup : public QDialog {
    Q_OBJECT
public:
    TrackPopup(Core& core, int track_id, QWidget* parent=nullptr);
    ~TrackPopup();

private:
    void set_labels();

    Ui::TrackPopup* ui;
    Core& core;
    int track_id;
};
