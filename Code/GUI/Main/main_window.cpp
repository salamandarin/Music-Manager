// Samuel Sutton - 2025
#include "main_window.h"
#include "ui_main_window.h"
#include "core.h"
#include "tracks_page.h"
#include "albums_page.h"
#include "artists_page.h"
#include "settings_popup.h"

MainWindow::MainWindow(Core& core, QWidget* parent)
    :core{core}, QMainWindow{parent}, ui{new Ui::MainWindow} {
    
    ui->setupUi(this);

    // add pages to stacked widget
    tracks_page = new TracksPage(core, this);
    ui->stacked_widget->addWidget(tracks_page);
    albums_page = new AlbumsPage(core, this);
    ui->stacked_widget->addWidget(albums_page);
    artists_page = new ArtistsPage(core, this);
    ui->stacked_widget->addWidget(artists_page);

    // connect sidebar buttons
    connect(ui->tracks_page_button, &QPushButton::clicked, this, &MainWindow::show_tracks_page);
    connect(ui->albums_page_button, &QPushButton::clicked, this, &MainWindow::show_albums_page);
    connect(ui->artists_page_button, &QPushButton::clicked, this, &MainWindow::show_artists_page);
    connect(ui->settings_button, &QPushButton::clicked, this, &MainWindow::show_settings);

    // show default page
    show_tracks_page();
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::show_tracks_page() {
    ui->stacked_widget->setCurrentWidget(tracks_page);
}
void MainWindow::show_albums_page() {
    albums_page->build_album_grid(); // update GUI
    ui->stacked_widget->setCurrentWidget(albums_page);
}
void MainWindow::show_artists_page() {
    artists_page->update_list(); // update GUI
    ui->stacked_widget->setCurrentWidget(artists_page);
}
void MainWindow::show_settings() {
    SettingsPopup* settings_popup = new SettingsPopup(core, this);
    settings_popup->exec();
}
