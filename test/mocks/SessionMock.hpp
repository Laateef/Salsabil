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

#ifndef SALSABIL_SESSIONMOCK_HPP
#define SALSABIL_SESSIONMOCK_HPP

#include "UserMock.hpp"

class SessionMock {
public:

    SessionMock() {
    }

    int getId() const {
        return id;
    }

    void setId(int id) {
        this->id = id;
    }

    std::string getTime() const {
        return time;
    }

    void setTime(const std::string& time) {
        this->time = time;
    }

    UserMock getStackUser() const {
        return stackUser;
    }

    void setStackUser(const UserMock& user) {
        this->stackUser = user;
    }

    UserMock* getUser() const {
        return user;
    }

    void setUser(UserMock* user) {
        this->user = user;
    }

    std::vector<UserMock*> getUsers() const {
        return users;
    }

    void setUsers(const std::vector<UserMock*>& users) {
        this->users = users;
    }

    int id;
    std::string time;
    UserMock* user;
    UserMock stackUser;
    std::vector<UserMock*> users;
};
#endif // SALSABIL_SESSIONMOCK_HPP