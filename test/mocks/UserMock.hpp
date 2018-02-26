/*
 * Copyright (C) 2017-2018, Abdullatif Kalla. All rights reserved.
 * E-mail: laateef@outlook.com
 * Github: https://github.com/Laateef/Salsabil
 *
 * This file is part of the Salsabil project.
 * 
 * Salsabil is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * Salsabil is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Salsabil. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SALSABIL_USERMOCK_HPP
#define SALSABIL_USERMOCK_HPP

#include <stdint.h>

//#include "SessionMock.hpp"
#include <vector>
#include <iostream>

class SessionMock;

class UserMock {
public:

    UserMock() : id(0), name(""), session(nullptr) {
    }

    UserMock(const UserMock& u) : id(u.id), name(u.name) {
    }

    UserMock(UserMock&& u) : id(std::move(u.id)), name(std::move(u.name)) {
    }

    UserMock& operator=(const UserMock& u) {
        this->id = u.id;
        this->name = u.name;
        return *this;
    }

    UserMock& operator=(UserMock&& u) {
        this->id = std::move(u.id);
        this->name = std::move(u.name);
        return *this;
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }

    std::string getName() const {
        return name;
    }

    void setName(const std::string& name) {
        this->name = name;
    }

    SessionMock* getSession() const {
        return session;
    }

    void setSession(SessionMock* session) {
        this->session = session;
    }

    std::vector<SessionMock*> getSessions() const {
        return sessions;
    }

    void setSessions(const std::vector<SessionMock*>& sessions) {
        this->sessions = sessions;
    }

    int id;
    std::string name;
    SessionMock* session;
    std::vector<SessionMock*> sessions;
};

#endif // SALSABIL_USERMOCK_HPP