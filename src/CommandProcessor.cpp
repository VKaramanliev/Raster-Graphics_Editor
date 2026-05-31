#include "../include/CommandProcessor.hpp"
#include "../include/PBMImage.hpp"
#include "../include/PGMImage.hpp"
#include "../include/PPMImage.hpp"

#include <iostream>
#include <sstream>

CommandProcessor::CommandProcessor() : running(true) {}

vector<string> CommandProcessor::split(const string& line) const {
    vector<string> result;
    stringstream ss(line);
    string word;

    while (ss >> word) {
        result.push_back(word);
    }

    return result;
}

Image* CommandProcessor::createImage(const string& fileName) const {
    if (fileName.size() < 4) {
        cout << "Invalid file name: " << fileName << endl;
        return nullptr;
    }

    string extension = fileName.substr(fileName.size() - 4);

    if (extension == ".pbm") {
        return new PBMImage(fileName);
    }

    if (extension == ".pgm") {
        return new PGMImage(fileName);
    }

    if (extension == ".ppm") {
        return new PPMImage(fileName);
    }

    cout << "Invalid file format: " << fileName << endl;
    return nullptr;
}

void CommandProcessor::help() const {
    cout << "The following commands are supported:" << endl;
    cout << "load <image1> <image2> ...             starts new session" << endl;
    cout << "add <image>                            adds image to current session" << endl;
    cout << "close                                  closes current session" << endl;
    cout << "save                                   saves current session" << endl;
    cout << "saveas <file>                          saves first image as new file" << endl;
    cout << "grayscale                              adds grayscale transformation" << endl;
    cout << "monochrome                             adds monochrome transformation" << endl;
    cout << "negative                               adds negative transformation" << endl;
    cout << "rotate                                 rotates the image" << endl;
    cout << "undo                                   removes last transformation" << endl;
    cout << "help                                   prints this information" << endl;
    cout << "session info                           prints current session info" << endl;
    cout << "switch <session>                       switches to session" << endl;
    cout << "collage <horizontal/vertical> <image1> <image2> <outimage>            makes collage from two images" << endl;
    cout << "help                                   prints this information" << endl;
    cout << "exit                                   exits the program" << endl;
}

void CommandProcessor::run() {
    string line;

    while (running) {
        cout << "> ";
        getline(cin, line);

        vector<string> parts = split(line);
        
        if (parts.empty()) {
            continue;
        }

        string command = parts[0];
        if (command == "load") {
            if (parts.size() < 2) {
                cout << "Usage: load <image1> <image2> ..." << endl;
                continue;
            }

            vector<Image*> images;

            for (size_t i = 1; i < parts.size(); i++) {
                Image* image = createImage(parts[i]);

                if (image != nullptr) {
                    image->load();
                    images.push_back(image);
                }
            }

            if (!images.empty()) {
                manager.createSession(images);
            }
        } else if (command == "add") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            if (parts.size() != 2) {
                cout << "Usage: add <image>" << endl;
                continue;
            }

            Image* image = createImage(parts[1]);

            if (image != nullptr) {
                image->load();
                manager.getCurrentSession()->addImage(image);
            }
        } else if (command == "save") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            manager.getCurrentSession()->save();

        } else if (command == "saveas") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            if (parts.size() != 2) {
                cout << "Usage: saveas <file>" << endl;
                continue;
            }

            manager.getCurrentSession()->saveas(parts[1]);

        } else if (command == "close") {
            manager.closeSession();

        } else if (command == "grayscale") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            manager.getCurrentSession()->grayscale();

        } else if (command == "monochrome") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            manager.getCurrentSession()->monochrome();

        } else if (command == "negative") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            manager.getCurrentSession()->negative();

        } else if (command == "rotate") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            if (parts.size() != 2) {
                cout << "Usage: rotate <left/right>" << endl;
                continue;
            }

            manager.getCurrentSession()->rotate(parts[1]);

        } else if (command == "undo") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            manager.getCurrentSession()->undo();

        } else if (command == "session") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            if (parts.size() == 2 && parts[1] == "info") {
                manager.getCurrentSession()->info();
            } else {
                cout << "Usage: session info" << endl;
            }

        } else if (command == "switch") {
            if (parts.size() != 2) {
                cout << "Usage: switch <session id>" << endl;
                continue;
            }

            int id = std::stoi(parts[1]);
            manager.switchSession(id);

        } else if (command == "help") {
            help();

        } else if (command == "exit") {
            cout << "Exiting the program..." << endl;
            running = false;

        } else if (command == "collage") {
            if (!manager.hasCurrentSession()) {
                cout << "No active session." << endl;
                continue;
            }

            if (parts.size() != 5) {
                cout << "Usage: collage <horizontal/vertical> <image1> <image2> <outimage>" << endl;
                continue;
            }

            manager.collage(parts[1], parts[2], parts[3], parts[4]);

        } else {
            cout << "Unknown command." << endl;
        }
    }
}