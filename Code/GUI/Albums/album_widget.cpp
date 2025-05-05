// Samuel Sutton - 2025
#include "album_widget.h"
#include "ui_album_widget.h"
#include "core.h"
#include "gui_utils.h"
#include <QMouseEvent>

using namespace gui_utils;

AlbumWidget::AlbumWidget(Album& album, QWidget* parent)
    :album{album}, QWidget{parent}, ui{new Ui::AlbumWidget} {
    
    ui->setupUi(this);

    set_data();
}

AlbumWidget::~AlbumWidget() {
    delete ui;
}

void AlbumWidget::set_data() {
    set_image(album.image_path, DEFAULT_IMAGE, ui->cover_art);
    ui->album_title->setText(QString::fromStdString(album.title));
    ui->artist_name->setText(QString::fromStdString(album.artist));
}

// emit signal when double clicked
void AlbumWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    event->accept(); // mark event as handled
    emit album_double_clicked(album); // emit signal
}
