// Samuel Sutton - 2025
#include "tracks_page.h"
#include "ui_tracks_page.h"
#include "core.h"
#include "track_popup.h"
#include "add_track_popup.h"
#include "gui_utils.h"
#include <QLabel>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

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

TracksPage::TracksPage(Core& core, QWidget* parent)
    :core{core}, QWidget{parent}, ui{new Ui::TracksPage} {
    
    ui->setupUi(this);
    setup_table(); // set table headers

    // connect double click signal -> open TrackPopup
    connect(ui->tracks_table, &QTableWidget::cellDoubleClicked,
                        this, &TracksPage::open_track_popup);

    // connect buttons to slots
    connect(ui->add_files_button, &QPushButton::clicked,
            this, &TracksPage::add_track_files);
    connect(ui->add_from_folder_button, &QPushButton::clicked,
            this, &TracksPage::add_tracks_from_folder);
    connect(ui->manual_add_button, &QPushButton::clicked,
            this, &TracksPage::manually_add_track);

    connect(ui->delete_library_button, &QPushButton::clicked, // TODO: DELETE THIS ENTIRE BUTTON
                            this, &TracksPage::delete_library);
}

TracksPage::~TracksPage() {
    delete ui;
}

//--------------------------------------------------------------------------------
//                                    BUTTONS SLOTS
//--------------------------------------------------------------------------------

void TracksPage::add_track_files() {
    QStringList file_paths = QFileDialog::getOpenFileNames(this, "Select Music File(s)",
                    "", "Audio Files (*.mp3 *.wav *.aiff *.aif *.flac *.m4a *.aac)",
                    nullptr, QFileDialog::ReadOnly);
    
    if (!file_paths.isEmpty()) {
        for (const QString &file_path : file_paths) {
            core.add_track(file_path.toStdString()); // add track
        }
        update_table(); // update table GUI
    }  
}

void TracksPage::add_tracks_from_folder() {
    std::string folder_path = QFileDialog::getExistingDirectory(this, "Select Folder",
                            "", QFileDialog::ShowDirsOnly | QFileDialog::ReadOnly).toStdString();
    
    if (!folder_path.empty()) {
        core.add_tracks_from_folder(folder_path); // add all files in folder

        update_table(); // update table GUI
    }  
}
void TracksPage::manually_add_track() {
    AddTrackPopup* add_track_popup = new AddTrackPopup(core, this);

    // update table GUI if tracks were added
    connect(add_track_popup, &AddTrackPopup::tracks_added,  
                        this, &TracksPage::update_table);
    
    add_track_popup->exec();
}

void TracksPage::delete_library() { // TODO: DELETE THIS ENTIRE BUTTON
    // get confirmation before deleting
    QMessageBox::StandardButton confirmation = QMessageBox::question(this, "Delete Library ", "Are you sure?",
                                                QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::Yes) {
        // delete libary + possible additional path
        core.delete_entire_library();
        
        update_table(); // refresh table GUI
    }
}

//--------------------------------------------------------------------------------
//                                   TABLE STUFF
//--------------------------------------------------------------------------------
void TracksPage::setup_table() { // TODO: CHANGE NAME???
    ui->tracks_table->setColumnCount(NUM_COLUMNS);
    ui->tracks_table->setHorizontalHeaderLabels({"", "Title", "Artist", "Album", "Duration", "Date", "Tracklist #"});
    ui->tracks_table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    update_table(); // fill in table
}

