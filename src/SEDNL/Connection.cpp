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

#include "SEDNL/Connection.hpp"

#ifdef SEDNL_WINDOWS
#else /* SEDNL_WINDOWS */

#include <unistd.h>

#endif /* SEDNL_WINDOWS */

namespace SedNL
{

void Connection::disconnect() noexcept
{
    //The call to close may fail, for example if the last write failed.
    //But what can we do to that? So we silently ignore the return value.
    if (m_connected)
        close(m_fd);
    m_connected = false;
}

}