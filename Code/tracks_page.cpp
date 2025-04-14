#include "tracks_page.h"
#include "ui_tracks_page.h"
#include "core.h"

TracksPage::TracksPage(Core& core, QWidget* parent)
    :core{core}, QWidget{parent}, ui{new Ui::TracksPage} {
    
    ui->setupUi(this);
}

TracksPage::~TracksPage() {
    delete ui;
}
