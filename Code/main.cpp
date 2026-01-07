// Samuel Sutton - 2025
#include "core.h"
#include "main_window.h"
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // set working directory // TODO: fix cuz hardcoded to only MY computer
        std::filesystem::current_path("/Users/Sam/Desktop/Computer_Science/Personal_Projects/1-GitHub/Music_Manager");

        QApplication application(argc, argv); // starts Qt program
        Core core;

        MainWindow main_window(core);
        main_window.show();

        return application.exec(); // keeps program running
    }
    catch (std::runtime_error& error) {
        std::cout << "\nError: " << error.what() << "\n";
    }
 }
