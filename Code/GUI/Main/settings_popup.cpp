// Samuel Sutton - 2025
#include "settings_popup.h"
#include "ui_settings_popup.h"
#include "core.h"

SettingsPopup::SettingsPopup(Core& core, QWidget* parent)
    :core{core}, QDialog{parent}, ui{new Ui::SettingsPopup} {
    
    // gui setup
    ui->setupUi(this);
    update_checkboxes(); // check/uncheck boxes to reflect actual settings

    // connect signals to functions
    connect(ui->toggle_nested_button, &QPushButton::clicked, this, [this, &core](bool checked) {
        core.set_is_nested(checked);
        ui->toggle_nested_button->setText(core.get_is_nested() ? "Switch to Flat File Structure" : "Switch to Nested File Structure");
    });
    // checkboxes
    connect(ui->copy_files_check, &QCheckBox::toggled, this, [this, &core](bool checked) {
        core.set_copy_music_files(checked);
    });
    // restore defaults button
    connect(ui->button_box->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked,
                                                    this, &SettingsPopup::restore_defaults);
}

SettingsPopup::~SettingsPopup() {
    delete ui;
}

// check/uncheck boxes to reflect actual settings
void SettingsPopup::update_checkboxes() {
    ui->toggle_nested_button->setText(core.get_is_nested() ? "Switch to Flat File Structure" : "Switch to Nested File Structure");
    ui->toggle_nested_button->setChecked(core.get_is_nested());

    ui->copy_files_check->setChecked(core.get_copy_music_files());
    // TODO: add other settings if added (CAREFUL OF T/F VALUE)
}

void SettingsPopup::restore_defaults() {
    core.set_is_nested(true);
    core.set_copy_music_files(true);
    // TODO: add other settings if added (CAREFUL OF T/F VALUE)

    // properly check/uncheck boxes to match
    update_checkboxes();
}
