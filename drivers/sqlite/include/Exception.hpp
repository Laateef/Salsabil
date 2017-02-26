/**
 * Copyright (c) 2-2-2017 Abdullateef Qallaa. All rights reserved.
 * E-mail: laateef@outlook.com
 * Github: https://github.com/Laateef/Salsabil
 *
 * This file is part of the Salsabil Project.
 * See the LICENSE file for terms of use.
 *
 * @file
*/

#include <string>

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

class Exception : public std::exception
{
public:
    Exception(const std::string& message);

    virtual const char* what() const noexcept override;

private:
    std::string mMessage;
};

#endif // EXCEPTION_HPP
