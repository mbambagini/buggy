/** \file ssWiTypes.hpp
 *  \brief type definition file
 */

#ifndef __SHARED_SLOTTED_TYPES_HPP__
#define __SHARED_SLOTTED_TYPES_HPP__

/** \brief type of the port identifier
 *
 * it must be one byte only
 */
typedef char PortID;

/** \brief type of the Value exchanged through the port
 *
 * it must be a type that can be used with std::atomic
 */
typedef int PortValue;

#endif //__SHARED_SLOTTED_TYPES_HPP__
