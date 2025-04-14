#include "artists_page.h"
#include "ui_artists_page.h"
#include "core.h"

ArtistsPage::ArtistsPage(Core& core, QWidget* parent)
    :core{core}, QWidget{parent}, ui{new Ui::ArtistsPage} {
    
    ui->setupUi(this);
}

ArtistsPage::~ArtistsPage() {
    delete ui;
}
