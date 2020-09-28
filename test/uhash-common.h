// SPDX-License-Identifier: BSD-2-Clause
/*
  Copyright (c) 2012-2020, Matthias Schiffer <mschiffer@universe-factory.net>
  All rights reserved.
*/


#pragma once


#include "crypto.h"
#include "util.h"


extern const fastd_mac_t fastd_mac_uhash_builtin __attribute__((weak));

/* clang-format off */

/* K = "abcdefghijklmnop" */
static const uint8_t key[1024 + 3 * 16 + 4 * 24 + 4 * 64 + 4 * 4] = {
	/* KDF(K, 1, 1024 + 3 * 16) */
	0xac, 0xd7, 0x9b, 0x4f, 0x6e, 0xda, 0x0d, 0x0e, 0x16, 0x25, 0xb6, 0x03, 0x84, 0xf9, 0xfc, 0x93,
	0xc6, 0xdf, 0xec, 0xa2, 0x96, 0x4a, 0x71, 0x0d, 0xad, 0x7e, 0xde, 0x4d, 0xa1, 0xd3, 0x93, 0x5e,
	0x62, 0xec, 0x86, 0x72, 0x84, 0x0a, 0x91, 0x08, 0x9a, 0x66, 0x18, 0x48, 0x4f, 0x41, 0xf7, 0xfe,
	0xb9, 0x98, 0xa7, 0xcd, 0x05, 0x7a, 0xec, 0x76, 0xca, 0x9c, 0xb7, 0xa4, 0xeb, 0x87, 0xff, 0x1e,
	0x19, 0x27, 0x82, 0xbf, 0x21, 0xa1, 0xf6, 0x10, 0x0e, 0x57, 0xb1, 0x09, 0x3a, 0x3a, 0xf3, 0xed,
	0x8e, 0x06, 0xf2, 0x1f, 0x63, 0x80, 0x3f, 0xda, 0x7a, 0x86, 0x55, 0x01, 0xb9, 0x2b, 0xa5, 0xff,
	0xb3, 0x73, 0x7d, 0x7d, 0xe6, 0x49, 0x03, 0x13, 0x39, 0x55, 0xd0, 0x2f, 0x0a, 0x32, 0xd9, 0xa6,
	0xef, 0xf2, 0x68, 0xf1, 0xd4, 0xb9, 0x87, 0x88, 0xf7, 0x02, 0x2e, 0x46, 0x27, 0x0d, 0xf0, 0x5e,
	0x41, 0xed, 0xdd, 0xb9, 0x2f, 0x62, 0x96, 0x75, 0xa3, 0x99, 0x33, 0xf5, 0x37, 0xbb, 0xda, 0xc5,
	0x51, 0x3f, 0x9b, 0xd7, 0xb7, 0x3e, 0x02, 0x8a, 0xa7, 0x31, 0x40, 0x54, 0xd0, 0xac, 0x09, 0x8b,
	0xe2, 0x81, 0xc7, 0x1d, 0xa2, 0xc8, 0x4d, 0xac, 0xf8, 0x8d, 0xfc, 0x6e, 0xbd, 0x9b, 0x70, 0xc8,
	0x6a, 0x00, 0xda, 0x34, 0xdb, 0x31, 0x3f, 0xa5, 0x76, 0x7a, 0xff, 0x00, 0x5a, 0x73, 0xa5, 0xa3,
	0x78, 0x9e, 0xcc, 0x7e, 0xd8, 0x4b, 0xda, 0x6b, 0xad, 0xd4, 0x6b, 0x91, 0x14, 0x6a, 0x02, 0xcf,
	0xe5, 0x18, 0xaf, 0x68, 0x4c, 0x1b, 0xac, 0x5c, 0x04, 0x8d, 0xfb, 0x8e, 0xaa, 0xac, 0xb5, 0x5f,
	0x26, 0x1c, 0x7c, 0xde, 0xc5, 0x1f, 0xbb, 0x02, 0xab, 0xb9, 0xca, 0xef, 0x7a, 0x9d, 0x69, 0x7c,
	0xaa, 0x7c, 0x28, 0xbe, 0x56, 0xaf, 0x26, 0xc8, 0x7f, 0xab, 0x0a, 0x29, 0x91, 0x64, 0x6e, 0xf7,
	0x9a, 0x25, 0x2c, 0x63, 0x3c, 0x1e, 0xb8, 0x89, 0xd1, 0x66, 0x6f, 0x44, 0xf8, 0xc2, 0x8d, 0x8a,
	0x53, 0x31, 0x77, 0xd5, 0xec, 0xab, 0x30, 0x98, 0x0a, 0x2c, 0x85, 0x4f, 0xc1, 0x39, 0xf3, 0x92,
	0xad, 0x47, 0xb0, 0x86, 0x54, 0x11, 0x51, 0x84, 0xcf, 0xc0, 0xa8, 0x9e, 0xda, 0x4d, 0xad, 0xed,
	0x4d, 0xb2, 0x5d, 0x22, 0xec, 0xce, 0xcf, 0x8f, 0xa4, 0x6d, 0xa6, 0x71, 0x6c, 0x74, 0xa1, 0x39,
	0xc3, 0x63, 0xc9, 0x35, 0x6c, 0x52, 0xbf, 0x9e, 0x04, 0xa9, 0xf8, 0xe4, 0x0d, 0x8d, 0xff, 0x89,
	0xfb, 0x6f, 0xfb, 0x2d, 0x85, 0x9e, 0x49, 0xc8, 0xba, 0xde, 0xef, 0xc1, 0xca, 0x59, 0x15, 0x79,
	0x8e, 0x06, 0x57, 0x1a, 0x8a, 0xc0, 0x88, 0x80, 0x5c, 0x7f, 0x90, 0x2f, 0x4b, 0x19, 0xc0, 0x44,
	0x00, 0x4f, 0x14, 0xd6, 0x76, 0xca, 0x70, 0x38, 0xc6, 0x7b, 0x6a, 0xeb, 0xcc, 0x8b, 0xb4, 0x43,
	0xe1, 0x54, 0x40, 0x2f, 0x50, 0xda, 0xfb, 0x19, 0xef, 0x50, 0xf8, 0xfe, 0x27, 0xf1, 0x29, 0xb9,
	0xfa, 0xd2, 0xdb, 0x63, 0x5d, 0xeb, 0x7d, 0xf9, 0x7c, 0x0d, 0xd9, 0xef, 0xa6, 0xe3, 0x53, 0xba,
	0xb0, 0x90, 0xbc, 0x88, 0x8d, 0xef, 0x60, 0x89, 0x6b, 0x48, 0xb3, 0x81, 0x0b, 0xaf, 0x64, 0xf5,
	0xc2, 0x76, 0x22, 0x15, 0x58, 0x9d, 0x6e, 0xde, 0x1a, 0xe7, 0xdb, 0xd6, 0xf3, 0x1a, 0x95, 0xb7,
	0xc5, 0x55, 0xbd, 0x23, 0xf1, 0xb1, 0xcb, 0x72, 0x20, 0xe0, 0xbb, 0xdf, 0x7a, 0x10, 0x0f, 0x34,
	0x84, 0x7d, 0x6b, 0x9f, 0xa3, 0x55, 0xf2, 0x10, 0xa4, 0x04, 0xf5, 0xae, 0xed, 0x0f, 0x60, 0x6d,
	0xed, 0x0c, 0xc0, 0x91, 0x30, 0xb3, 0x7a, 0x16, 0x23, 0x3e, 0x69, 0x1a, 0x46, 0x11, 0x83, 0x97,
	0x24, 0x74, 0x60, 0xd3, 0xef, 0xfa, 0xc8, 0x28, 0x89, 0xf8, 0x84, 0x7c, 0xf1, 0x21, 0xc3, 0xd1,
	0xe2, 0x10, 0xa6, 0xe2, 0x7e, 0xa1, 0xa0, 0x6c, 0x36, 0x82, 0xf7, 0x74, 0xff, 0xd3, 0xd6, 0xb3,
	0x21, 0x2c, 0xf0, 0xb7, 0xa3, 0xa6, 0xfe, 0x69, 0x56, 0x8c, 0x4e, 0x25, 0x71, 0x71, 0x5c, 0x5b,
	0xfe, 0x98, 0xc1, 0x99, 0x90, 0xaa, 0x52, 0xe7, 0xc6, 0x52, 0x13, 0x32, 0xa8, 0x51, 0xdc, 0x94,
	0x8a, 0x7a, 0x05, 0xba, 0x60, 0xc0, 0x60, 0xfe, 0x60, 0x86, 0x60, 0xdd, 0x90, 0x7e, 0x11, 0x8d,
	0x54, 0x98, 0x5c, 0x23, 0x07, 0x3f, 0x88, 0xf8, 0x12, 0xc4, 0x5e, 0x7f, 0x22, 0x14, 0x99, 0xb4,
	0x98, 0x45, 0xd8, 0x8c, 0x66, 0x69, 0x87, 0x9a, 0xcf, 0x0f, 0x0c, 0x1e, 0x9a, 0x5f, 0x8e, 0x50,
	0x3e, 0xfd, 0x65, 0xb7, 0x91, 0x82, 0x1b, 0x71, 0x6f, 0xc5, 0x14, 0x3c, 0x7b, 0x81, 0xba, 0x6c,
	0x2a, 0x9a, 0x41, 0x67, 0x7c, 0xcc, 0x34, 0xb6, 0x90, 0x5f, 0xde, 0xeb, 0x37, 0xa5, 0xb2, 0xad,
	0x0f, 0x38, 0x25, 0xb7, 0x66, 0x4b, 0x61, 0xa8, 0xf9, 0x1b, 0xc8, 0x11, 0x47, 0x49, 0x55, 0x26,
	0x26, 0xce, 0x7b, 0x87, 0x58, 0x34, 0x43, 0x47, 0xb8, 0xcd, 0xc6, 0x4b, 0x0a, 0xce, 0xc3, 0xb8,
	0x85, 0x57, 0xf9, 0x33, 0xf6, 0xda, 0xc0, 0x13, 0x45, 0x4c, 0x84, 0x38, 0xad, 0x6d, 0x33, 0x7d,
	0x8d, 0x74, 0x87, 0x96, 0x94, 0x64, 0x64, 0xd8, 0x6f, 0x7f, 0x85, 0x24, 0x97, 0xc9, 0xb2, 0xef,
	0xdb, 0x64, 0x32, 0xbe, 0x34, 0x62, 0x27, 0x89, 0xdb, 0x9d, 0xcc, 0x75, 0x06, 0x21, 0xaf, 0x9b,
	0x7c, 0x30, 0xc6, 0xd5, 0x84, 0x32, 0x34, 0x33, 0x4b, 0xf7, 0x6d, 0x71, 0x99, 0x55, 0xf9, 0x06,
	0x0a, 0x9e, 0x74, 0xce, 0x3f, 0xcb, 0xed, 0xb1, 0xa0, 0x2b, 0x23, 0xd7, 0xa6, 0xec, 0x10, 0xa7,
	0xba, 0xd9, 0x96, 0x5f, 0xff, 0x21, 0xf2, 0xfa, 0x64, 0x4a, 0x2d, 0x98, 0x02, 0x0a, 0xe3, 0xee,
	0x29, 0xcc, 0x21, 0x52, 0xf7, 0xdd, 0xaf, 0xf0, 0xf1, 0x95, 0x2e, 0xdf, 0x00, 0xc9, 0x24, 0x0c,
	0x31, 0x1e, 0x4e, 0x97, 0xdd, 0x27, 0xae, 0x5d, 0x76, 0xc4, 0x0d, 0x6a, 0xf0, 0x2f, 0x55, 0xa4,
	0xbe, 0xa9, 0x77, 0x09, 0x0a, 0x2f, 0x58, 0x79, 0x07, 0x45, 0xe4, 0xf6, 0x2f, 0x17, 0x07, 0xd9,
	0x4c, 0x6b, 0xcf, 0xca, 0xd5, 0x6d, 0x05, 0x1f, 0xb3, 0x44, 0x46, 0x5a, 0x47, 0xe3, 0xf4, 0xa8,
	0xb9, 0xc4, 0x47, 0x57, 0x5e, 0x52, 0xcb, 0x00, 0xa8, 0xd9, 0x98, 0xd3, 0xd6, 0x61, 0xfd, 0x7a,
	0xe9, 0x6e, 0xf0, 0xe8, 0xea, 0x83, 0x0b, 0x27, 0x94, 0xee, 0x4c, 0xe8, 0xed, 0x8d, 0x87, 0x2e,
	0x6e, 0x93, 0xd2, 0xef, 0x85, 0xe1, 0x6c, 0xcb, 0x0a, 0x1d, 0xff, 0x2c, 0xe7, 0xdb, 0x2f, 0x50,
	0xd1, 0x6b, 0x91, 0x7d, 0xf1, 0x56, 0x78, 0xe6, 0x54, 0xb8, 0xa0, 0x5c, 0xb6, 0xf4, 0xf8, 0xe6,
	0xc5, 0xf2, 0xf6, 0x19, 0x84, 0xb9, 0xc9, 0xc7, 0x74, 0x83, 0x87, 0x57, 0x07, 0x73, 0xae, 0x1c,
	0x6d, 0xdc, 0x56, 0x06, 0xf1, 0x3e, 0xe3, 0x2a, 0xe5, 0x17, 0x1f, 0x94, 0x4e, 0xac, 0x04, 0xe4,
	0xcd, 0x67, 0x4d, 0x1a, 0x61, 0x88, 0x04, 0x35, 0xec, 0xf4, 0x0f, 0xf7, 0x65, 0xbd, 0x1c, 0x62,
	0xf0, 0x72, 0xa6, 0x71, 0xec, 0xce, 0x26, 0x19, 0xa8, 0xc9, 0x0c, 0x9a, 0xfe, 0x3c, 0xa0, 0x73,
	0x33, 0xfc, 0xa3, 0x2a, 0x3e, 0xf0, 0xfb, 0xef, 0x04, 0x80, 0x9e, 0xec, 0x36, 0x4f, 0xe6, 0x38,
	0xa6, 0xcb, 0x6c, 0x40, 0xf5, 0x6c, 0x58, 0xec, 0x9b, 0x7a, 0x77, 0x90, 0x4e, 0x81, 0x8e, 0xc3,
	0x0b, 0x05, 0xd4, 0x41, 0x8d, 0x88, 0x0b, 0x66, 0xbe, 0x78, 0x75, 0x01, 0x06, 0x75, 0x9d, 0x33,
	0x33, 0x4d, 0x9c, 0x7d, 0x89, 0x5b, 0xb0, 0xe9, 0x1a, 0x71, 0x04, 0x25, 0x0b, 0xf0, 0xf5, 0x6c,
	0xec, 0x8c, 0xc9, 0x97, 0x70, 0xc5, 0x39, 0x3c, 0x45, 0xbe, 0x18, 0xda, 0x74, 0x4c, 0x29, 0x4f,
	0x96, 0x93, 0xfe, 0x79, 0xa0, 0xdf, 0xab, 0xf3, 0x24, 0x9b, 0xc9, 0xf0, 0x13, 0x69, 0x54, 0xf4,
	0xd9, 0xa4, 0xfa, 0xfe, 0x38, 0xb7, 0xa0, 0x1a, 0x7b, 0xf9, 0x0b, 0x23, 0x0e, 0x1f, 0x8f, 0xe1,

	/* KDF(K, 2, 4 * 24) */
	0xbe, 0x94, 0xb8, 0xdd, 0x39, 0x37, 0xbe, 0xf8, 0x7b, 0x9e, 0x34, 0xb6, 0xd0, 0x40, 0x6d, 0xb9,
	0xb2, 0xcc, 0x8a, 0xd9, 0xcc, 0xcc, 0xa5, 0xee, 0xbb, 0x03, 0x6f, 0x4d, 0xac, 0x0e, 0x7e, 0x72,
	0x2a, 0xfa, 0xb9, 0x08, 0x64, 0x0a, 0xf5, 0x7d, 0x74, 0x38, 0xf7, 0x1c, 0xf6, 0x84, 0x12, 0xfe,
	0x53, 0xa7, 0x44, 0x90, 0xc3, 0x70, 0xb1, 0xfc, 0xab, 0x77, 0x0c, 0x79, 0x13, 0x56, 0xbb, 0xbf,
	0x00, 0x03, 0xcd, 0x3a, 0xb8, 0x9f, 0xba, 0x87, 0x51, 0x69, 0xfa, 0x1f, 0x3a, 0xb0, 0xa9, 0xc0,
	0x65, 0xb7, 0x82, 0xd5, 0xe1, 0xa9, 0x25, 0x3c, 0x6f, 0xa5, 0x17, 0x4c, 0xda, 0x19, 0x02, 0xe0,

	/* KDF(K, 3, 4 * 64) */
	0xf4, 0x5a, 0x8b, 0x4d, 0xa9, 0x83, 0xec, 0x44, 0xfb, 0x3e, 0xa9, 0x14, 0xba, 0xb0, 0x60, 0x8f,
	0xd2, 0x18, 0xac, 0x6c, 0x63, 0xea, 0x42, 0x22, 0xc0, 0xc3, 0x39, 0x05, 0xec, 0xc3, 0x27, 0x57,
	0xf1, 0xef, 0xc4, 0x35, 0x19, 0x98, 0xd6, 0x59, 0xb6, 0x92, 0x68, 0x57, 0x20, 0x10, 0x45, 0x95,
	0x59, 0x66, 0x91, 0xfc, 0x11, 0x40, 0xeb, 0x02, 0x52, 0xb7, 0x83, 0x34, 0x4d, 0xac, 0xea, 0x7d,
	0x66, 0x96, 0xc2, 0x0e, 0x8a, 0x9f, 0x78, 0x6e, 0x6c, 0xb5, 0x68, 0xad, 0xbe, 0xfc, 0x83, 0x08,
	0x22, 0x85, 0xa3, 0x88, 0x92, 0x87, 0xfe, 0xeb, 0x94, 0x87, 0x37, 0x48, 0xc6, 0x9c, 0x7b, 0x93,
	0xd0, 0xfd, 0x55, 0x84, 0xc3, 0x70, 0x22, 0x96, 0x31, 0x54, 0xd1, 0x61, 0x17, 0x7e, 0xa1, 0x91,
	0x74, 0xd6, 0xeb, 0x42, 0xe7, 0x80, 0x70, 0x5e, 0xdb, 0x44, 0x73, 0x94, 0x7d, 0x46, 0x2b, 0xd0,
	0xf8, 0xf3, 0x38, 0x55, 0xfe, 0xe9, 0xaf, 0x6b, 0x51, 0xdf, 0xa4, 0x6b, 0x93, 0x5a, 0x9b, 0xd7,
	0xd7, 0x6e, 0xda, 0x64, 0x6b, 0x2d, 0x82, 0x83, 0x68, 0xf1, 0xb0, 0x31, 0xa1, 0xab, 0xd7, 0xe5,
	0xc0, 0xe5, 0x37, 0xbe, 0xff, 0x9d, 0x87, 0x17, 0x2c, 0xb7, 0x50, 0xc2, 0x29, 0x2e, 0x8b, 0x24,
	0x52, 0xef, 0x5f, 0xb7, 0x81, 0xfe, 0x58, 0xc2, 0x95, 0xc3, 0x35, 0x65, 0xf6, 0x08, 0x22, 0x85,
	0x43, 0xa3, 0x7f, 0xfa, 0x09, 0x7e, 0xc5, 0x94, 0xfc, 0x2d, 0x4c, 0xd8, 0xe5, 0x0a, 0x2d, 0x61,
	0x8f, 0x4f, 0xfd, 0x05, 0xf5, 0x04, 0x79, 0x1d, 0x83, 0x0f, 0xaa, 0x15, 0x4c, 0x89, 0x67, 0x45,
	0x13, 0x5d, 0x28, 0x40, 0x83, 0xe5, 0xb3, 0xda, 0x89, 0x51, 0x9c, 0x76, 0xea, 0x42, 0x1e, 0x19,
	0x94, 0x29, 0xde, 0x69, 0xf5, 0x6e, 0xfa, 0x2f, 0x3c, 0xf1, 0xc9, 0xa6, 0x25, 0xbe, 0xbf, 0xad,

	/* KDF(K, 4, 4 * 4) */
	0x2e, 0x79, 0xf4, 0x61, 0xa7, 0x4c, 0x03, 0xaa, 0xc9, 0x43, 0xba, 0x6d, 0x21, 0x2e, 0xb6, 0xcb,
};

/* Nonce = "bcdefghi" */
static const fastd_block128_t pad = { .b = {
	0x8d, 0xdc, 0xc1, 0x69, 0x1a, 0xa6, 0xbe, 0xfb, 0xf0, 0x1a, 0x26, 0x61, 0xb7, 0x76, 0x0a, 0xf8,
}};

/* clang-format on */
