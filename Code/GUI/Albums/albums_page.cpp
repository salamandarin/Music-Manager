// Samuel Sutton - 2025
#include "albums_page.h"
#include "ui_albums_page.h"
#include "core.h"
#include "album_widget.h"
#include "album_popup.h"

AlbumsPage::AlbumsPage(Core& core, QWidget* parent)
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

    // TODO: clear grid first

    // grab all albums, fill in grid
    std::vector<Album> albums = core.get_all_albums(); // get all albums
    for (int i = 0; i < albums.size(); ++i) {
        // make album widget
        AlbumWidget* album_widget = create_album_widget(albums[i], ui->scroll_area_widget_contents); // pass in parent

        int area_width = ui->scrollArea->maximumViewportSize().width();
        const int columns = 4; // TODO: make constants at top?
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
    connect(album_widget, &AlbumWidget::album_double_clicked,
            this, &AlbumsPage::open_album_popup); // gets album object from signal // TODO: AlbumWidget gotta be const, OR return album and call album_widget->get_album() here

    return album_widget;
}


void AlbumsPage::open_album_popup(Album& album) {
    // make popup 
    AlbumPopup* album_popup = new AlbumPopup(core, album, this);

    // connect album_updated signal -> update_album() // TODO: MAKE THIS STUFF
    // connect(album_popup, &AlbumPopup::album_updated,
    //                 this, &AlbumsPage::update_album);

    album_popup->exec();
}
