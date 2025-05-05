// Samuel Sutton - 2025
#include "albums_page.h"
#include "ui_albums_page.h"
#include "core.h"
#include "main_window.h"
#include "album_widget.h"
#include "album_popup.h"

AlbumsPage::AlbumsPage(Core& core, MainWindow* parent)
    :core{core}, QWidget{parent}, ui{new Ui::AlbumsPage} {
    
    ui->setupUi(this);
    build_album_grid(); // make album grid
}

AlbumsPage::~AlbumsPage() {
    delete ui;
}

void AlbumsPage::build_album_grid() {
    // disable GUI updates until done (so not slowing down)
    ui->scroll_area_widget_contents->setUpdatesEnabled(false);

    // clear grid first
    QLayoutItem* child;
    while ((child = ui->grid_layout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }

    // grab all albums, fill in grid
    std::vector<Album> albums = core.get_all_albums(); // get all albums
    for (int i = 0; i < albums.size(); ++i) {
        // make album widget
        AlbumWidget* album_widget = create_album_widget(albums[i], ui->scroll_area_widget_contents); // pass in parent

        int area_width = ui->scrollArea->maximumViewportSize().width() - 15;
        const int columns = 4;
        const int spacing = 10;
        const int margins = 5;
        const double widget_height_ratio = 1.5;
        // calculate widget width & height
        int widget_width = (area_width - (2 * margins) - ((columns - 1) * spacing)) / columns;
        int widget_height = static_cast<int>(widget_width * widget_height_ratio);

        // set album widget size
        album_widget->setFixedSize(widget_width, widget_height); // set album widget size

        // add album to grid layout
        const int row = i / columns;
        const int column = i % columns;
        ui->grid_layout->addWidget(album_widget, row, column, Qt::AlignCenter);
    }

    // enable GUI updates again now that done
    ui->scroll_area_widget_contents->setUpdatesEnabled(true);
}

AlbumWidget* AlbumsPage::create_album_widget(Album& album, QWidget* parent) {
    AlbumWidget* album_widget = new AlbumWidget(album, parent);

    // connect double click signal -> open AlbumPopup
    connect(album_widget, &AlbumWidget::album_double_clicked, this, [this, album]() {
        open_album_popup(album.id);
    });

    return album_widget;
}


void AlbumsPage::open_album_popup(int album_id) {
    // make popup 
    AlbumPopup* album_popup = new AlbumPopup(core, album_id, this);

    // connect album_updated signal -> update_album() // TODO: code
    // connect(album_popup, &AlbumPopup::album_updated,
    //                 this, &AlbumsPage::update_album);

    album_popup->exec();
}
