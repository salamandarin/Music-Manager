// Samuel Sutton - 2025
#include "albums_page.h"
#include "ui_albums_page.h"
#include "core.h"

AlbumsPage::AlbumsPage(Core& core, QWidget* parent)
    :core{core}, QWidget{parent}, ui{new Ui::AlbumsPage} {
    
    ui->setupUi(this);
}

AlbumsPage::~AlbumsPage() {
    delete ui;
}
