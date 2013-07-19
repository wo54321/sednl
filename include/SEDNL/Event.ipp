// SEDNL - Copyright (c) 2013 Jeremy S. Cochoy
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//     1. The origin of this software must not be misrepresented; you must not
//        claim that you wrote the original software. If you use this software
//        in a product, an acknowledgment in the product documentation would
//        be appreciated but is not required.
//
//     2. Altered source versions must be plainly marked as such, and must not
//        be misrepresented as being the original software.
//
//     3. This notice may not be removed or altered from any source
//        distribution.

#ifndef EVENT_IPP_
#define EVENT_IPP_

namespace SedNL
{

Event::Event(std::string name)
    :m_name(name)
{}

Event::Event(std::string name, const Packet& packet)
    :m_name(name), m_packet(packet)
{}

const std::string& Event::get_name() const noexcept
{
    return m_name;
}

const Packet& Event::get_packet() const noexcept
{
    return m_packet;
}

} //namespace SedNL

#endif /* !EVENT_IPP_ */
