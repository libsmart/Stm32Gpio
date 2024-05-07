/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * Enable or disable the use of std::function.
 */
#undef LIBSMART_ENABLE_STD_FUNCTION
#define LIBSMART_ENABLE_STD_FUNCTION



/**
 * Enable or disable overwriting of the verbose_terminate_handler.
 * Overwriting reduces the binary size by several 10kB.
 * @see __gnu_cxx::__verbose_terminate_handler()
 */
#undef LIBSMART_OVERWRITE_verbose_terminate_handler
#define LIBSMART_OVERWRITE_verbose_terminate_handler


