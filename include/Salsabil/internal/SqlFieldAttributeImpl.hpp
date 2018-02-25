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

#ifndef SALSABIL_SQLFIELDATTRIBUTEIMPL_HPP
#define SALSABIL_SQLFIELDATTRIBUTEIMPL_HPP

#include "SqlField.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"

namespace Salsabil {
    class SqlDriver;

    template<typename ClassType, typename AttributeType>
    class SqlFieldAttributeImpl : public SqlField<ClassType> {
        using FieldType = typename Utility::Traits<AttributeType>::AttributeType;

    public:

        SqlFieldAttributeImpl(std::string name, int column, AttributeType attribute, bool isPrimary) :
        SqlField<ClassType>(name, column, isPrimary),
        mAttribute(attribute) {
        }

        virtual void readFromDriver(ClassType* instance, int column) {
            FieldType t;
            Utility::driverToVariable(SqlEntityConfigurer<ClassType>::driver(), column, Utility::initializeInstance(&t));
            instance->*getAttribute() = t;
        }

        virtual void writeToDriver(const ClassType* instance, int column) {
            FieldType t = instance->*getAttribute();
            Utility::variableToDriver(SqlEntityConfigurer<ClassType>::driver(), column, Utility::pointerizeInstance(&t));
        }

        void setAttribute(AttributeType attribute) {
            mAttribute = attribute;
        }

        AttributeType getAttribute() const {
            return mAttribute;
        }

    private:
        AttributeType mAttribute;
    };
}
#endif // SALSABIL_SQLFIELDATTRIBUTEIMPL_HPP

