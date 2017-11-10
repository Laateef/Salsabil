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

#ifndef SALSABIL_EXCEPTION_HPP
#define SALSABIL_EXCEPTION_HPP

#include <string>

namespace Salsabil {

    /** 
     * @class Exception
     * @brief The Exception class represents the base class from which all Salsabil exception classes inherit. 
     */
    class Exception : public std::exception {
    public:
        /** @brief Constructs an exception object with an error message <i>message</i>.
         *  @param message C-style string error message.
         *                 The string contents are copied upon construction.
         *                 Hence, responsibility for deleting the char* lies
         *                 with the caller. 
         */
        explicit Exception(const char* message);

        /** @brief Constructs an exception object with an error message <i>message</i>.
         *  @param message STL-string error message.
         */
        explicit Exception(const std::string& message);

        virtual ~Exception() {
        }

        /** @brief Returns a C-style string error description.
         *  @return A pointer to a const char*. The underlying memory
         *          is in ownership of the Exception object. Callers must
         *          not attempt to free the memory.
         */
        virtual const char* what() const noexcept override;

    protected:
        std::string mMessage; //!< Message explains the error occurred.
    };
}
#endif // SALSABIL_EXCEPTION_HPP
