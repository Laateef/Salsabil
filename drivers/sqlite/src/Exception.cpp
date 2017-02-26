/**
 * Copyright (c) 2-2-2017 Abdullateef Qallaa. All rights reserved.
 * E-mail: laateef@outlook.com
 * Github: https://github.com/Laateef/Nar
 *
 * This file is part of the Nar Project.
 * See the LICENSE file for terms of use.
 *
 * @file
*/

#include "Exception.hpp"

Exception::Exception(const std::string& message)
    : mMessage(message)
{}

const char* Exception::what() const noexcept
{
    return mMessage.c_str();
}
