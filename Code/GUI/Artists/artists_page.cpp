// Samuel Sutton - 2025
#include "artists_page.h"
#include "ui_artists_page.h"
#include "core.h"
#include "main_window.h"
#include <QListWidgetItem>

ArtistsPage::ArtistsPage(Core& core, MainWindow* parent)
    :core{core}, QWidget{parent}, ui{new Ui::ArtistsPage} {
    
    ui->setupUi(this);
    build_list(); // fill in artists

    // connect page opened signal -> build list
    connect(parent, &MainWindow::artists_page_opened,
                    this, &ArtistsPage::build_list);
}

ArtistsPage::~ArtistsPage() {
    delete ui;
}

void ArtistsPage::build_list() {
    // clear list first
    ui->artists_list->clear();

    std::vector<Artist> artists = core.get_all_artists();

    for (const Artist& artist : artists) {
        new QListWidgetItem(QString::fromStdString(artist.name), ui->artists_list);
    }
}
