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
    //    MOCK_CONST_METHOD0(getId, int(void));
    //    MOCK_CONST_METHOD0(getName, std::string(void));
    //    MOCK_CONST_METHOD0(getSession, SessionMock*(void));
    //    MOCK_CONST_METHOD0(getSessions, std::vector<SessionMock*>(void));
    //    MOCK_METHOD1(setId, void(int));
    //    MOCK_METHOD1(setName, void(const std::string&));
    //    MOCK_METHOD1(setSessions, void(const std::vector<SessionMock*>&));
    //    MOCK_METHOD1(setSession, void(SessionMock*));
    //
    //    void registerCallbacks() {
    //        ON_CALL(*this, setId(_)).WillByDefault(WithArgs<0>(Invoke(pSetId)));
    //        ON_CALL(*this, getId()).WillByDefault(Return(id));
    //
    //        ON_CALL(*this, setName(_)).WillByDefault(WithArgs<0>(Invoke(pSetName)));
    //        ON_CALL(*this, getName()).WillByDefault(Return(name));
    //
    //        //        ON_CALL(*this, setId(_)).WillByDefault(Invoke([&, this](int id) {
    //        //            this->id = id;
    //        //            ON_CALL(*this, getId()).WillByDefault(Return(Invoke(pGetId)));
    //        //        }));
    //
    //        //        ON_CALL(*this, setName(_)).WillByDefault(Invoke([&, this](const std::string & name) {
    //        //            ON_CALL(*this, getName()).WillByDefault(Return(name));
    //        //        }));
    //
    //        ON_CALL(*this, setSession(_)).WillByDefault(Invoke([&, this](SessionMock * session) {
    //            ON_CALL(*this, getSession()).WillByDefault(Return(session));
    //        }));
    //
    //        ON_CALL(*this, setSessions(_)).WillByDefault(Invoke([&, this](const std::vector<SessionMock*>& sessions) {
    //            ON_CALL(*this, getSessions()).WillByDefault(Return(sessions));
    //        }));
    //    }

    UserMock() : id(0), name("") {
        //        registerCallbacks();
    }

    UserMock(const UserMock& u) : id(u.id), name(u.name) {
        //        registerCallbacks();

        //        this->id = u.id;
        //        this->name = u.name;
        //        setId(u.getId());
        //        setName(u.getName());
    }

    UserMock(UserMock&& u) : id(std::move(u.id)), name(std::move(u.name)) {
        //        this->id = std::move(u.id);
        //        this->name = std::move(u.name);
        //            registerCallbacks();
        //            setId(std::move(u.getId()));
        //            setName(std::move(u.getName()));
    }

    UserMock& operator=(const UserMock& u) {
        this->id = u.id;
        this->name = u.name;
        //            registerCalls();
        //            setId(u.getId());
        //            setName(u.getName());
        return *this;
    }

    UserMock& operator=(UserMock&& u) {
        this->id = std::move(u.id);
        this->name = std::move(u.name);
        //            registerCalls();
        //            setId(std::move(u.getId()));
        //            setName(std::move(u.getName()));
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

private:
    int id;
    std::string name;
    SessionMock* session;
    std::vector<SessionMock*> sessions;
};

//std::ostream& operator<<(std::ostream& s, const UserMock& u) {
//    s << "UserMock {" << u.getId() << ", " << u.getName();
//    return s;
//}
#endif // SALSABIL_USERMOCK_HPP