void TracksPage::update_table() {
    // reset table first
    ui->tracks_table->clearContents();
    ui->tracks_table->setRowCount(0);  // reset row count (before setting row count later)

    // get all tracks
    std::vector<Track> tracks = core.get_all_tracks();
    
    // set row count
    ui->tracks_table->setRowCount(tracks.size()); // set row count
    
    // fill in table
    for (int i = 0; i < tracks.size(); ++i) {   
        // -------------------- IMAGE --------------------
        // set either actual or default image
        QLabel* image_label = new QLabel();
        image_label->setFixedSize(50, 50); // image size
        set_image(tracks[i].image_path, DEFAULT_TRACK_IMAGE, image_label); // put image in label
        // put image (inside QLabel) into row
        ui->tracks_table->setCellWidget(i, IMAGE_COLUMN, image_label);


        // -------------------- OTHER COLUMNS --------------------
        // set title + store id (in title column)
        QTableWidgetItem* title_item = new QTableWidgetItem(QString::fromStdString(tracks[i].title));
        title_item->setData(Qt::UserRole, tracks[i].id); // store id in title column
        ui->tracks_table->setItem(i, TITLE_COLUMN, title_item);

        // set other columns     // row, column, item (column 0 is image)
        ui->tracks_table->setItem(i, ARTIST_COLUMN, new QTableWidgetItem(QString::fromStdString(tracks[i].artist)));
        ui->tracks_table->setItem(i, ALBUM_COLUMN, new QTableWidgetItem(QString::fromStdString(tracks[i].album)));
        ui->tracks_table->setItem(i, DURATION_COLUMN, new QTableWidgetItem(QString::fromStdString(tracks[i].duration.to_string())));
        ui->tracks_table->setItem(i, DATE_COLUMN, new QTableWidgetItem(QString::fromStdString(tracks[i].date.to_string())));
        ui->tracks_table->setItem(i, TRACKLIST_NUM_COLUMN, new QTableWidgetItem(QString::number(tracks[i].tracklist_num)));
    }
    
    // resize rows & columns to fit content
    ui->tracks_table->resizeColumnsToContents();
    ui->tracks_table->resizeRowsToContents();
}

//--------------------------------------------------------------------------------
//                             TRACK POPUP SLOTS
//--------------------------------------------------------------------------------
void TracksPage::open_track_popup(int row, int column) {
    // make popup
    TrackPopup* track_popup = new TrackPopup(core, get_track_id(row), this);

    // connect track_updated signal -> update_track()
    connect(track_popup, &TrackPopup::track_updated,  
                    this, &TracksPage::update_track);

    track_popup->exec();
}

// update track (row)
void TracksPage::update_track(int track_id) {
    update_row(get_track_row(track_id), track_id);
}
//--------------------------------------------------------------------------------
//                             UPDATE SINGULAR ROW
//--------------------------------------------------------------------------------
void TracksPage::update_row(int row, int track_id) {
    // grab updated track data
    Track track = core.get_track(track_id);

    // update row text
    ui->tracks_table->item(row, TITLE_COLUMN)->setText(QString::fromStdString(track.title));
    ui->tracks_table->item(row, ARTIST_COLUMN)->setText(QString::fromStdString(track.artist));
    ui->tracks_table->item(row, ALBUM_COLUMN)->setText(QString::fromStdString(track.album));
    ui->tracks_table->item(row, DURATION_COLUMN)->setText(QString::fromStdString(track.duration.to_string()));
    ui->tracks_table->item(row, DATE_COLUMN)->setText(QString::fromStdString(track.date.to_string()));
    ui->tracks_table->item(row, TRACKLIST_NUM_COLUMN)->setText(QString::number(track.tracklist_num));
}
//--------------------------------------------------------------------------------
//                                 HELPER FUNCTIONS
//--------------------------------------------------------------------------------
int TracksPage::get_track_row(int track_id) {
    // find which row contains the track_id
    for (int row = 0; row < ui->tracks_table->rowCount(); ++row) {
        QTableWidgetItem* title_item = ui->tracks_table->item(row, TITLE_COLUMN);
        if (title_item && title_item->data(Qt::UserRole).toInt() == track_id) {
            return row;
        }
    }

    // throw error if no row with that id is found
    std::string error_message = "Track ID: " + std::to_string(track_id) + " NOT FOUND in tracks table!!!";
    error_message += "\nEither ID doesn't exist, or track isn't in table";
    throw std::runtime_error(error_message);
}

int TracksPage::get_track_id(int row) {
    // grab the id from the title column
    QTableWidgetItem* title_item = ui->tracks_table->item(row, TITLE_COLUMN);
    if (title_item) {
        return title_item->data(Qt::UserRole).toInt(); // return id
    }

    // throw error if title column item is somehow now found
    std::string error_message = "Could not find title column of Row #" + std::to_string(row);
    error_message += " in tracks table";
    throw std::runtime_error(error_message);
}
