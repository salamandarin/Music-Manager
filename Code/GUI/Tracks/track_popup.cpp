// Samuel Sutton - 2025
#include "track_popup.h"
#include "ui_track_popup.h"
#include "core.h"
#include "gui_utils.h"
#include <QPushButton>

using namespace gui_utils;

TrackPopup::TrackPopup(Core& core, int track_id, QWidget* parent)
    :core{core}, track_id{track_id}, QDialog{parent}, ui{new Ui::TrackPopup} {

    // set up gui
    ui->setupUi(this);
    set_data(); // set all values

    ui->button_box->button(QDialogButtonBox::Close)->setAutoDefault(false); // make it so enter key doesn't close popup
    ui->clear_date_button->setDefault(false); // make enter key not press clear_date
    ui->clear_date_button->setAutoDefault(false); // make enter key not press clear_date

    // connect signals to functions 
    connect(ui->edit_title_input, &QLineEdit::editingFinished,
                            this, &TrackPopup::edit_title);
    connect(ui->edit_artist_input, &QLineEdit::editingFinished,
                            this, &TrackPopup::edit_artist);
    connect(ui->edit_album_input, &QLineEdit::editingFinished,
                            this, &TrackPopup::edit_album);
    connect(ui->edit_date_input, &QDateEdit::editingFinished,
                            this, &TrackPopup::edit_date);
    connect(ui->clear_date_button, &QPushButton::clicked,
                            this, &TrackPopup::clear_date);
    connect(ui->edit_tracklist_num_input, &QLineEdit::editingFinished,
                            this, &TrackPopup::edit_tracklist_num);
}

TrackPopup::~TrackPopup() {
    delete ui;
}


void TrackPopup::set_data() {
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
    ui->track_id->setText(QString::number(track.id));
    ui->info_title->setText(QString::fromStdString(track.title));
    ui->info_artist->setText(QString::fromStdString(track.artist));
    ui->info_album->setText(QString::fromStdString(track.album));
    ui->info_duration->setText(QString::fromStdString(track.duration.to_string()));
    ui->info_date->setText(QString::fromStdString(track.date.to_string()));
    ui->info_tracklist_num->setText(QString::number(track.tracklist_num));
    ui->info_file_path->setText(QString::fromStdString(track.file_path));
    ui->info_image_path->setText(QString::fromStdString(track.image_path));
}

void TrackPopup::edit_title() {
    // grab new data
    std::string new_title = ui->edit_title_input->text().trimmed().toStdString(); // trim to remove extra whitespace

    // set new data
    core.set_track_title(track_id, new_title);

    // update gui
    set_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
void TrackPopup::edit_artist() {
    // grab new data
    std::string new_artist = ui->edit_artist_input->text().trimmed().toStdString(); // trim to remove extra whitespace

    // set new data
    core.set_track_artist(track_id, new_artist);

    // update gui
    set_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
void TrackPopup::edit_album() {
    // grab new data
    std::string new_album = ui->edit_album_input->text().trimmed().toStdString(); // trim to remove extra whitespace

    // set new data
    core.set_track_album(track_id, new_album);

    // update gui
    set_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
void TrackPopup::edit_date() {
    // grab new data
    QDate qt_date = ui->edit_date_input->date();
    Date new_date{qt_date.month(), qt_date.day(), qt_date.year()};

    // set new data
    core.set_track_date(track_id, new_date);

    // update gui
    set_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
void TrackPopup::clear_date() {
    // reset input to default date
    ui->edit_date_input->setDate(QDate(2025, 1, 1));

    // set null / default date
    core.set_track_date(track_id, Date{});

    // update gui
    set_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
void TrackPopup::edit_tracklist_num() {
    // grab new data (non-int input just sets to 0)
    int new_tracklist_num = ui->edit_tracklist_num_input->text().trimmed().toInt(); // trim to remove extra whitespace

    // set new data
    core.set_track_tracklist_num(track_id, new_tracklist_num);

    // update gui
    set_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
