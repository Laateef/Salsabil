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

#ifndef SALSABIL_SQLFIELDIMPL_HPP
#define SALSABIL_SQLFIELDIMPL_HPP

#include "SqlField.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "AccessWrapper.hpp"
#include <memory>

namespace Salsabil {
    class SqlDriver;

    template<typename ClassType, typename FieldType>
    class SqlFieldImpl : public SqlField<ClassType> {
    public:

        SqlFieldImpl(std::string name, int column, AccessWrapper<ClassType, FieldType>* accessWrapper) :
        SqlField<ClassType>(name, column, false),
        mAccessWrapper(accessWrapper) {
        }

        virtual void readFromDriver(ClassType* instance, int column) {
            FieldType t;
            Utility::driverToVariable(SqlEntityConfigurer<ClassType>::driver(), column, Utility::initializeInstance(&t));
            mAccessWrapper->set(instance, &t);
        }

        virtual void writeToDriver(const ClassType* instance, int column) {
            FieldType t;
            mAccessWrapper->get(instance, &t);
            Utility::variableToDriver(SqlEntityConfigurer<ClassType>::driver(), column, Utility::pointerizeInstance(&t));
        }

    private:
        std::unique_ptr<AccessWrapper<ClassType, FieldType>> mAccessWrapper;
    };
}
#endif // SALSABIL_SQLFIELDIMPL_HPP

