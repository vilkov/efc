/**
 * This file is part of the EFC library.
 *
 * Copyright (C) 2011-2016 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * The EFC library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The EFC library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the EFC library. If not, see <http://www.gnu.org/licenses/>.
 */
#include <ws2tcpip.h>
#include <errno.h>


namespace EFC {

Sender::Sender(Socket &socket) :
	Transport(socket)
{}

bool Sender::send(const void *buffer, size_t size)
{
	int written = ::send(descriptor(), static_cast<const char *>(buffer), size, 0);

    if (LIKELY(written > 0))
    {
		int res;

		while (static_cast<size_t>(written) < size)
		{
			res = ::send(descriptor(), static_cast<const char *>(buffer) + written, size - written, 0);

		    if (LIKELY(res > 0))
				written += res;
		    else
		    {
		        if (LIKELY(res == 0))
		            setLastError(EINTR);
		        else
					setLastError(::WSAGetLastError());

		        return false;
		    }
		}

		return true;
	}
    else
        if (LIKELY(written == 0))
            setLastError(EINTR);
        else
			setLastError(::WSAGetLastError());

	return false;
}

}
