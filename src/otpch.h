#ifdef __OTSERV_OTCP_H__
#error "Precompiled header should only be included once"
#endif
#define __OTSERV_OTCP_H__

//#undef __USE_OTPCH__

// Definitions should be global.
#include "definitions.h"

#ifdef __USE_OTPCH__

#ifdef __WINDOWS__
#include <winerror.h>
#endif

// libxml
#include <libxml/parser.h>
#include <libxml/threads.h>
#include <libxml/xmlmemory.h>
// boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/tokenizer.hpp>
// std
#include <list>
#include <map>
#include <string>
#include <vector>
// lua
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
// otserv
#include "thing.h"

#endif
