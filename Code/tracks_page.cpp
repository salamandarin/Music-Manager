// Samuel Sutton - 2025
#include "tracks_page.h"
#include "ui_tracks_page.h"
#include "core.h"
#include "track_popup.h"
#include <QLabel>
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

TracksPage::TracksPage(Core& core, QWidget* parent)
    :core{core}, QWidget{parent}, ui{new Ui::TracksPage} {
    
    ui->setupUi(this);
    setup_table(); // set table headers

    // connect double click signal -> open TrackPopup
    connect(ui->tracks_table, &QTableWidget::cellDoubleClicked,
                        this, &TracksPage::open_track_popup);

    // connect buttons to slots
    connect(ui->add_track_button, &QPushButton::clicked,
                            this, &TracksPage::add_track);
    connect(ui->add_from_folder_button, &QPushButton::clicked,
                            this, &TracksPage::add_from_folder);
    connect(ui->delete_library_button, &QPushButton::clicked, // TODO: DELETE THIS ENTIRE BUTTON
                            this, &TracksPage::delete_library);
}

TracksPage::~TracksPage() {
    delete ui;
}

//--------------------------------------------------------------------------------
//                                    BUTTONS SLOTS
//--------------------------------------------------------------------------------
void TracksPage::add_track() {
    Track track;

    // gather input
    bool is_value_entered; // whether they clicked OK or not (or hit enter)
    track.title = QInputDialog::getText(this,
                                         tr("Add Track"), // window label
                                         tr("Title:"), // text box label
                                         QLineEdit::Normal,
                                         "", // default text (empty)
                                         &is_value_entered).toStdString();

    // TODO: GET OTHER FIELDS!!!

    // add track
    if (is_value_entered && !track.title.empty()) {
        core.add_track(track); // add tracks from folder
        update_table(); // refresh table GUI
    }
}
void TracksPage::add_from_folder() {
    // gather input for folder path
    bool is_value_entered; // whether they clicked OK or not (or hit enter)
    QString folder_path = QInputDialog::getText(this,
                                         tr("Add Tracks from Folder"), // window label
                                         tr("Enter folder path (relative to 'Music_Manager'):"), // text box label
                                         QLineEdit::Normal,
                                         "", // default text (empty)
                                         &is_value_entered);

    // add tracks from folder                                    
    if (is_value_entered && !folder_path.isEmpty()) {
        core.add_tracks_from_folder(folder_path.toStdString()); // add tracks from folder
        update_table(); // refresh table GUI
    }

    // TODO: HANDLE INCORRECT PATHS WITHOUT CRASHING!!! LET THEM TRY AGAIN!!
}
void TracksPage::delete_library() { // TODO: DELETE THIS ENTIRE BUTTON
    core.delete_entire_library("TEST_MUSIC copy"); // TODO: MAKE IT TAKE INPUT - NOT HARDCODED PATH
    update_table(); // refresh table GUI
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
        // get either default or actual image path
        std::string image_path = "Code/Default_Images/default_track.jpg"; // default image
        if (!tracks[i].image_path.empty()) { // if has image
            image_path = tracks[i].image_path; // replace default image with real one
        }
        // set image
        QPixmap image_pixmap(QString::fromStdString(image_path));
        if (image_pixmap.isNull()) { // throw error if image failed to load
            throw std::runtime_error("Failed to load image from " + image_path);
        }
        // put image in a QLabel for scaling
        QLabel* image_label = new QLabel();
        image_label->setPixmap(image_pixmap);
        image_label->setFixedSize(50, 50); // set size
        image_label->setScaledContents(true); // let QLabel scale it
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
