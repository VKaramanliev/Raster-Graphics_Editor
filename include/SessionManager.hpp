#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include "Session.hpp"

class SessionManager {
    private:
        vector<Session*> sessions;
        int current;
        int next;

    public:
        SessionManager();

        SessionManager(const SessionManager&) = delete;
        SessionManager& operator=(const SessionManager&) = delete;

        void collage(const string& direction, const string& image1, const string& image2, const string& outimage);
        void createSession(const vector<Image*>& images);
        void switchSession(int id);
        void closeSession();

        Session* getCurrentSession() const;

        bool hasCurrentSession() const;

        ~SessionManager();
};

#endif