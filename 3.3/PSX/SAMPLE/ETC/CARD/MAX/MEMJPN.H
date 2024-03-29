/*
 * $PSLibId: Runtime Library Version 3.3$
 */
#ifndef _MEMJPN_H_
#define _MEMJPN_H_

/*****************************************************************
 *
 * file: memjpn.h
 *
 * 	Copyright (C) 1995 by Sony Computer Entertainment Inc.
 *				          All Rights Reserved.
 *
 *	Sony Computer Entertainment Inc. Development Department
 *
 *****************************************************************/

unsigned long _TITLE[] = {
	0x00000010,
	0x00000008,
	0x0000002c,
	0x01e00000,
	0x00010010,
	0x14417fff,
	0x30832062,
	0x692738a4,
	0x5d073ca4,
	0x54e644c5,
	0x28634cc5,
	0x10211c42,
	0x80000800,
	0x00000914,
	0x00000280,
	0x00220022,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0x55555555,
	0xfffffff7,
	0xdeedfeff,
	0xcc11dddd,
	0xbb22cccc,
	0x3b33bbbb,
	0x44634333,
	0x88686666,
	0xa88a8888,
	0xa8a8a8a8,
	0x8a88a8a8,
	0x86868888,
	0x43444666,
	0x3b3b3333,
	0xc222bbbb,
	0xd11ccccc,
	0xfeededdd,
	0x57fffffe,
	0xfffffff5,
	0xdddedeee,
	0xccccc11d,
	0xbb22222c,
	0x3333b3bb,
	0x66664443,
	0x88888666,
	0x9a98aa88,
	0x9a9a9a9a,
	0xaa9a9a9a,
	0x8888888a,
	0x46666666,
	0x33333344,
	0x222bbbb3,
	0xcccccc22,
	0xdddddd11,
	0x55fffeee,
	0xeffffff5,
	0xddddeeef,
	0xccc11111,
	0xbb2b2222,
	0x33333b3b,
	0x66464443,
	0x88888866,
	0xaaa9a8a8,
	0xa9a9a9aa,
	0x898989a9,
	0x688888aa,
	0x46466688,
	0x33333344,
	0x2bbbbb3b,
	0x11cccc22,
	0xdeddd111,
	0x75ffefee,
	0xfefffff5,
	0xdddedeee,
	0xccc1cc11,
	0xbbb222c2,
	0x333333b3,
	0x66646346,
	0xa8886888,
	0xa9a9899a,
	0x999999a9,
	0xaa9a9aaa,
	0x88888a89,
	0x46668668,
	0x33343444,
	0x222bb3bb,
	0xccccc222,
	0xededd11d,
	0x55fefeee,
	0xeffffff5,
	0xddddedee,
	0xcccccd1d,
	0xbbbb2222,
	0x3333333b,
	0x66666444,
	0xa8888888,
	0x9aaa98a8,
	0x99999999,
	0xa9aa9999,
	0x8888aa98,
	0x66666688,
	0x33333444,
	0xb2bb3bb3,
	0xc1cc2c22,
	0xdeddd1dc,
	0x55fffeed,
	0xffeffff5,
	0x1ddedfee,
	0x2cc11cd1,
	0xbbb22b22,
	0x34333bb3,
	0x68664643,
	0x98a88866,
	0x999aa9aa,
	0x9999a999,
	0x9a999999,
	0x8889989a,
	0x64666888,
	0x33334436,
	0x2bbbbb33,
	0x1cccc222,
	0xeddd1111,
	0x55feeeed,
	0xeffffff5,
	0xddddddee,
	0xccccc111,
	0x3bbb2222,
	0x4333333b,
	0x86666444,
	0x89888888,
	0xa99a9a9a,
	0x97797999,
	0xa9a99997,
	0x88a8a89a,
	0x46688868,
	0x33334446,
	0x22bb3bb3,
	0x00ccc222,
	0xedddd110,
	0x75fffee0,
	0xfefffff5,
	0xddddeede,
	0x2ccc10c1,
	0x0000000c,
	0x00000000,
	0x86000664,
	0x000a8888,
	0x79999aa9,
	0x79979779,
	0x99999799,
	0x0009a9aa,
	0x66666888,
	0x33334466,
	0x2bbbb333,
	0x00cc222b,
	0x0e0011d0,
	0x55efee00,
	0xeefffff5,
	0x1dddedee,
	0x2cc00001,
	0x00000002,
	0x00000000,
	0x86000463,
	0x000a8888,
	0x799999aa,
	0x99797799,
	0x99997977,
	0x0008aaa9,
	0x66688888,
	0x33344364,
	0x2bbb3b33,
	0x00c22222,
	0xf00ed110,
	0x55feef00,
	0xefeffff5,
	0x1dddedee,
	0x2cc0000d,
	0x0000000c,
	0x00000000,
	0x66000464,
	0x0008a888,
	0x979999a9,
	0x77979977,
	0x99779797,
	0x000a9a99,
	0x66688888,
	0x33344466,
	0xbbbbb333,
	0x001cc222,
	0x000d1110,
	0x55ffeee0,
	0xeffffff5,
	0xddddd00e,
	0xc2110000,
	0x0002c222,
	0x333b3bbb,
	0x88000464,
	0x000a8888,
	0x97999998,
	0x77777797,
	0xa9997977,
	0x00000000,
	0x60000000,
	0x33344446,
	0xbbbb3333,
	0x00122222,
	0xe0e11110,
	0x75feeeee,
	0xefeffff5,
	0x1dd0000e,
	0x22210000,
	0x000bb222,
	0x4443333b,
	0x86000466,
	0x0008aa88,
	0x7977999a,
	0x77777777,
	0x97797797,
	0x00000000,
	0x60000000,
	0x33344466,
	0x2bb3bb33,
	0x00cc22b2,
	0xdedd1d00,
	0x55ffeeee,
	0xeefffff5,
	0x0e000000,
	0x22cc1000,
	0x000bb222,
	0x4433333b,
	0x86000664,
	0x000a9888,
	0x9799999a,
	0x77777777,
	0x79977777,
	0x00000000,
	0x60000000,
	0x33443666,
	0xbb3bb333,
	0x00c22222,
	0xdd1e0000,
	0x55feeedd,
	0xefeffff5,
	0x000000ff,
	0xc2ccc000,
	0x00000000,
	0x00000000,
	0x88000460,
	0x00088a88,
	0x77779799,
	0x77777777,
	0xa7797777,
	0x8000aaa9,
	0x60006688,
	0x33444646,
	0xbbbb3333,
	0x0012222b,
	0xdd000000,
	0x55feeeee,
	0xeefefff5,
	0x0000fede,
	0xc2cc1100,
	0x00000000,
	0x00000000,
	0x88000640,
	0x000a9888,
	0x00000a9a,
	0x00000000,
	0x79700000,
	0xa000a999,
	0x6000888a,
	0x00000066,
	0x00000000,
	0x00120000,
	0x00000000,
	0x75feeed0,
	0xeefffff5,
	0x000eddde,
	0x2ccc1000,
	0x00000000,
	0x00000000,
	0x86000640,
	0x000a89a8,
	0x0000099a,
	0x00000000,
	0x97900000,
	0x80009a99,
	0x600088a8,
	0x00000066,
	0x00000000,
	0x00cc0000,
	0x00000dd0,
	0x55eff000,
	0xeefffff5,
	0x0000ddee,
	0xc22c0000,
	0x0002222c,
	0x333b3bbb,
	0x86000664,
	0x000a9a88,
	0x0000079a,
	0x00000000,
	0x79900000,
	0x8000a999,
	0x6000888a,
	0x00000046,
	0x00000000,
	0x00cc0000,
	0x000ddd10,
	0x55feef00,
	0xefeffff5,
	0x00000eee,
	0x22c00000,
	0x0002b222,
	0x64433333,
	0x88000466,
	0x000888a8,
	0x97999999,
	0x77777777,
	0x97777977,
	0xaa000a79,
	0x6000688a,
	0x3b333466,
	0x222b2bbb,
	0x0012cc2c,
	0x0ee11110,
	0x55feeef0,
	0xeefefff5,
	0xe00000ee,
	0x2200000e,
	0x0002b2b2,
	0x4443433b,
	0x88666664,
	0xa000aa88,
	0x7779799a,
	0x75557577,
	0x97977775,
	0x9a0000a7,
	0x600088a8,
	0x34446666,
	0xbbbb3333,
	0x00122222,
	0xedd11cc0,
	0x75ffeee0,
	0xeefffff5,
	0x1e00000e,
	0x22100011,
	0x000bb222,
	0x4443333b,
	0x88866666,
	0xa0000a88,
	0x77979979,
	0x57755777,
	0x79777777,
	0xa880009a,
	0x60008888,
	0x33444466,
	0xbbb3b333,
	0x00c222b2,
	0xdddd1dd0,
	0x55eeeede,
	0x0eeffff5,
	0x11e00000,
	0x22c1011c,
	0x000bb222,
	0x00000000,
	0x06866664,
	0x9a000000,
	0x77797999,
	0x77577777,
	0x97977777,
	0x9aa00007,
	0x60000a88,
	0x33443666,
	0xbbb3b333,
	0x00c22222,
	0xddd11110,
	0x55ffeede,
	0x000ffff5,
	0x1ddee000,
	0x2ccc11cd,
	0x0022b2b2,
	0x00000000,
	0x00006666,
	0xaaa00000,
	0x77997a99,
	0x57777777,
	0x09777777,
	0x089a0000,
	0x66000000,
	0x33444666,
	0xbbbb3333,
	0x0012222b,
	0xdddd11c0,
	0x55feeede,
	0x00fffff5,
	0x11ddee00,
	0x2ccccc11,
	0x002b2222,
	0x00000000,
	0x00006664,
	0xa9a88000,
	0x77779999,
	0x77777777,
	0x09979777,
	0x0a8aa000,
	0x66000000,
	0x33344466,
	0x2b3bbb33,
	0x00122222,
	0xdddd1110,
	0x75ffeede,
	0x0ffffff5,
	0x1ddddeff,
	0x22ccc1c1,
	0xbbbbbb22,
	0x33b33bbb,
	0x00066666,
	0xaaaaa886,
	0x99999999,
	0x77777777,
	0x99777777,
	0x0a9aaa00,
	0x66600000,
	0x43443666,
	0xbbbb3333,
	0x00cc22b2,
	0xeddd11d0,
	0x55efeeed,
	0xefeffff5,
	0x11ddedde,
	0xc2ccc1cd,
	0xb3bb2222,
	0x44433333,
	0x66666644,
	0x989a8888,
	0x7979999a,
	0x77777779,
	0xa9997977,
	0x889aa9aa,
	0x66666868,
	0x33344466,
	0x2bbb3b33,
	0x11c22222,
	0xddd11dc1,
	0x55feeeee,
	0xeffffff5,
	0x1ddddeee,
	0x2ccc1cd1,
	0xbbbbb2b2,
	0x3433333b,
	0x88666664,
	0xa988a888,
	0x979a99aa,
	0x77777777,
	0x97979977,
	0xaaaa9a99,
	0x68688888,
	0x33344366,
	0xbbb3b333,
	0xccc2c22b,
	0xeddd1111,
	0x55ffeedd,
	0xefeffff5,
	0x1dddedee,
	0x22ccc1cd,
	0x3bbb2222,
	0x43333333,
	0x88666664,
	0xaa988888,
	0x7a9999a9,
	0x97979979,
	0x9a9a7979,
	0x8898a9a9,
	0x66668888,
	0x33334666,
	0x2bbbbb33,
	0x1ccc2222,
	0xdddd111c,
	0x75feeefd,
	0xeefffff5,
	0x1dddedee,
	0xcccc1c11,
	0xbbbbb222,
	0x433333bb,
	0x86666644,
	0x9a898888,
	0x99999aaa,
	0x79997999,
	0x99999999,
	0x88898aaa,
	0x66866888,
	0x34334464,
	0x2bbb3b3b,
	0xcccc2222,
	0xdedd1dc1,
	0x55fffeee,
	0xfffffff5,
	0xdddeddee,
	0x2ccc1cd1,
	0x3bb22222,
	0x43333333,
	0x86666444,
	0xa8a88886,
	0x9999aa9a,
	0x99779999,
	0xaa99a799,
	0x8a8989a9,
	0x66668888,
	0x33334436,
	0x22bbbb33,
	0xc1cc222b,
	0xedddd111,
	0x55feeeed,
	0xeefffff5,
	0x1dddefee,
	0xccccc111,
	0xbbbb2222,
	0x33433b3b,
	0x66866634,
	0x89888888,
	0x99aaa98a,
	0x7a9a9999,
	0xa9a999a9,
	0x8888989a,
	0x66666888,
	0x3b343446,
	0x2bbb33b3,
	0x1ccccc22,
	0xeddd111c,
	0x55fffeed,
	0xfffffff5,
	0xdddeddee,
	0x2cc11cd1,
	0xbbb2b222,
	0x333333b3,
	0x66664444,
	0x88888886,
	0x9a9a9a98,
	0x9999a999,
	0xa9aa9a99,
	0x88888989,
	0x46666688,
	0x33333444,
	0x22bbbb3b,
	0xc1cc2222,
	0xdeddd1d1,
	0x75fefeee,
	0x55555555,
	0x55555555,
	0x55555557,
	0x55555557,
	0x55575555,
	0x55575575,
	0x55555575,
	0x55555555,
	0x55555557,
	0x55555555,
	0x55555555,
	0x75555557,
	0x55555575,
	0x55555555,
	0x55555755,
	0x55555555,
	0x55555555,
	0x55555757,
	0x55555555,
	0x57575555,
	0x55555755,
	0x75755557,
	0x57555755,
	0x57575755,
	0x57557555,
	0x75755555,
	0x57555555,
	0x57575755,
	0x55757575,
	0x57575755,
	0x75557555,
	0x57575555,
	0x55555757,
	0x57555555,
};

