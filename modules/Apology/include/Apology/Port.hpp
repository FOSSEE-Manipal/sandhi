// Copyright (C) by Josh Blum. See LICENSE.txt for licensing information.

#ifndef INCLUDED_APOLOGY_PORT_HPP
#define INCLUDED_APOLOGY_PORT_HPP

#include <Apology/Config.hpp>
#include <Apology/Wax.hpp>
#include <Theron/Actor.h>

namespace Apology
{

/*!
 * A port represents a the IO port of an base.
 * The base can be a block or a topology.
 * The index can be a input or output index.
 * The interpretation of port as a source of data
 * or a sink for data is up to the caller.
 */
struct APOLOGY_API Port
{
    //! Create an unitialized port
    Port(void);

    //! Create a port from an element and IO index
    Port(Base *elem, const size_t index, Wax container = Wax());

    //! The base in this port
    Base *elem;

    //! The IO index of this port
    size_t index;

    //! An optional reference to a container class
    Wax container;
};

//! a comparison operator overload for ports
APOLOGY_API bool operator==(const Port &lhs, const Port &rhs);

} //namespace Apology

#endif /*INCLUDED_APOLOGY_PORT_HPP*/
