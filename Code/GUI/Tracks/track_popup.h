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

signals:  
    void track_updated(int track_id);

private slots:
    void edit_title();
    void edit_artist();
    void edit_album();
    void edit_date();
    void clear_date();
    void edit_tracklist_num();

private:
    void set_data();

    Ui::TrackPopup* ui;
    Core& core;
    int track_id;
};
