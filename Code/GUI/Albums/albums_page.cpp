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


// TODO: COPY TRACKS PAGE FOR UPDATING LIST WITH STUFF (& STORING ID INSIDE IT)
// TODO: USE MY CUSTOM IMAGE CLASS
// TODO: custom track row? (OR DO THAT AFTER ALBUMS + ARTISTS IMAGE LOGIC IS DONE & CAN EDIT ALL OF THE FIELDS)
