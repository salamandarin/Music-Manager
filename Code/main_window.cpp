// Samuel Sutton - 2025
#include "main_window.h"
#include "track_popup.h"
#include <QHeaderView>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPushButton>
// TODO: DELETE EXTRA
#include <QInputDialog>
#include <QLineEdit>

enum Columns {
    IMAGE_COLUMN = 0,
    TITLE_COLUMN,
    ARTIST_COLUMN,
    ALBUM_COLUMN,
    DURATION_COLUMN,
    DATE_COLUMN,
    TRACKLIST_NUM_COLUMN,

    NUM_COLUMNS // total # of columns (last column # + 1)
};

MainWindow::MainWindow(Core& core, QWidget* parent)
    :core{core}, QMainWindow{parent} {
    setup_gui();
    update_table(); // load table values
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
    tracks_table->setColumnCount(NUM_COLUMNS);
    tracks_table->setHorizontalHeaderLabels({"", "ID", "Title", "Artist", "Album", "Duration", "Date", "Tracklist #"});
    layout->addWidget(tracks_table); // add table to layout
    
    // make table fill all available space
    tracks_table->horizontalHeader()->setStretchLastSection(true);
    tracks_table->setSelectionBehavior(QAbstractItemView::SelectRows); // select only rows
    tracks_table->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi-selection with shift/command
    tracks_table->setEditTriggers(QAbstractItemView::NoEditTriggers);


    // -------------------- CONNECT TO TRACK POPUP --------------------
    // connect double click signal -> open TrackPopup
    connect(tracks_table, &QTableWidget::cellDoubleClicked,
                    this, &MainWindow::open_track_popup);


    // -------------------- BUTTONS --------------------
    // button layout
    QVBoxLayout* button_layout = new QVBoxLayout();
    layout->addLayout(button_layout); // add to main layout


    // make toggle_nested button
    QPushButton* toggle_nested_button = new QPushButton("Toggle Nested File Structure", this);
    connect(toggle_nested_button, &QPushButton::clicked, this, [this](){
        this->core.toggle_nested(); // connect button to toggle_nested
    });
    button_layout->addWidget(toggle_nested_button); // add to layout


    // make add_folder button (HARDCODED FOR TESTING ONLY)
    QPushButton* add_from_folder_button = new QPushButton("DEMO: Add 'Test_Music' Folder", this);
    connect(add_from_folder_button, &QPushButton::clicked, this, [this](){ // connect button to delete_entire_library
        this->core.add_tracks_from_folder("../../../TEST_MUSIC copy"); // TODO: MAKE IT TAKE INPUT - NOT HARDCODED PATH
        update_table(); // refresh table GUI
    });
    button_layout->addWidget(add_from_folder_button); // add to layout

    // edit track title (demo - only works for track 1)
    QPushButton* edit_title_demo = new QPushButton("DEMO: Edit Track #1 Title", this);
    connect(edit_title_demo, &QPushButton::clicked, this, [this](){ // connect button to function
        bool is_value_entered; // whether they clicked OK or not (or hit enter)
        QString new_title = QInputDialog::getText(this,
                                             tr("Edit Track Title"), // window label
                                             tr("Enter new title for track 1:"), // text box label
                                             QLineEdit::Normal,
                                             "", // default text (empty)
                                             &is_value_entered);

        if (is_value_entered && !new_title.isEmpty()) {
            this->core.set_track_title(1, new_title.toStdString()); // TODO: make track # not hardcoded
            update_table(); // refresh table GUI
        }
    });
    button_layout->addWidget(edit_title_demo); // add to layout

    // make DELETE_LIBRARY button (FOR TESTING ONLY)
    QPushButton* delete_library_button = new QPushButton("DELETE ENTIRE LIBRARY", this);
    connect(delete_library_button, &QPushButton::clicked, this, [this](){
        // connect button to delete_entire_library
        this->core.delete_entire_library("../../../TEST_MUSIC copy"); // TODO: MAKE IT TAKE INPUT - NOT HARDCODED PATH
        update_table(); // refresh table GUI
    });
    button_layout->addWidget(delete_library_button); // add to layout

    
    // set window properties
    setWindowTitle("Music Manager"); // window title
    resize(1280, 720); // window size
}

