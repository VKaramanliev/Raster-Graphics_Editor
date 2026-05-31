#include "../include/SessionManager.hpp"

#include <iostream>

SessionManager::SessionManager() {
    current = -1;
    next = 1;
}

void SessionManager::collage(const string& direction,
                             const string& image1,
                             const string& image2,
                             const string& outimage) {
    if (!hasCurrentSession()) {
        cout << "No active session." << endl;
        return;
    }

    getCurrentSession()->collage(direction, image1, image2, outimage);
}

void SessionManager::createSession(const vector<Image*>& images) {
    if (images.empty()) {
        cout << "Cannot start session without images." << endl;
        return;
    }

    Session* newSession = new Session(next);

    for (Image* image : images) {
        newSession->addImage(image);
    }

    sessions.push_back(newSession);
    current = newSession->getId();

    cout << "Session with ID: " << newSession->getId() << " started" << endl;
    next++;
}

void SessionManager::switchSession(int id) {
    for (Session* session : sessions) {
        if (session->getId() == id) {
            current = id;
            cout << "You switched to session with ID: " << id << "!" << endl;

            session->info();
            return;
        }
    }

    cout << "Session with ID " << id << " does not exist!" << endl;
}

void SessionManager::closeSession() {
    if (!hasCurrentSession()) {
        cout << "No session to be closed!" << endl;
        return;
    }

    for (int i = 0; i < sessions.size(); i++) {
        if (sessions[i]->getId() == current) {
            delete sessions[i];
            sessions.erase(sessions.begin() + i);

            cout << "Successfully closed session with ID: " << current << endl;

            if (sessions.empty()) {
                current = -1;
            } else {
                current = sessions.back()->getId();
            }

            return;
        }
    }

    current = -1;
}

Session* SessionManager::getCurrentSession() const {
    for (Session* session : sessions) {
        if (session->getId() == current) {
            return session;
        }
    }

    return nullptr;
}

bool SessionManager::hasCurrentSession() const {
    return getCurrentSession() != nullptr;
}

SessionManager::~SessionManager() {
    for (Session* session : sessions) {
        delete session;
    }

    sessions.clear();
}