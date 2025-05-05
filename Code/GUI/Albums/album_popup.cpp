// Samuel Sutton - 2025
#include "album_popup.h"
#include "ui_album_popup.h"
#include "core.h"
#include "gui_utils.h"

using namespace gui_utils;

AlbumPopup::AlbumPopup(Core& core, Album& album, QWidget* parent)
    :core{core}, album{album}, QDialog{parent}, ui{new Ui::AlbumPopup} {
    
    ui->setupUi(this);
    set_data();
}

AlbumPopup::~AlbumPopup() {
    delete ui;
}

void AlbumPopup::set_data() {
    set_image(album.image_path, DEFAULT_TRACK_IMAGE, ui->cover_art);
    ui->title->setText(QString::fromStdString(album.title));
    ui->artist_name->setText(QString::fromStdString(album.artist));
    ui->date->setText(QString::fromStdString(album.date.to_string()));

    // TODO: calculate total duration (+ make Duration.to_long_string())

    // TODO: fill in tracks table
    // TODO: make tracks table a widget cuz using EXACT thing here !!!!!!!
}