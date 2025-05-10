// Samuel Sutton - 2025
#include "add_album_popup.h"
#include "ui_add_album_popup.h"
#include "core.h"
#include <QFileDialog>

AddAlbumPopup::AddAlbumPopup(Core& core, QWidget* parent)
    :core{core}, QDialog{parent}, ui{new Ui::AddAlbumPopup} {
    
    ui->setupUi(this);

    // connect signals to functions 
    connect(ui->choose_image_file_button, &QPushButton::clicked,
                                this, &AddAlbumPopup::add_image_file);
    connect(ui->button_box, &QDialogButtonBox::accepted,
                            this, &AddAlbumPopup::add_album);
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

AddAlbumPopup::~AddAlbumPopup() {
    delete ui;
}

void AddAlbumPopup::add_image_file() {
    QString file_path = QFileDialog::getOpenFileName(this, "Select Image File",
                                        "", "Images (*.png *.jpg *jpeg)");

    if (!file_path.isEmpty()) {
        // set text of image_path label so it can be grabbed later
        ui->image_path->setText(file_path);
    }
}

void AddAlbumPopup::add_album() {
    // grab info (trimming to remove extra whitespace)
    Album album;
    album.image_path = ui->image_path->text().trimmed().toStdString();
    album.title = ui->title_input->text().trimmed().toStdString(); // TODO: WILL CRASH IF NULL!!!!
    album.artist = ui->artist_input->text().trimmed().toStdString(); // TODO: make dropdown to get id???
    album.type = ui->type_input->currentText().trimmed().toStdString(); // TODO: make dropdown to get id???
    // check if date was entered
    if (date_modified) {
        QDate qt_date = ui->date_input->date();
        album.date = Date(qt_date.month(), qt_date.day(), qt_date.year());
    }
    
    core.add_album(album); // add album

    emit album_added(); // emit signal to update GUI
}
