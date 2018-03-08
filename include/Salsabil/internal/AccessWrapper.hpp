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

#ifndef SALSABIL_ACCESSWRAPPER_HPP
#define SALSABIL_ACCESSWRAPPER_HPP

namespace Salsabil {

    template<class ClassType, typename FieldType>
    class AccessWrapper {
    public:
        virtual void get(const ClassType*, FieldType*) = 0;
        virtual void set(ClassType*, FieldType*) = 0;
    };

    template<class ClassType, typename FieldType, typename AttributeType>
    class AccessWrapperAttributeImpl : public AccessWrapper<ClassType, FieldType> {
        AttributeType mAttributePtr;
    public:

        AccessWrapperAttributeImpl(AttributeType attribute) : mAttributePtr(attribute) {
        }

        virtual void get(const ClassType* classInstance, FieldType* fieldInstance) {
            *fieldInstance = classInstance->*mAttributePtr;
        }

        virtual void set(ClassType* classInstance, FieldType* fieldInstance) {
            classInstance->*mAttributePtr = *fieldInstance;
        }
    };

    template<class ClassType, typename FieldType, typename GetMethodType, typename SetMethodType>
    class AccessWrapperMethodImpl : public AccessWrapper<ClassType, FieldType> {
        GetMethodType mGetterPtr;
        SetMethodType mSetterPtr;
    public:

        AccessWrapperMethodImpl(GetMethodType getter, SetMethodType setter) : mGetterPtr(getter), mSetterPtr(setter) {
        }

        virtual void get(const ClassType* classInstance, FieldType* fieldInstance) {
            *fieldInstance = (classInstance->*mGetterPtr)();
        }

        virtual void set(ClassType* classInstance, FieldType* fieldInstance) {
            (classInstance->*mSetterPtr)(*fieldInstance);
        }
    };
}

#endif // SALSABIL_ACCESSWRAPPER_HPP 
