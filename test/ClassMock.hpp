/*
 * Copyright (C) 2017, Abdullatif Kalla. All rights reserved.
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

#ifndef SALSABIL_CLASSMOCK_HPP
#define SALSABIL_CLASSMOCK_HPP

#include <stdint.h>

#include "gmock/gmock.h"

using namespace ::testing;

class ClassMock {
public:
    MOCK_CONST_METHOD0(getId, int(void));
    MOCK_CONST_METHOD0(getName, std::string(void));
    MOCK_METHOD1(setId, void(int));
    MOCK_METHOD1(setName, void(const std::string&));

    ClassMock() {
        ON_CALL(*this, setId(_)).WillByDefault(Invoke([&, this](int id) {
            ON_CALL(*this, getId()).WillByDefault(Return(id));
        }));

        ON_CALL(*this, setName(_)).WillByDefault(Invoke([&, this](const std::string & name) {
            ON_CALL(*this, getName()).WillByDefault(Return(name));
        }));
    }

    int id;
    std::string name;
    float weight;

};
#endif // SALSABIL_CLASSMOCK_HPP