#pragma once

// Kisak bridge: route shared includes to MP or SP layer by build target.
#ifdef KISAK_SP

#include <qcommon/msg_mp.h> // msg_sp alias until dedicated msg_sp lands

#include <client_sp/client_sp.h>
#include <server_sp/server_sp.h>

#else

#include <qcommon/msg_mp.h>

#include <client_mp/client_mp.h>
#include <server_mp/server_mp.h>

#endif
