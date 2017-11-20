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

#ifndef SALSABIL_SQLFIELDMETHODPTRIMPL_HPP
#define SALSABIL_SQLFIELDMETHODPTRIMPL_HPP

#include "SqlField.hpp"
#include "PointerTypeHelper.hpp"
#include "FieldTypeResolver.hpp"

namespace Salsabil {
    class SqlDriver;

    template<typename ClassType, typename GetMethodType, typename SetMethodType>
    class SqlFieldMethodPtrImpl : public SqlField<ClassType> {
        using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;

    public:

        SqlFieldMethodPtrImpl(std::string name, int column, GetMethodType getter, SetMethodType setter, bool isPrimary) :
        SqlField<ClassType>(name, column, isPrimary),
        mGetter(getter),
        mSetter(setter) {
        }

        virtual void readFromDriver(const SqlDriver* driver, ClassType* instance) {
            FieldType t;
            Utility::FieldTypeResolver::driverToVariable(driver, SqlField<ClassType>::column(), &t);
            (instance->*setter())(t);
        }

        virtual void writeToDriver(const ClassType* instance, SqlDriver* driver) {
            Utility::FieldTypeResolver::variableToDriver(driver, SqlField<ClassType>::column(), (instance->*getter())());
        }

        void getter(GetMethodType getter) {
            mGetter = getter;
        }

        GetMethodType getter() {
            return mGetter;
        }

        void setter(SetMethodType setter) {
            mSetter = setter;
        }

        SetMethodType setter() {
            return mSetter;
        }

    private:
        GetMethodType mGetter;
        SetMethodType mSetter;
    };
}
#endif // SALSABIL_SQLFIELDMETHODPTRIMPL_HPP

