// Samuel Sutton - 2025
#include "album_popup.h"
#include "ui_album_popup.h"
#include "core.h"
#include "album.h"
#include "gui_utils.h"
#include <QpushButton>

using namespace gui_utils;

AlbumPopup::AlbumPopup(Core& core, int album_id, QWidget* parent)
    :core{core}, album_id{album_id}, QDialog{parent}, ui{new Ui::AlbumPopup} {
    
    ui->setupUi(this);
    set_data();

    ui->button_box->button(QDialogButtonBox::Close)->setDefault(true); // make it so enter key DOES close popup
    ui->button_box->button(QDialogButtonBox::Close)->setAutoDefault(true); // make it so enter key DOES close popup
}

AlbumPopup::~AlbumPopup() {
    delete ui;
}

void AlbumPopup::set_data() {
    Album album = core.get_album(album_id); // get info

    set_image(album.image_path, DEFAULT_TRACK_IMAGE, ui->cover_art);
    ui->title->setText(QString::fromStdString(album.title));
    ui->artist_name->setText(QString::fromStdString(album.artist));
    ui->date->setText(QString::fromStdString(album.date.to_string()));
}
