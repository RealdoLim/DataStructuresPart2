#ifndef SYSTEM_CONFIG_HPP
#define SYSTEM_CONFIG_HPP

// system_config.hpp
// System-wide constants. Kept in its own file so any module can tweak its limits without touching code.

// Pending order threshold. Once pendingCount reaches this value, addOrder prints a system-overload warning.
// The system still ACCEPTS new orders past this point, the warning is an early indicator only.
// (Plan, Section 5, "Overload decision" - value = 20.)
const int MAX_PENDING_WARNING = 3;

#endif