unsigned long univ_mem[] = {
	0x00000010,
	0x00000008,
	0x0000002c,
	0x01e00000,
	0x00010010,
	0x08427fff,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x000032e8,
	0x00000280,
	0x0069003e,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x8a00a2aa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x20aaaaaa,
	0xaaaaaa0a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x20aaaaaa,
	0xaaaa8a00,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x8a000020,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x490000a0,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x0000aaaa,
	0xaaaa0a00,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00a0aaaa,
	0xaaaa4900,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xa2aaaaaa,
	0x09000000,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaa08a2,
	0xa0aa0a20,
	0xa08a0000,
	0xaaaa0000,
	0x490001a2,
	0x8a20aaaa,
	0xaa00a2aa,
	0x080000aa,
	0x0a0000aa,
	0x5100aaaa,
	0xaaaa0400,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x0000aaaa,
	0x0a20aa0a,
	0x0000a0aa,
	0x0000a008,
	0x51aaaa0a,
	0xa0aa4900,
	0xa2aa0000,
	0x00aaaa00,
	0x008a0000,
	0xaaaa0000,
	0x04005525,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xa2aaaaaa,
	0xaa8a0000,
	0xa0aa0420,
	0xa0000000,
	0xaa000000,
	0x490061aa,
	0x080020aa,
	0x4a00a2aa,
	0x000000aa,
	0x0000000a,
	0x5aa5aa0a,
	0xaa9a0400,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x0000a0aa,
	0x0400aa45,
	0x5004a0aa,
	0x51049000,
	0xa2aa8a00,
	0x00aa4900,
	0xa05a0400,
	0x00aa4a00,
	0x00090045,
	0xaa081045,
	0x0400aaa6,
	0xaaaaaa9a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x20aaaaaa,
	0xaa595504,
	0xa09a0400,
	0x50005104,
	0x4a005504,
	0x4900a2aa,
	0x554500a2,
	0x4900a09a,
	0x104500aa,
	0x50450005,
	0xa2aaaa04,
	0xaa9a0500,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x550520aa,
	0x0000aa59,
	0x9204a09a,
	0x9a045000,
	0xa2aa0900,
	0x00a24900,
	0xa09a5555,
	0x00aa0900,
	0x00052049,
	0x9a00a049,
	0x0500a2aa,
	0xaaaaaa9a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00aaaaaa,
	0xa2aa9a45,
	0xa09a0005,
	0x50049204,
	0x09209a04,
	0x4900a2aa,
	0xaa5904a0,
	0x095020aa,
	0x204900aa,
	0xa2490045,
	0xa0aa9a00,
	0xaaaa4500,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaa4500aa,
	0x0005a2aa,
	0x9004a05a,
	0x9a045004,
	0xa2aa0520,
	0x04a04900,
	0x20aaaa5a,
	0x00aa0550,
	0x00450049,
	0x5a00a249,
	0x550020aa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00aaaaaa,
	0xa0aaaa49,
	0xa04a0005,
	0x90050000,
	0x05209a04,
	0x4900a2aa,
	0xaa9a04a0,
	0x045000aa,
	0x000000aa,
	0xa2490059,
	0x00aa5a00,
	0xaaaa5904,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaa4900aa,
	0x0045a0aa,
	0x0000a04a,
	0x9a049045,
	0xa2aa0520,
	0x04a04900,
	0x04aaaa9a,
	0x00aa0450,
	0x00590400,
	0x5a00a249,
	0x5a0500a2,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x20aaaaaa,
	0xa0aaaa49,
	0xa0090000,
	0xa0050000,
	0x45209a04,
	0x4900a2aa,
	0xaa9a04a2,
	0x000000aa,
	0x0000009a,
	0xa249005a,
	0x00a05a04,
	0xaaaa9a45,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaa0920aa,
	0x000020aa,
	0x1004a009,
	0x9a04a009,
	0xa2aa4500,
	0x00a24900,
	0x00a2aa9a,
	0x009a0000,
	0x009a0041,
	0x5a04a049,
	0xaa550020,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x20aaaaaa,
	0x20aaaa08,
	0xa0050000,
	0xa0041004,
	0x49009a04,
	0x4900a2aa,
	0xaa8a00a2,
	0x000000a2,
	0x0041005a,
	0xa049004a,
	0x04009a04,
	0xaaaaaa59,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x0000a0aa,
	0x5055008a,
	0x9004a004,
	0x9204a000,
	0xa2aa5900,
	0x00aa4900,
	0x05a00800,
	0x004a0055,
	0x000a0049,
	0x92052049,
	0x08000000,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xa2aaaaaa,
	0x008a0000,
	0xa0045055,
	0x90009004,
	0x59040000,
	0x4900a2aa,
	0x080020aa,
	0x005505a0,
	0x0049004a,
	0x00000009,
	0x00009045,
	0xaaaa0800,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x0010aaaa,
	0xa05a0409,
	0x92049004,
	0x00004000,
	0xa2aa5a05,
	0xa1aa4900,
	0x45900000,
	0x004900aa,
	0x00042049,
	0xa0550000,
	0x04000000,
	0xaaaaaa9a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x04510065,
	0x9000a25a,
	0x50059204,
	0x9a550000,
	0x4900a2aa,
	0x0550a6aa,
	0x20aa4510,
	0x20490009,
	0x05000055,
	0x0000a059,
	0xaa9a0400,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x55a6aaaa,
	0xaa9aa555,
	0x9a559a55,
	0x55555965,
	0xaaaaaa55,
	0xaaaa5965,
	0x595a5565,
	0xa559a5aa,
	0x9555a659,
	0xaa5a5555,
	0x55555555,
	0xaaaaaa9a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x655aa5aa,
	0x5a65aa9a,
	0xaa659a55,
	0xaa5a5555,
	0x5965aaaa,
	0x55aaaaaa,
	0xa6aa59a6,
	0xa659a555,
	0x5555a55a,
	0x5555aaaa,
	0xaa9a5555,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaa080020,
	0xaa088aa0,
	0x00008200,
	0xaaaa0a00,
	0xaaaa08a2,
	0xa0aa0aa2,
	0xaaaa0000,
	0xaa28aa20,
	0x8a20aaaa,
	0xaa20aaaa,
	0x0a0000aa,
	0xa20aa2aa,
	0xaaaaaa8a,
	0xa0aa0a20,
	0x00a2aa08,
	0x0020aaaa,
	0x8a00a208,
	0x82008a00,
	0x0a000000,
	0x0000aaaa,
	0x0000aa0a,
	0x0000a08a,
	0xaa00a00a,
	0xa0aa8a00,
	0xa0aa0000,
	0x00aa0800,
	0x00aa0000,
	0xaa08a00a,
	0x0a20aaaa,
	0xaa08a0aa,
	0xaaaa00a2,
	0x9a040020,
	0x9a004100,
	0x00004100,
	0xa2aa0500,
	0xa28a0000,
	0xa00a0000,
	0xa2080000,
	0x9a005a00,
	0x080020aa,
	0x000020aa,
	0x000000aa,
	0xa005208a,
	0xaaaaaa09,
	0xa0aa0420,
	0x00a29a04,
	0x0520aa5a,
	0x01209a55,
	0x49005904,
	0x55450065,
	0x0000a0aa,
	0x0000a045,
	0x5004a008,
	0x4a00a200,
	0x00aa5900,
	0x005a0400,
	0x008a0000,
	0x200a0045,
	0x9a05a004,
	0x0400aaaa,
	0x9a04a0aa,
	0xaa5a00a2,
	0x9a550520,
	0x5a040020,
	0x00654900,
	0x20aa5545,
	0x20595504,
	0xa0005105,
	0x9a005104,
	0x5a044200,
	0x554500a2,
	0x10550092,
	0x1045000a,
	0x2004a009,
	0xaaaaaa45,
	0xa09a0400,
	0x00a29a04,
	0x0520aa5a,
	0x00a0aaaa,
	0x49009a05,
	0xaa4900aa,
	0x550520aa,
	0x51052059,
	0x92049000,
	0x41205a00,
	0x00a25a04,
	0x00925555,
	0x00091055,
	0xa2052049,
	0xaa451004,
	0x0000aaaa,
	0x9a04a09a,
	0xaa5a00a2,
	0xaaaa0520,
	0x9a4500a2,
	0x00aa4900,
	0x00aaaa49,
	0x00aa9a45,
	0x4000aa45,
	0x5a009004,
	0x9a050020,
	0xaa5904a0,
	0xa05a04a0,
	0x00490004,
	0x0000a205,
	0xa2aaaa59,
	0xa09a0005,
	0x00a29a04,
	0x0020aa5a,
	0x00a2aa08,
	0x4900aa55,
	0xaa4900aa,
	0xaa4500aa,
	0xaa4500aa,
	0x90044000,
	0x00a05a04,
	0x04a09a45,
	0x04a0aa5a,
	0x0004a05a,
	0xaa450049,
	0xaa590400,
	0x0005a2aa,
	0x9a04a05a,
	0xaa5a00a2,
	0xaa080020,
	0xaa4900a2,
	0x00aa4900,
	0x00aaaa49,
	0x00aaaa49,
	0x4000aa49,
	0x5a050004,
	0xaa4500a0,
	0xaa9a04a0,
	0xa09a04a0,
	0x00400004,
	0x0400aa55,
	0xa0aaaa5a,
	0xa04a0005,
	0x00a29a04,
	0x0020aa5a,
	0x00a09a04,
	0x4900aa0a,
	0xaa4900aa,
	0xaa4900aa,
	0xaa4900aa,
	0x00044000,
	0x00a29a45,
	0x04a0aa59,
	0x04a0aa9a,
	0x0004a09a,
	0xaa590440,
	0xaa9a0520,
	0x0045a0aa,
	0x9a04a04a,
	0xaa5a00a2,
	0x9a550520,
	0xaa0900a0,
	0x00aa4900,
	0x20aaaa49,
	0x00aaaa49,
	0x4000aa49,
	0xaa550404,
	0xaa5900a2,
	0xaa9a04a2,
	0xa09a04a0,
	0x45400004,
	0x0520aa5a,
	0xa0aaaa9a,
	0xa0090000,
	0x00a29a04,
	0x0520aa5a,
	0x00209a55,
	0x4900aa04,
	0xaa4900aa,
	0xaa0920aa,
	0xa20920aa,
	0x95045000,
	0x00a2aa59,
	0x00a2aa5a,
	0x00a2aa9a,
	0x0005209a,
	0xaa9a5549,
	0xaaaa0520,
	0x000020aa,
	0x9a04a009,
	0xaa5a00a2,
	0xaaaa0520,
	0xaa004000,
	0x00aa4900,
	0x20aaaa49,
	0x20aaaa08,
	0x5000a008,
	0xaa9a9504,
	0xaa5a00a2,
	0xaa8a00a2,
	0x008a00a2,
	0x59490005,
	0x0520aaaa,
	0x20aaaaaa,
	0xa0050000,
	0x00a29a04,
	0x0520aa5a,
	0x5100aaaa,
	0x49009a00,
	0xaa4900aa,
	0x0000a0aa,
	0x0000a08a,
	0x9a045004,
	0x00a2aaaa,
	0x00aaaa5a,
	0x00aa0800,
	0x00450000,
	0xaaaaaa49,
	0xaaaa0520,
	0x505500aa,
	0x9a04a004,
	0xaa5a00a2,
	0xa2000020,
	0x4a005100,
	0x00aa4900,
	0xa2aaaa49,
	0xa28a0000,
	0x50050000,
	0xaaaa9a04,
	0xaa5a00a2,
	0x080020aa,
	0x000020aa,
	0xaa490055,
	0x0520aaaa,
	0x00aaaaaa,
	0xa0045055,
	0x00820000,
	0x00200a00,
	0x5a04a000,
	0x49000a00,
	0xaa4900aa,
	0x0010aaaa,
	0x0000aa09,
	0x9a049045,
	0x00a2aaaa,
	0xa1aaaa5a,
	0xa09a0000,
	0x00590400,
	0xaaaaaa49,
	0xaaaa0520,
	0xa05a04aa,
	0x00009004,
	0x0a000082,
	0x5a000020,
	0x59045a04,
	0x00aa4900,
	0xaaaaaa49,
	0xaa590065,
	0xa0550561,
	0xaaaa9a04,
	0xaa5a00a2,
	0x0550a6aa,
	0x5510a69a,
	0xaa49005a,
	0x0520aaaa,
	0x04a2aaaa,
	0x9000a25a,
	0x00410000,
	0x55a60900,
	0x9a555a55,
	0x59a555a5,
	0xaa59a5aa,
	0x55a6aaaa,
	0x55a5aa55,
	0x9a55aa59,
	0x65aaaaaa,
	0xaaaaaa5a,
	0xaa5a5565,
	0xa59a5555,
	0xaaaaaa59,
	0xaaaa55a6,
	0xaa9aa5aa,
	0x55559a55,
	0x59556559,
	0x5a5555a6,
	0x9aa59aa5,
	0xa5aa59a5,
	0xaaaaaa59,
	0xaa5aa5aa,
	0xaaaa65aa,
	0xaaaa9a55,
	0xaa5a65aa,
	0x55aaaaaa,
	0x5aa6aaaa,
	0xaa59a5aa,
	0x55a6aaaa,
	0x65aaaaaa,
	0x5a65aa9a,
	0x65595555,
	0x00a05955,
	0x00aaaa00,
	0x0a200a00,
	0x0000a2aa,
	0x00000082,
	0x8a000002,
	0x0aaa20aa,
	0xaa8a0020,
	0xa0aaaa08,
	0xaa08aa8a,
	0xaaaa0aa0,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x0a0000a0,
	0x0a0000aa,
	0xa2aa0a20,
	0x00820000,
	0x00020000,
	0x00a28a00,
	0x002008a2,
	0x8a00a08a,
	0xaa0aa0aa,
	0x0000aa08,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00a0aaaa,
	0x00aa0000,
	0x05200500,
	0x0000a2aa,
	0x00000041,
	0x49000001,
	0x055200aa,
	0x20490020,
	0xa0aa9a00,
	0x9204aa08,
	0xaa8a0000,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x005104a0,
	0x5545008a,
	0xa2aa0520,
	0x65595500,
	0x00514500,
	0x00aa5955,
	0x45100542,
	0x59452059,
	0xaa00a0aa,
	0x50059004,
	0xaaaaaa0a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x04a0aaaa,
	0x004a0055,
	0x05205545,
	0x5500a2aa,
	0x45006559,
	0x59550051,
	0x454020aa,
	0x20594520,
	0xa0aa5a05,
	0x10048a00,
	0xaa085005,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x009a04a0,
	0xaa490009,
	0xa2aa0520,
	0xaaaa5a00,
	0x00a24900,
	0x20aaaa5a,
	0x00204540,
	0xaa08208a,
	0x0a00a0aa,
	0xa2451004,
	0xaaaaaa04,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x04a0aaaa,
	0x0009209a,
	0x0520aa49,
	0x5a00a2aa,
	0x4900aaaa,
	0xaa5a00a2,
	0x5900a0aa,
	0xa08a0020,
	0xa0aaaa00,
	0x10040840,
	0x9a04a245,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x209a04a0,
	0x0a000005,
	0xa2aa0520,
	0xaa8a0000,
	0x00a24900,
	0xa2aa8a00,
	0x00205900,
	0x9a00a249,
	0x0040a0aa,
	0xa2491004,
	0xaaaa9a04,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x04a0aaaa,
	0x0005209a,
	0x05200a00,
	0x0000a2aa,
	0x4900aa8a,
	0x8a0000a2,
	0x5a04a2aa,
	0xaa594520,
	0xa0aa4a00,
	0x10040041,
	0x9a04a249,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x209a04a0,
	0x05000005,
	0xa2aa0520,
	0xaa490000,
	0x00a24900,
	0xa2aa4900,
	0x45205a04,
	0x4a20aa59,
	0x0049a0aa,
	0xa0091004,
	0xaaaa9a04,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x04a0aaaa,
	0x0045209a,
	0x05205545,
	0x5500a2aa,
	0x4900aa59,
	0x595500a2,
	0x9a04a2aa,
	0x2aaa4920,
	0xa0aa4920,
	0x90042049,
	0x9a05a009,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x009a04a0,
	0x55450045,
	0xa2aa0520,
	0xaa595500,
	0x00a24900,
	0xa2aa5955,
	0x00209a04,
	0x5900000a,
	0xa049a0aa,
	0x00009204,
	0xaaaa9a45,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x04a0aaaa,
	0x0049009a,
	0x0520aa49,
	0x5a00a2aa,
	0x4900aaaa,
	0xaa5a00a2,
	0x9a04a2aa,
	0x000a0020,
	0xa0aa5900,
	0x9a04a249,
	0xaa550000,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x009204a0,
	0xaa490059,
	0xa2aa0520,
	0xaaaa5a00,
	0x00a24900,
	0xa2aaaa5a,
	0x00209a04,
	0x5a05a005,
	0xaa49a0aa,
	0x05609a04,
	0xaaaaaa59,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00a0aaaa,
	0x00590400,
	0x00200800,
	0x0000a200,
	0x4900aa0a,
	0x0a0000a2,
	0x9a65aaaa,
	0xa55555a6,
	0xaaaa5a55,
	0x9a65aa59,
	0xaa5a55a5,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x050000a0,
	0x0800005a,
	0xa2000020,
	0xaa0a0000,
	0x00a24900,
	0xaaaa0a00,
	0x55a69a65,
	0xaa55aa55,
	0xaa59aaaa,
	0x55aa9aa5,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00a0aaaa,
	0x009a5500,
	0x00100400,
	0x00005200,
	0x4900aa05,
	0x050000a2,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x555555aa,
	0x5555a5aa,
	0x5a555596,
	0xaa555565,
	0x65aa59a5,
	0xaaaa5555,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x55aaaaaa,
	0xa5aa5a55,
	0x55965555,
	0x55655a55,
	0x59a5aa55,
	0x555565aa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaa20aa,
	0xaa08aaaa,
	0x8a20aaaa,
	0xaa28aaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaa08aaaa,
	0xaaaaaaaa,
	0x0aa2aaaa,
	0xaaaaaaaa,
	0x2028aaaa,
	0x20aa0a00,
	0xaaaaaaaa,
	0xaaaaaa08,
	0xaa0a0020,
	0x0a002028,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xa0aaaa08,
	0x0a000000,
	0xaaaa0aa2,
	0xaaaaaaaa,
	0x0a002028,
	0xaa5a00aa,
	0x9a04aaaa,
	0x0000aaaa,
	0x1024aa8a,
	0xaaaa0a00,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x9a04aaaa,
	0x0000a0aa,
	0x00000a00,
	0xaaaaaa00,
	0x1004aaaa,
	0x04aa0500,
	0xaaaaaa5a,
	0xaaaa9a04,
	0xaa4555a6,
	0x05001004,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xa0aa9a04,
	0x05000000,
	0xaa000000,
	0xaaaaaaaa,
	0x55109604,
	0xaa5a04aa,
	0x9204aaaa,
	0x00a0aa8a,
	0x9604aa59,
	0xaaaa5555,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x9a04aaaa,
	0x1055aaaa,
	0x00005505,
	0xaaaa5a00,
	0x0004aaaa,
	0x04a20500,
	0xaaaaaa9a,
	0xa08a0000,
	0xaa090000,
	0x55559604,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaa9a04,
	0x55550055,
	0x5a5505a1,
	0xaaaaaaaa,
	0x0a000004,
	0xaa8a00a2,
	0x0020aaaa,
	0x0000a209,
	0x9a04aa0a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x9a04aaaa,
	0x04a2aaaa,
	0x05a1aa55,
	0xaaaa5855,
	0x0004aaaa,
	0x00a20500,
	0xa2aaa80a,
	0xa2590500,
	0xaa045545,
	0xaaaa1a04,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaa9a04,
	0xaa5a05a2,
	0x08080000,
	0xaaaaaaaa,
	0x55109504,
	0xa00800a0,
	0x5504208a,
	0x5565aa55,
	0x1004aa04,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x9a04aaaa,
	0x45a00aa2,
	0x0000aa9a,
	0xaa9a2000,
	0x9004aaaa,
	0x05a05500,
	0x208aa004,
	0xaaaa5565,
	0x9a04aa69,
	0xaaaa0404,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x8aa09a04,
	0xaaaa45a0,
	0x10050000,
	0xaaaaaa9a,
	0x5a001004,
	0x900445a0,
	0x9a550449,
	0xaaaaaaaa,
	0x04049a04,
	0xaaaaaa5a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x9a04aaaa,
	0x49a045a0,
	0x05a1aaaa,
	0xaa5a1055,
	0x0804aaaa,
	0x45a00a10,
	0x04591004,
	0xaaaaaa5a,
	0x9a05a2aa,
	0xaa9a0004,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x49209a24,
	0xaaaa49a0,
	0x205505a1,
	0xaaaaaa5a,
	0x0a100404,
	0x10045920,
	0xaa9a0059,
	0x20aaaaaa,
	0x10049a05,
	0xaaaa0a00,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x9a20aaaa,
	0x08a25900,
	0x05a2aaaa,
	0xaa5a00aa,
	0x0004aaaa,
	0x59200504,
	0x205a0020,
	0xaa0a0000,
	0xaa450000,
	0x0a001004,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x59040020,
	0x8a0000a2,
	0x040000aa,
	0xaaaaaa5a,
	0x45061004,
	0x04205a20,
	0x0000a05a,
	0x0020aa0a,
	0x9204aa55,
	0xaaaa0500,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00a0aaaa,
	0x00aa5a05,
	0x00aaaa00,
	0xaa5a0500,
	0x9124aaaa,
	0x5a209505,
	0xa65a45a0,
	0xaa050000,
	0xaa595520,
	0x55559624,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x9a5500a6,
	0x590061aa,
	0x5500a0aa,
	0xaaaaaa9a,
	0xa95596a5,
	0x55a65aa6,
	0x5555aa9a,
	0x55a6aa55,
	0x9aa5aa5a,
	0xaaaa5565,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x55aaaaaa,
	0xa5aaaa55,
	0xa5aa5a55,
	0xaaaa5555,
	0x9aa6aaaa,
	0x5aa5aa65,
	0xaaaa59aa,
	0xaa5555a5,
	0xaaaa5aa6,
	0xaaaa9aa6,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaa5aa5aa,
	0x5a65aaaa,
	0x5a55aaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaa0a20aa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaa08aa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xa0aaaaaa,
	0x0aa2aa8a,
	0x00a2aaaa,
	0xaaaaaa00,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00aaaa08,
	0x08aa0800,
	0xaaaaaaaa,
	0x00aaaaaa,
	0xaa000000,
	0xaa8aa0aa,
	0xaaaa0aa2,
	0x8a0086a2,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaa08aaaa,
	0x080000aa,
	0xaa9a04aa,
	0xa0aa0aa2,
	0x000000aa,
	0x0000a000,
	0x05a20a00,
	0x65aa0aaa,
	0xaaaa4a00,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x008a0000,
	0x049a0400,
	0x0aa2aa9a,
	0x00aa20aa,
	0x50000000,
	0x0a000000,
	0x0a0a0000,
	0x590069aa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x0000aaaa,
	0x4550a58a,
	0xaa9a049a,
	0xa0aa05a2,
	0x0051a54a,
	0x00005055,
	0x00000500,
	0xa2aa0408,
	0xaaaa5900,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xa5490000,
	0x049a0400,
	0x05a2aa9a,
	0xa50aa0aa,
	0x5a140550,
	0x55455055,
	0x24040000,
	0x5a04a2aa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x5504aaaa,
	0x0000a259,
	0xaa9a048a,
	0xa2aa05a2,
	0x4120aa09,
	0x0051aa40,
	0x45a05545,
	0xa09a1414,
	0xaaaa5a05,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xa0595504,
	0x048a0000,
	0x05a2aa9a,
	0xaa05a2aa,
	0x9a805120,
	0xaa4900a0,
	0x101045a0,
	0x9a45a09a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x9a0420aa,
	0x504520aa,
	0xa29a0409,
	0xa2aa0502,
	0x5904aa05,
	0x60205a09,
	0x49a0aa49,
	0x209a1020,
	0xaaaaaa55,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00a2aaaa,
	0x00a29a00,
	0x04095155,
	0x0582a09a,
	0xaa04aaaa,
	0x5a695a04,
	0xaa495020,
	0x10205920,
	0xaa59205a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x0a0000a2,
	0xa25906a2,
	0xa09a0405,
	0xaa280541,
	0x9a04aa04,
	0x5020aaa5,
	0x5920aa49,
	0xa65a5620,
	0xaaaaaa5a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x45a0aaaa,
	0x45aa0000,
	0x2405a25a,
	0x0549209a,
	0x9a04aa20,
	0xaaaa9a00,
	0xaa4900a0,
	0x56005a20,
	0xaa5a245a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x000445a0,
	0x20aa658a,
	0x009a2045,
	0x9a040459,
	0x0a209a04,
	0x00a6aaaa,
	0x5a00aa49,
	0x00aa5a04,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x00a2aaaa,
	0xa04a1004,
	0x20450000,
	0x00590400,
	0x9a045a04,
	0xaa080020,
	0xaa4910aa,
	0x5a040004,
	0xaa9a00aa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x900500a2,
	0x0400a059,
	0x0500a059,
	0x9a05205a,
	0x00a09aa5,
	0x0020aa0a,
	0x0004a259,
	0x24aa9a04,
	0xaaaaaa5a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x20aaaaaa,
	0xa0599645,
	0xa6594500,
	0xa09a5500,
	0x9aa59a45,
	0x9a0510a6,
	0xaa590420,
	0x9a050025,
	0xaa5aa5aa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaa5565aa,
	0x5555aa5a,
	0x5555aa9a,
	0xaa55a6aa,
	0x55aaaaaa,
	0x45a0aa55,
	0x9565aa5a,
	0xa6aa9a55,
	0xaaaaaa9a,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xa6aaaaaa,
	0xaaaaaa59,
	0xaaaa5955,
	0xaaaa5aa5,
	0xaaaaaa59,
	0xaa55a6aa,
	0xaa9a55a6,
	0xaa55a5a6,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0x65aaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
	0xaaaaaaaa,
};

#endif /* _MEMJPN_H_ */
/* DON'T ADD STUFF AFTER THIS */