void MainWindow::update_table() {
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
        tracks_table->setItem(i, IMAGE_COLUMN, image_item); // set image

        // set title + store id (in title column)
        QTableWidgetItem* title_item = new QTableWidgetItem(QString::fromStdString(tracks[i].title));
        title_item->setData(Qt::UserRole, tracks[i].id); // store id in title column
        tracks_table->setItem(i, TITLE_COLUMN, title_item);

        // set other columns     // row, column, item (column 0 is image)
        tracks_table->setItem(i, ARTIST_COLUMN, new QTableWidgetItem(QString::fromStdString(tracks[i].artist)));
        tracks_table->setItem(i, ALBUM_COLUMN, new QTableWidgetItem(QString::fromStdString(tracks[i].album)));
        tracks_table->setItem(i, DURATION_COLUMN, new QTableWidgetItem(QString::fromStdString(tracks[i].duration.to_string())));
        tracks_table->setItem(i, DATE_COLUMN, new QTableWidgetItem(QString::fromStdString(tracks[i].date.to_string())));
        tracks_table->setItem(i, TRACKLIST_NUM_COLUMN, new QTableWidgetItem(QString::number(tracks[i].tracklist_num)));
    }
    
    // resize rows & columns to fit content
    tracks_table->resizeColumnsToContents();
    tracks_table->resizeRowsToContents();
}

//--------------------------------------------------------------------------------
//                             TRACK POPUP SLOTS
//--------------------------------------------------------------------------------
void MainWindow::open_track_popup(int row, int column) {
    // make popup
    TrackPopup* popup = new TrackPopup(core, get_track_id(row), this);

    // connect track_updated signal -> update_track()
    connect(popup, &TrackPopup::track_updated,  
                this, &MainWindow::update_track);

    popup->exec();
}

// update track (row)
void MainWindow::update_track(int track_id) {
    update_row(get_track_row(track_id), track_id);
}
//--------------------------------------------------------------------------------
//                             UPDATE SINGULAR ROW
//--------------------------------------------------------------------------------
void MainWindow::update_row(int row, int track_id) {
    // grab updated track data
    Track track = core.get_track(track_id);

    // update row text
    tracks_table->item(row, TITLE_COLUMN)->setText(QString::fromStdString(track.title));
    tracks_table->item(row, ARTIST_COLUMN)->setText(QString::fromStdString(track.artist));
    tracks_table->item(row, ALBUM_COLUMN)->setText(QString::fromStdString(track.album));
    tracks_table->item(row, DURATION_COLUMN)->setText(QString::fromStdString(track.duration.to_string()));
    tracks_table->item(row, DATE_COLUMN)->setText(QString::fromStdString(track.date.to_string()));
    tracks_table->item(row, TRACKLIST_NUM_COLUMN)->setText(QString::number(track.tracklist_num));
}

//--------------------------------------------------------------------------------
//                                 HELPER FUNCTIONS
//--------------------------------------------------------------------------------
int MainWindow::get_track_row(int track_id) {
    // find which row contains the track_id
    for (int row = 0; row < tracks_table->rowCount(); ++row) {
        QTableWidgetItem* title_item = tracks_table->item(row, TITLE_COLUMN);
        if (title_item && title_item->data(Qt::UserRole).toInt() == track_id) {
            return row;
        }
    }

    // throw error if no row with that id is found
    std::string error_message = "Track ID: " + std::to_string(track_id) + " NOT FOUND in tracks table!!!";
    error_message += "\nEither ID doesn't exist, or track isn't in table";
    throw std::runtime_error(error_message);
}

int MainWindow::get_track_id(int row) {
    // grab the id from the title column
    QTableWidgetItem* title_item = tracks_table->item(row, TITLE_COLUMN);
    if (title_item) {
        return title_item->data(Qt::UserRole).toInt(); // return id
    }

    // throw error if title column item is somehow now found
    std::string error_message = "Could not find title column of Row #" + std::to_string(row);
    error_message += " in tracks table";
    throw std::runtime_error(error_message);
}