#include "track_popup.h"
#include "ui_track_popup.h"
#include "core.h"

TrackPopup::TrackPopup(Core& core, int track_id, QWidget* parent)
    :core{core}, track_id{track_id}, QDialog{parent}, ui{new Ui::TrackPopup} {

    ui->setupUi(this);
    set_labels();
}

TrackPopup::~TrackPopup() {
    delete ui;
}


void TrackPopup::set_labels() {
    Track track = core.get_track(track_id);

    setWindowTitle(QString::fromStdString(track.title));
    
    ui->title->setText(QString::fromStdString(track.title));
    ui->artist_name->setText(QString::fromStdString(track.artist));
    ui->album_title->setText(QString::fromStdString(track.album));
    ui->duration->setText(QString::fromStdString(track.duration.to_string()));
}
