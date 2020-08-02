/*
 *      Copyright (C) 2020 Johnathan Law
 *
 *      This file is part of SWL.
 *
 *      SWL is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      SWL is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with SWL.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef DATAMODEL_HPP
#define DATAMODEL_HPP

#include <string>


/**
 * @brief   A base class for MVC models.
 *          Model: knows nothing about visualisation and user interaction,
 *                 only about the data / structure.
 */
template<class T>
class DataModel
{
public:
    /// accessors:
    virtual std::size_t rows() const = 0;
    virtual T const& at(std::size_t index) const = 0;
};


#endif
