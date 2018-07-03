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

#ifndef SALSABIL_SQLRELATIONALFIELDIMPL_HPP
#define SALSABIL_SQLRELATIONALFIELDIMPL_HPP

#include "SqlRelationalField.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "AccessWrapper.hpp"
#include "Logging.hpp"

#include <memory>

namespace Salsabil {
    class SqlDriver;

    template<typename ClassType, typename FieldType>
    class SqlRelationalFieldImpl : public SqlRelationalField<ClassType> {
        using FieldPureType = typename Utility::Traits<FieldType>::UnqualifiedType;
        std::unique_ptr<AccessWrapper<ClassType, FieldType>> mAccessWrapper;

    public:

        SqlRelationalFieldImpl(std::map<std::string, std::string> columnNameMap, AccessWrapper<ClassType, FieldType>* accessWrapper) :
        SqlRelationalField<ClassType>(columnNameMap),
        mAccessWrapper(accessWrapper) {
        }

        virtual void injectInto(ClassType* instance) {
            SALSABIL_LOG_DEBUG("SqlRelationalFieldImpl, inject");
            FieldType t;
            auto pt = Utility::initializeInstance(&t);
            auto primaryFieldList = SqlEntityConfigurer<FieldPureType>::primaryFieldList();
            for (std::size_t idx = 0; idx < primaryFieldList.size(); ++idx) {
                primaryFieldList[idx]->readFromDriver(pt, SqlRelationalField<ClassType>::columnNameIndexMap().at(primaryFieldList[idx]->name()));
            }

            mAccessWrapper->set(instance, &t);
        }

        virtual std::map<std::string, std::string> parseFrom(const ClassType* instance) {
            SALSABIL_LOG_DEBUG("SqlRelationalFieldImpl, parse");
            FieldType t;
            mAccessWrapper->get(instance, &t);
            auto pt = Utility::pointerizeInstance(&t);
            std::map<std::string, std::string> columnValueMap;
            auto pfList = SqlEntityConfigurer<FieldPureType>::primaryFieldList();
            for (std::size_t idx = 0; idx < pfList.size(); ++idx)
                columnValueMap.insert({SqlRelationalField<ClassType>::columnNameMap().at(pfList[idx]->name()), pfList[idx]->fetchFromInstance(pt).toString()});
            return columnValueMap;
        }
    };
}
#endif // SALSABIL_SQLRELATIONALFIELDIMPL_HPP

