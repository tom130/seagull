/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * (c)Copyright 2006 Hewlett-Packard Development Company, LP.
 *
 */

#ifndef _GENERIC_METHODS_HPP_
#define _GENERIC_METHODS_HPP_

#include "C_ProtocolFrame.hpp"


extern "C" int sys_time_secs (T_pValueData  P_msgPart,
                              T_pValueData  P_args,
                              T_pValueData  P_result);
extern "C" int session_id (T_pValueData  P_msgPart,
                              T_pValueData  P_args,
                              T_pValueData  P_result);
extern "C" int sys_time_unsig_sec (T_pValueData  P_msgPart,
                              T_pValueData  P_args,
                              T_pValueData  P_result); 


#endif // _GENERIC_METHODS_HPP_

