#include "track_popup.h"
#include "ui_track_popup.h"
#include "core.h"
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
    
    
}

TrackPopup::~TrackPopup() {
    delete ui;
}


void TrackPopup::update_data() {
    // get track info
    Track track = core.get_track(track_id);

    // -------------------- COVER ART --------------------
    // get image path (either actual image, or default)
    std::string image_path = "Code/Default_Images/default_track.jpg"; // default image
    if (!track.image_path.empty()) { // if has image
        image_path = track.image_path; // replace default image with real one
    }
    // set image
    QPixmap image(QString::fromStdString(image_path));
    if (image.isNull()) { // throw error if image failed to load
        throw std::runtime_error("Failed to load image from " + image_path);
    }
    ui->cover_art->setScaledContents(true); // let QLabel scale it
    ui->cover_art->setPixmap(image); // set cover art
    

    // -------------------- MAIN INFO --------------------
    setWindowTitle(QString::fromStdString(track.title)); // window title
    ui->title->setText(QString::fromStdString(track.title));
    ui->artist_name->setText(QString::fromStdString(track.artist));
    ui->album_title->setText(QString::fromStdString(track.album));
    ui->duration->setText(QString::fromStdString(track.duration.to_string()));


    // -------------------- TABS --------------------
    // TODO: SET TAB INFO
}

void TrackPopup::edit_title() {
    // grab new title
    std::string new_title = ui->edit_title_input->text().trimmed().toStdString(); // trim to remove extra whitespace

    // set new title
    core.set_track_title(track_id, new_title);

    // update gui
    update_data(); // update this popup
    emit track_updated(track_id); // emit signal so main table can refresh
}
