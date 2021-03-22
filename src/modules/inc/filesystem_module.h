/**
 * filesystem_module.h
 *
 * purpose: Stores, retrieves and organizes files on the
 *          satellite's filesystem.
 * author:  alex amellal
 *
 */

#ifndef CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVERS_FILESYSTEM_H
#define CUBESAT_CORE_INCLUDE_SUBSYSTEM_SERVERS_FILESYSTEM_H

// Macros
#define _XOPEN_SOURCE

// Project headers
#include "ipc_std.h"
#include "subsystem_module.h"

// Standard C libraries
#include <signal.h>
#include <string.h>

// Subsystem module
extern SubsystemModule filesystem_module;

#endif