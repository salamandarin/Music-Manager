// Samuel Sutton - 2025
#include "settings_popup.h"
#include "ui_settings_popup.h"
#include "core.h"

SettingsPopup::SettingsPopup(Core& core, QWidget* parent)
    :core{core}, QDialog{parent}, ui{new Ui::SettingsPopup} {
    
    ui->setupUi(this);

    // connect signals to functions
    // checkboxes
    connect(ui->nested_check, &QCheckBox::toggled, this, [this, &core](bool checked) {
        core.set_is_nested(checked);
    });
    connect(ui->copy_files_check, &QCheckBox::toggled, this, [this, &core](bool checked) {
        core.set_copy_music_files(checked);
    });
    connect(ui->hide_nonfiles_check, &QCheckBox::toggled, this, [this, &core](bool checked) {
        // TODO: CODE ?
    });
    connect(ui->album_art_check, &QCheckBox::toggled, this, [this, &core](bool checked) {
        // TODO: CODE ?
    });
    // restore defaults button
    connect(ui->button_box->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked,
                                                    this, &SettingsPopup::restore_defaults);
}

SettingsPopup::~SettingsPopup() {
    delete ui;
}

void SettingsPopup::restore_defaults() {
    core.set_is_nested(true);
    ui->nested_check->setChecked(true);

    core.set_copy_music_files(true);
    ui->copy_files_check->setChecked(true);
    // TODO: add other settings if added (CAREFUL OF T/F VALUE)
}