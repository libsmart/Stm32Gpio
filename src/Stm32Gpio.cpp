/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "libsmart_config.hpp"

// https://elegantinvention.com/blog/information/smaller-binary-size-with-c-on-baremetal-g/
// extern "C" void __cxa_pure_virtual() { while (1); }

/*
extern "C" int __cxa_atexit(void (*destructor)(void *), void *objptr, void *dso) {
    (void) (destructor);
    (void) (objptr);
    (void) (dso);
    return 0;
}
*/

