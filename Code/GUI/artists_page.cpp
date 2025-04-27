// Samuel Sutton - 2025
#include "artists_page.h"
#include "ui_artists_page.h"
#include "core.h"

ArtistsPage::ArtistsPage(Core& core, QWidget* parent)
    :core{core}, QWidget{parent}, ui{new Ui::ArtistsPage} {
    
    ui->setupUi(this);
    update_list(); // fill in artists
}

ArtistsPage::~ArtistsPage() {
    delete ui;
}

void ArtistsPage::update_list() {
    std::vector<Artist> artists = core.get_all_artists();

    for (const Artist& artist : artists) {
        new QListWidgetItem(QString::fromStdString(artist.name), ui->artists_list);
    }
}
