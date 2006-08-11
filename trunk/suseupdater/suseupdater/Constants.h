/* 
   Copyright (C) 2006 Narayan Newton <narayannewton@gmail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define PROGRAM_NAME "suseupdater"
#define VERSION "0.1"

#define TRAY_ICON_GREEN "suse_green.png"
#define TRAY_ICON_RED	"suse_red.png"

//Enum to represent the various backends installed. New backends added here
enum { BACKEND_ZMD, BACKEND_ZYPP, BACKEND_SMART };

//ZYPP and SMART don't actually exist at this point by the way...

//Enum to represent the two protocols for ZMD
enum { ZMD_TCP, ZMD_UDS };

/* The TCP and UDS address of the local zmd server */
#define TCP_SERVER_ADDY "127.0.0.1"
#define TCP_SERVER_POSTFIX ":2544/zmd/RPC2"
#define UDS_SERVER_ADDY "udshttp:/var/tmp/kzmd-http"

//Program Options
#define BUGGY_ZMD //turns on bug workarounds for ZMD
#define NO_PACKAGE_LOCKS //turns off support for locking packages

#endif

