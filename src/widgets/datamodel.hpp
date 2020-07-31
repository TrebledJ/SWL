/*
 *    The MultiGen Game -- an elaborate, customizable 2D strategy game.
 *    Copyright (C) 2020 and GNU GPL'd by TrebledJ
 *    Thanks to all code contributors, including but not limited to:
 *      TrebledJ (C++ design & development)
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
