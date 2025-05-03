// Samuel Sutton - 2025
#include "track_popup.h"
#include "ui_track_popup.h"
#include "core.h"
#include "gui_utils.h"
#include <QPushButton>

TrackPopup::TrackPopup(Core& core, int track_id, QWidget* parent)
    :core{core}, track_id{track_id}, QDialog{parent}, ui{new Ui::TrackPopup} {

    // set up gui
    ui->setupUi(this);
    update_data(); // set all values

    ui->button_box->button(QDialogButtonBox::Close)->setAutoDefault(false); // make it so enter key doesn't close popup

    // connect signals to functions 
    connect(ui->edit_title_input, &QLineEdit::editingFinished,
                            this, &TrackPopup::edit_title);
    connect(ui->edit_artist_input, &QLineEdit::editingFinished,
                            this, &TrackPopup::edit_artist);
    connect(ui->edit_album_input, &QLineEdit::editingFinished,
                            this, &TrackPopup::edit_album);
    connect(ui->edit_date_input, &QDateEdit::editingFinished,
                            this, &TrackPopup::edit_date);
    connect(ui->edit_tracklist_num_input, &QLineEdit::editingFinished,
                            this, &TrackPopup::edit_tracklist_num);
}

TrackPopup::~TrackPopup() {
    delete ui;
}


void TrackPopup::update_data() {
    // get track info
    Track track = core.get_track(track_id);

    // -------------------- COVER ART --------------------
    // set cover art (either actual or default)
    set_image(track.image_path, DEFAULT_TRACK_IMAGE, ui->cover_art);

    // -------------------- MAIN INFO --------------------
    setWindowTitle(QString::fromStdString(track.title)); // window title
    ui->title->setText(QString::fromStdString(track.title));
    ui->artist_name->setText(QString::fromStdString(track.artist));
    ui->album_title->setText(QString::fromStdString(track.album));
    ui->duration->setText(QString::fromStdString(track.duration.to_string()));

    // -------------------- INFO TAB --------------------
    // TODO: album cover art
    ui->info_album_title->setText(QString::fromStdString(track.album));
    // TODO: album type
    // TODO: album year

    // TODO: artist image
    ui->info_artist_name->setText(QString::fromStdString(track.artist));
    // TODO: artist person hebind

    ui->date->setText(QString::fromStdString(track.date.to_string()));
    ui->duration_smaller->setText(QString::fromStdString(track.duration.to_string()));
    // TODO: category
    // TODO: description
    ui->tracklist_num->setText(QString::number(track.tracklist_num));

    // -------------------- FILE INFO TAB --------------------
    // TODO: file type
    ui->file_path->setText(QString::fromStdString(track.file_path));
    ui->image_path->setText(QString::fromStdString(track.image_path));
    // TODO: FINISH

    // TODO: NOTES TAB
}

void TrackPopup::edit_title() {
    // grab new data
    std::string new_title = ui->edit_title_input->text().trimmed().toStdString(); // trim to remove extra whitespace

    // set new data
    core.set_track_title(track_id, new_title);

    // update gui
    update_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
void TrackPopup::edit_artist() {
    // grab new data
    std::string new_artist = ui->edit_artist_input->text().trimmed().toStdString(); // trim to remove extra whitespace

    // set new data
    core.set_track_artist(track_id, new_artist);

    // update gui
    update_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
void TrackPopup::edit_album() {
    // grab new data
    std::string new_album = ui->edit_album_input->text().trimmed().toStdString(); // trim to remove extra whitespace

    // set new data
    core.set_track_album(track_id, new_album);

    // update gui
    update_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
void TrackPopup::edit_date() {
    // grab new data
    QDate qt_date = ui->edit_date_input->date();
    Date new_date{qt_date.month(), qt_date.day(), qt_date.year()};

    // set new data
    core.set_track_date(track_id, new_date);

    // update gui
    update_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
void TrackPopup::edit_tracklist_num() {
    // grab new data
    int new_tracklist_num = ui->edit_tracklist_num_input->text().trimmed().toInt(); // trim to remove extra whitespace
    // TODO: handle non-int input (or leave be cuz just sets to 0)

    // set new data
    core.set_track_tracklist_num(track_id, new_tracklist_num);

    // update gui
    update_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
