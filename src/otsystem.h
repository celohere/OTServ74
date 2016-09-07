//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTSERV_OTTHREAD_H__
#define __OTSERV_OTTHREAD_H__

#include "definitions.h"

#include <algorithm>
#include <list>
#include <vector>

typedef std::vector<std::pair<uint32_t, uint32_t>> IPList;

#ifdef __WINDOWS__
#ifdef __WIN_LOW_FRAG_HEAP__
#define _WIN32_WINNT 0x0501
#endif
#include <cstddef>
#include <cstdlib>
#include <sys/timeb.h>
#include <sys/types.h>
#include <winsock2.h>

#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif

inline int64_t OTSYS_TIME()
{
	_timeb t;
	_ftime(&t);
	return ((int64_t)t.millitm) + ((int64_t)t.time) * 1000;
}

typedef int socklen_t;

#else // __WINDOWS__

#include <arpa/inet.h>
#include <ctime>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/timeb.h>
#include <sys/types.h>

inline int64_t OTSYS_TIME()
{
	timeb t;
	ftime(&t);
	return ((int64_t)t.millitm) + ((int64_t)t.time) * 1000;
}

#ifndef SOCKET
#define SOCKET int
#endif

#ifndef closesocket
#define closesocket close
#endif

#endif // #if defined __WINDOWS__

#endif // #ifndef __OTSYSTEM_H__
