// Samuel Sutton - 2025
#include "main_window.h"
#include <QHeaderView>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPushButton>
// TODO: DELETE EXTRA
#include <QInputDialog>
#include <QLineEdit>

MainWindow::MainWindow(Core& core, QWidget* parent)
    :QMainWindow{parent}, core(core) {
    setup_gui();
    load_tracks();
}

void MainWindow::setup_gui() {
    // create the main widget and layout
    QWidget* central_widget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(central_widget);
    // add layout to central_widget
    central_widget->setLayout(layout);
    setCentralWidget(central_widget);

    // -------------------- TABLE --------------------
    // create tracks table
    tracks_table = new QTableWidget(this);
    tracks_table->setColumnCount(8); // 8 columns
    tracks_table->setHorizontalHeaderLabels({"", "ID", "Title", "Artist", "Album", "Duration", "Date", "Tracklist #"});
    layout->addWidget(tracks_table); // add table to layout
    
    // make table fill all available space
    tracks_table->horizontalHeader()->setStretchLastSection(true);
    tracks_table->setSelectionBehavior(QAbstractItemView::SelectRows); // select only rows
    tracks_table->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi-selection with shift/command
    tracks_table->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // -------------------- BUTTONS --------------------
    // button layout
    QVBoxLayout* button_layout = new QVBoxLayout();
    layout->addLayout(button_layout); // add to main layout


    // make toggle_nested button
    QPushButton* toggle_nested_button = new QPushButton("Toggle Nested File Structure", this);
    QObject::connect(toggle_nested_button, &QPushButton::clicked, this, [this](){ // connect button to toggle_nested
        this->core.toggle_nested();
    });
    button_layout->addWidget(toggle_nested_button); // add to layout


    // make add_folder button (HARDCODED FOR TESTING ONLY)
    QPushButton* add_from_folder_button = new QPushButton("DEMO: Add 'Test_Music' Folder", this);
    QObject::connect(add_from_folder_button, &QPushButton::clicked, this, [this](){ // connect button to delete_entire_library
        this->core.add_tracks_from_folder("../../../TEST_MUSIC copy"); // TODO: MAKE IT TAKE INPUT - NOT HARDCODED PATH
        load_tracks(); // refresh GUI
    });
    button_layout->addWidget(add_from_folder_button); // add to layout

    // edit track title (demo - only works for track 1)
    QPushButton* edit_title_demo = new QPushButton("DEMO: Edit Track #1 Title", this);
    QObject::connect(edit_title_demo, &QPushButton::clicked, this, [this](){
        bool is_value_entered; // whether they clicked OK or not (or hit enter)
        QString new_title = QInputDialog::getText(this,
                                             tr("Edit Track Title"), // window label
                                             tr("Enter new title for track 1:"), // text box label
                                             QLineEdit::Normal,
                                             "", // default text (empty)
                                             &is_value_entered);

        if (is_value_entered && !new_title.isEmpty()) {
            this->core.set_track_title(1, new_title.toStdString()); // TODO: make track # not hardcoded
            load_tracks(); // refresh GUI
        }
    });
    button_layout->addWidget(edit_title_demo); // add to layout


    // // make DELETE_LIBRARY button (FOR TESTING ONLY)
    // QPushButton* delete_library_button = new QPushButton("DELETE ENTIRE LIBRARY", this);
    // QObject::connect(delete_library_button, &QPushButton::clicked, this, [this](){ // connect button to delete_entire_library
    //     this->core.delete_entire_library("../../../TEST_MUSIC copy"); // TODO: MAKE IT TAKE INPUT - NOT HARDCODED PATH
    //     load_tracks(); // refresh GUI
    // });
    // button_layout->addWidget(delete_library_button); // add to layout

    // set window properties
    setWindowTitle("Music Manager"); // window title
    resize(1280, 720); // window size
}

void MainWindow::load_tracks() {
    // reset table first
    tracks_table->clearContents();
    tracks_table->setRowCount(0);  // reset row count (before setting row count later)

    // get all tracks
    std::vector<Track> tracks = core.get_all_tracks();
    
    // set row count
    tracks_table->setRowCount(tracks.size()); // set row count
    
    // fill in table
    for (int i = 0; i < tracks.size(); ++i) {   

        // add image
        QTableWidgetItem* image_item = new QTableWidgetItem();
        // set either default image, or real image
        std::string image_path = "../../Default_Images/default_track.jpg"; // default image
        if (!tracks[i].image_path.empty()) { // if has image
            image_path = tracks[i].image_path; // replace default image with real one
        }
        // set image
        QPixmap pixmap(QString::fromStdString(image_path));
        if (!pixmap.isNull()) {
            QIcon icon(pixmap); // make icon
            tracks_table->setIconSize(QSize(50, 50)); // icon / image size
            image_item->setIcon(icon);
        }
        tracks_table->setItem(i, 0, image_item); // set image

                              // row, column, item (column 0 is image)
        tracks_table->setItem(i, 1, new QTableWidgetItem(QString::number(tracks[i].id)));
        tracks_table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tracks[i].title)));
        tracks_table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(tracks[i].artist)));
        tracks_table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(tracks[i].album)));
        tracks_table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(tracks[i].duration.to_string())));
        tracks_table->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(tracks[i].date.to_string())));
        tracks_table->setItem(i, 7, new QTableWidgetItem(QString::number(tracks[i].tracklist_num)));
        // skip file path & image path
    }
    
    // resize rows & columns to fit content
    tracks_table->resizeColumnsToContents();
    tracks_table->resizeRowsToContents();
}
