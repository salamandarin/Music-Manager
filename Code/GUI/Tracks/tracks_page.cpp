// Samuel Sutton - 2025
#include "tracks_page.h"
#include "ui_tracks_page.h"
#include "core.h"
#include "track_popup.h"
#include "add_track_popup.h"
#include "gui_utils.h"
#include "main_window.h"
#include "settings_popup.h"
#include <QLabel>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QScrollBar>

using namespace gui_utils;

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

// sizes
const int ROW_HEIGHT = 60;
const int IMAGE_MARGIN = 5;

TracksPage::TracksPage(Core& core, MainWindow* parent)
    :core{core}, QWidget{parent}, ui{new Ui::TracksPage} {
    
    ui->setupUi(this);
    // set up table
    ui->tracks_table->setColumnCount(NUM_COLUMNS);
    ui->tracks_table->setHorizontalHeaderLabels({"", "Title", "Artist", "Album", "Duration", "Date", "Tracklist #"});
    ui->tracks_table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tracks_table->verticalHeader()->setDefaultSectionSize(ROW_HEIGHT); // row height
    ui->tracks_table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed); // row height resize off
    ui->tracks_table->verticalScrollBar()->setSingleStep(8); // adjust scroll speed

    // fill in table
    build_table(); 

    // connect double click signal -> open TrackPopup
    connect(ui->tracks_table, &QTableWidget::cellDoubleClicked,
                        this, &TracksPage::open_track_popup);

    // connect library_deleted signal -> rebuild table
    connect(parent, &MainWindow::library_deleted,
            this, &TracksPage::build_table);

    // connect buttons to slots
    connect(ui->add_files_button, &QPushButton::clicked,
            this, &TracksPage::add_track_files);
    connect(ui->add_from_folder_button, &QPushButton::clicked,
            this, &TracksPage::add_tracks_from_folder);
    connect(ui->manual_add_button, &QPushButton::clicked,
            this, &TracksPage::manually_add_track);
}

TracksPage::~TracksPage() {
    delete ui;
}

//--------------------------------------------------------------------------------
//                                BUTTON SLOTS
//--------------------------------------------------------------------------------

void TracksPage::add_track_files() {
    QStringList file_paths = QFileDialog::getOpenFileNames(this, "Select Music File(s)",
                    "", "Audio Files (*.mp3 *.wav *.aiff *.aif *.flac *.m4a *.aac)",
                    nullptr, QFileDialog::ReadOnly);
    
    if (!file_paths.isEmpty()) {
        for (const QString &file_path : file_paths) {
            core.add_track(file_path.toStdString()); // add track
        }
        build_table(); // update table GUI
    }  
}

void TracksPage::add_tracks_from_folder() {
    std::string folder_path = QFileDialog::getExistingDirectory(this, "Select Folder",
                            "", QFileDialog::ShowDirsOnly | QFileDialog::ReadOnly).toStdString();
    
    if (!folder_path.empty()) {
        core.add_tracks_from_folder(folder_path); // add all files in folder

        build_table(); // update table GUI
    }  
}
void TracksPage::manually_add_track() {
    AddTrackPopup* add_track_popup = new AddTrackPopup(core, this);

    // update table GUI if tracks were added
    connect(add_track_popup, &AddTrackPopup::tracks_added,  
                        this, &TracksPage::build_table);
    
    add_track_popup->exec();
}

//--------------------------------------------------------------------------------
//                                   TABLE STUFF
//--------------------------------------------------------------------------------
void TracksPage::build_table() {
    // disable GUI updates until done (so not slowing down)
    ui->tracks_table->setUpdatesEnabled(false);

    // reset table
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
        image_label->setFixedSize(ROW_HEIGHT, ROW_HEIGHT);
        set_image(tracks[i].image_path, DEFAULT_TRACK_IMAGE, image_label); // put image in label
        // put image (inside QLabel) into row
        ui->tracks_table->setCellWidget(i, IMAGE_COLUMN, image_label);
        ui->tracks_table->cellWidget(i, IMAGE_COLUMN)->setContentsMargins(IMAGE_MARGIN, IMAGE_MARGIN, IMAGE_MARGIN, IMAGE_MARGIN); // set margins


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
    
    // resize column widths
    ui->tracks_table->resizeColumnsToContents(); // resize to fit
    ui->tracks_table->setColumnWidth(IMAGE_COLUMN, ROW_HEIGHT); // width = height so square
    ui->tracks_table->setColumnWidth(TITLE_COLUMN, 350);
    ui->tracks_table->setColumnWidth(ALBUM_COLUMN, 200);
    ui->tracks_table->setColumnWidth(TRACKLIST_NUM_COLUMN, 50);

    // enable GUI updates again now that done
    ui->tracks_table->setUpdatesEnabled(true);
}

// delete track with backspace key press
void TracksPage::keyPressEvent(QKeyEvent* key_press) {
    QModelIndexList selected_rows = ui->tracks_table->selectionModel()->selectedRows(); // grab selected rows

    // check if backspace was pressed & if any rows were selected
    if (key_press->key() == Qt::Key_Backspace && !selected_rows.isEmpty()) {

        // confirm deletion
        QMessageBox::StandardButton confirmation = QMessageBox::question(this, "Delete Tracks",
                "Are you sure you want to delete selected tracks?", QMessageBox::Yes | QMessageBox::No);
       // return if selected no
        if (confirmation == QMessageBox::No) {
            key_press->ignore(); // mark as ignored
            return;
        };

        // sort rows greatest to least (to prevent errors when removing indexes)
        std::vector<int> rows;
        for (const QModelIndex& index : selected_rows) {
            rows.push_back(index.row());
        }
        std::sort(rows.begin(), rows.end(), std::greater<int>());

        // delete all selected tracks
        for (int row : rows) {
            int track_id = get_track_id(row);
            core.remove_track(track_id); // delete track
            ui->tracks_table->removeRow(row);
        }

        key_press->accept(); // mark event as handled
    }
    else {
        // pass key press event to parent class if wasn't backspace
        QWidget::keyPressEvent(key_press);
    }
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
