
# Copyright (C) 2017-2018, Abdullatif Kalla. All rights reserved.
# E-mail: laateef@outlook.com
# Github: https://github.com/Laateef/Salsabil
#
# This file is part of the Salsabil project.
# 
# Salsabil is free software: you can redistribute it and/or modify 
# it under the terms of the GNU General Public License as published by 
# the Free Software Foundation, either version 3 of the License, or 
# (at your option) any later version.
# 
# Salsabil is distributed in the hope that it will be useful, 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Salsabil. If not, see <http://www.gnu.org/licenses/>.

project(Tests)

add_subdirectory(core)

add_subdirectory(drivers)


enable_testing()

add_executable(
main_test 
SqlRelationOneToOneAttributeTest.cpp 
SqlRelationOneToOneMethodTest.cpp 
SqlRelationOneToManyMethodTest.cpp
SqlRepositoryTest.cpp 
SqlEntityConfigurerTest.cpp
)

target_link_libraries(main_test doctest_with_main sqlite_driver_lib sqlite3_backend core_lib)

add_test(main_test main_test)





