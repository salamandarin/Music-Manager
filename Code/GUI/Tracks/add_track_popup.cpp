// Samuel Sutton - 2025
#include "add_track_popup.h"
#include "ui_add_track_popup.h"
#include "core.h"
#include <QFileDialog>

AddTrackPopup::AddTrackPopup(Core& core, QWidget* parent)
    :core{core}, QDialog{parent}, ui{new Ui::AddTrackPopup} {
    
    ui->setupUi(this);

    // connect signals to functions 
    connect(ui->choose_music_file_button, &QPushButton::clicked,
                                this, &AddTrackPopup::add_music_file);
    connect(ui->choose_image_file_button, &QPushButton::clicked,
                                this, &AddTrackPopup::add_image_file);
    connect(ui->button_box, &QDialogButtonBox::accepted,
                            this, &AddTrackPopup::add_track);
    // date signals
    connect(ui->date_input, &QDateEdit::dateChanged,
                            this, [this]() {
        date_modified = true;
    });
    connect(ui->clear_date_button, &QPushButton::clicked,
                            this, [this]() {
        date_modified = false;
        ui->date_input->setDate(QDate(2025, 1, 1));
    });
}

AddTrackPopup::~AddTrackPopup() {
    delete ui;
}

void AddTrackPopup::add_music_file() {
    std::string file_path = QFileDialog::getOpenFileName(this, "Select Music File",
                    "", "Audio Files (*.mp3 *.wav *.aiff *.aif *.flac *.m4a *.aac)").toStdString();
    if (!file_path.empty()) {
        core.add_track(file_path); // add track

        emit tracks_added(); // emit signal to update table GUI
        
        accept(); // close popup
    }
}
void AddTrackPopup::add_image_file() {
    QString file_path = QFileDialog::getOpenFileName(this, "Select Image File",
                                        "", "Images (*.png *.jpg *jpeg)");

    if (!file_path.isEmpty()) {
        // set text of image_path label so it can be grabbed later
        ui->image_path->setText(file_path);
    }
}

void AddTrackPopup::add_track() {
    // grab info (trimming to remove extra whitespace)
    Track track;
    track.image_path = ui->image_path->text().trimmed().toStdString();
    track.title = ui->title_input->text().trimmed().toStdString();
    track.artist = ui->artist_input->text().trimmed().toStdString();
    track.album = ui->album_input->text().trimmed().toStdString();
    // check if date was entered
    if (date_modified) {
        QDate qt_date = ui->date_input->date();
        track.date = Date(qt_date.month(), qt_date.day(), qt_date.year());
    }
    track.tracklist_num = ui->tracklist_num_input->text().trimmed().toInt(); // TODO: handle non-int input (or leave be cuz just sets to 0)
    
    core.add_track(track); // add track

    emit tracks_added(); // emit signal to update table GUI
}
