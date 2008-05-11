/**
 * Axl Babel: encoding support for axl: iso-8859-6 coding
 * Copyright (C) 2008 Advanced Software Production Line, S.L.
 */

/* include base header */
#include <axl_babel_iso88596.h>

/**
 * @brief Creates the translation table for iso-8859-6 representation
 *  to move information from iso-8859-6 to utf-8 and viceversa.
 */
axlBabelTable * axl_babel_build_iso88596_table (void) {

	axlBabelTable * table;

	/* create the table to hold information translate iso-8859-6 encoding */
	table = axl_new (axlBabelTable, 256);

	/* store item associated to code 0 */
	table[0].size      = 1;
	table[0].buffer[0] = (unsigned char) 0; /* 0x0 */

	/* store item associated to code 1 */
	table[1].size      = 1;
	table[1].buffer[0] = (unsigned char) 1; /* 0x1 */

	/* store item associated to code 2 */
	table[2].size      = 1;
	table[2].buffer[0] = (unsigned char) 2; /* 0x2 */

	/* store item associated to code 3 */
	table[3].size      = 1;
	table[3].buffer[0] = (unsigned char) 3; /* 0x3 */

	/* store item associated to code 4 */
	table[4].size      = 1;
	table[4].buffer[0] = (unsigned char) 4; /* 0x4 */

	/* store item associated to code 5 */
	table[5].size      = 1;
	table[5].buffer[0] = (unsigned char) 5; /* 0x5 */

	/* store item associated to code 6 */
	table[6].size      = 1;
	table[6].buffer[0] = (unsigned char) 6; /* 0x6 */

	/* store item associated to code 7 */
	table[7].size      = 1;
	table[7].buffer[0] = (unsigned char) 7; /* 0x7 */

	/* store item associated to code 8 */
	table[8].size      = 1;
	table[8].buffer[0] = (unsigned char) 8; /* 0x8 */

	/* store item associated to code 9 */
	table[9].size      = 1;
	table[9].buffer[0] = (unsigned char) 9; /* 0x9 */

	/* store item associated to code 10 */
	table[10].size      = 1;
	table[10].buffer[0] = (unsigned char) 10; /* 0xa */

	/* store item associated to code 11 */
	table[11].size      = 1;
	table[11].buffer[0] = (unsigned char) 11; /* 0xb */

	/* store item associated to code 12 */
	table[12].size      = 1;
	table[12].buffer[0] = (unsigned char) 12; /* 0xc */

	/* store item associated to code 13 */
	table[13].size      = 1;
	table[13].buffer[0] = (unsigned char) 13; /* 0xd */

	/* store item associated to code 14 */
	table[14].size      = 1;
	table[14].buffer[0] = (unsigned char) 14; /* 0xe */

	/* store item associated to code 15 */
	table[15].size      = 1;
	table[15].buffer[0] = (unsigned char) 15; /* 0xf */

	/* store item associated to code 16 */
	table[16].size      = 1;
	table[16].buffer[0] = (unsigned char) 16; /* 0x10 */

	/* store item associated to code 17 */
	table[17].size      = 1;
	table[17].buffer[0] = (unsigned char) 17; /* 0x11 */

	/* store item associated to code 18 */
	table[18].size      = 1;
	table[18].buffer[0] = (unsigned char) 18; /* 0x12 */

	/* store item associated to code 19 */
	table[19].size      = 1;
	table[19].buffer[0] = (unsigned char) 19; /* 0x13 */

	/* store item associated to code 20 */
	table[20].size      = 1;
	table[20].buffer[0] = (unsigned char) 20; /* 0x14 */

	/* store item associated to code 21 */
	table[21].size      = 1;
	table[21].buffer[0] = (unsigned char) 21; /* 0x15 */

	/* store item associated to code 22 */
	table[22].size      = 1;
	table[22].buffer[0] = (unsigned char) 22; /* 0x16 */

	/* store item associated to code 23 */
	table[23].size      = 1;
	table[23].buffer[0] = (unsigned char) 23; /* 0x17 */

	/* store item associated to code 24 */
	table[24].size      = 1;
	table[24].buffer[0] = (unsigned char) 24; /* 0x18 */

	/* store item associated to code 25 */
	table[25].size      = 1;
	table[25].buffer[0] = (unsigned char) 25; /* 0x19 */

	/* store item associated to code 26 */
	table[26].size      = 1;
	table[26].buffer[0] = (unsigned char) 26; /* 0x1a */

	/* store item associated to code 27 */
	table[27].size      = 1;
	table[27].buffer[0] = (unsigned char) 27; /* 0x1b */

	/* store item associated to code 28 */
	table[28].size      = 1;
	table[28].buffer[0] = (unsigned char) 28; /* 0x1c */

	/* store item associated to code 29 */
	table[29].size      = 1;
	table[29].buffer[0] = (unsigned char) 29; /* 0x1d */

	/* store item associated to code 30 */
	table[30].size      = 1;
	table[30].buffer[0] = (unsigned char) 30; /* 0x1e */

	/* store item associated to code 31 */
	table[31].size      = 1;
	table[31].buffer[0] = (unsigned char) 31; /* 0x1f */

	/* store item associated to code 32 */
	table[32].size      = 1;
	table[32].buffer[0] = (unsigned char) 32; /* 0x20 */

	/* store item associated to code 33 */
	table[33].size      = 1;
	table[33].buffer[0] = (unsigned char) 33; /* 0x21 */

	/* store item associated to code 34 */
	table[34].size      = 1;
	table[34].buffer[0] = (unsigned char) 34; /* 0x22 */

	/* store item associated to code 35 */
	table[35].size      = 1;
	table[35].buffer[0] = (unsigned char) 35; /* 0x23 */

	/* store item associated to code 36 */
	table[36].size      = 1;
	table[36].buffer[0] = (unsigned char) 36; /* 0x24 */

	/* store item associated to code 37 */
	table[37].size      = 1;
	table[37].buffer[0] = (unsigned char) 37; /* 0x25 */

	/* store item associated to code 38 */
	table[38].size      = 1;
	table[38].buffer[0] = (unsigned char) 38; /* 0x26 */

	/* store item associated to code 39 */
	table[39].size      = 1;
	table[39].buffer[0] = (unsigned char) 39; /* 0x27 */

	/* store item associated to code 40 */
	table[40].size      = 1;
	table[40].buffer[0] = (unsigned char) 40; /* 0x28 */

	/* store item associated to code 41 */
	table[41].size      = 1;
	table[41].buffer[0] = (unsigned char) 41; /* 0x29 */

	/* store item associated to code 42 */
	table[42].size      = 1;
	table[42].buffer[0] = (unsigned char) 42; /* 0x2a */

	/* store item associated to code 43 */
	table[43].size      = 1;
	table[43].buffer[0] = (unsigned char) 43; /* 0x2b */

	/* store item associated to code 44 */
	table[44].size      = 1;
	table[44].buffer[0] = (unsigned char) 44; /* 0x2c */

	/* store item associated to code 45 */
	table[45].size      = 1;
	table[45].buffer[0] = (unsigned char) 45; /* 0x2d */

	/* store item associated to code 46 */
	table[46].size      = 1;
	table[46].buffer[0] = (unsigned char) 46; /* 0x2e */

	/* store item associated to code 47 */
	table[47].size      = 1;
	table[47].buffer[0] = (unsigned char) 47; /* 0x2f */

	/* store item associated to code 48 */
	table[48].size      = 1;
	table[48].buffer[0] = (unsigned char) 48; /* 0x30 */

	/* store item associated to code 49 */
	table[49].size      = 1;
	table[49].buffer[0] = (unsigned char) 49; /* 0x31 */

	/* store item associated to code 50 */
	table[50].size      = 1;
	table[50].buffer[0] = (unsigned char) 50; /* 0x32 */

	/* store item associated to code 51 */
	table[51].size      = 1;
	table[51].buffer[0] = (unsigned char) 51; /* 0x33 */

	/* store item associated to code 52 */
	table[52].size      = 1;
	table[52].buffer[0] = (unsigned char) 52; /* 0x34 */

	/* store item associated to code 53 */
	table[53].size      = 1;
	table[53].buffer[0] = (unsigned char) 53; /* 0x35 */

	/* store item associated to code 54 */
	table[54].size      = 1;
	table[54].buffer[0] = (unsigned char) 54; /* 0x36 */

	/* store item associated to code 55 */
	table[55].size      = 1;
	table[55].buffer[0] = (unsigned char) 55; /* 0x37 */

	/* store item associated to code 56 */
	table[56].size      = 1;
	table[56].buffer[0] = (unsigned char) 56; /* 0x38 */

	/* store item associated to code 57 */
	table[57].size      = 1;
	table[57].buffer[0] = (unsigned char) 57; /* 0x39 */

	/* store item associated to code 58 */
	table[58].size      = 1;
	table[58].buffer[0] = (unsigned char) 58; /* 0x3a */

	/* store item associated to code 59 */
	table[59].size      = 1;
	table[59].buffer[0] = (unsigned char) 59; /* 0x3b */

	/* store item associated to code 60 */
	table[60].size      = 1;
	table[60].buffer[0] = (unsigned char) 60; /* 0x3c */

	/* store item associated to code 61 */
	table[61].size      = 1;
	table[61].buffer[0] = (unsigned char) 61; /* 0x3d */

	/* store item associated to code 62 */
	table[62].size      = 1;
	table[62].buffer[0] = (unsigned char) 62; /* 0x3e */

	/* store item associated to code 63 */
	table[63].size      = 1;
	table[63].buffer[0] = (unsigned char) 63; /* 0x3f */

	/* store item associated to code 64 */
	table[64].size      = 1;
	table[64].buffer[0] = (unsigned char) 64; /* 0x40 */

	/* store item associated to code 65 */
	table[65].size      = 1;
	table[65].buffer[0] = (unsigned char) 65; /* 0x41 */

	/* store item associated to code 66 */
	table[66].size      = 1;
	table[66].buffer[0] = (unsigned char) 66; /* 0x42 */

	/* store item associated to code 67 */
	table[67].size      = 1;
	table[67].buffer[0] = (unsigned char) 67; /* 0x43 */

	/* store item associated to code 68 */
	table[68].size      = 1;
	table[68].buffer[0] = (unsigned char) 68; /* 0x44 */

	/* store item associated to code 69 */
	table[69].size      = 1;
	table[69].buffer[0] = (unsigned char) 69; /* 0x45 */

	/* store item associated to code 70 */
	table[70].size      = 1;
	table[70].buffer[0] = (unsigned char) 70; /* 0x46 */

	/* store item associated to code 71 */
	table[71].size      = 1;
	table[71].buffer[0] = (unsigned char) 71; /* 0x47 */

	/* store item associated to code 72 */
	table[72].size      = 1;
	table[72].buffer[0] = (unsigned char) 72; /* 0x48 */

	/* store item associated to code 73 */
	table[73].size      = 1;
	table[73].buffer[0] = (unsigned char) 73; /* 0x49 */

	/* store item associated to code 74 */
	table[74].size      = 1;
	table[74].buffer[0] = (unsigned char) 74; /* 0x4a */

	/* store item associated to code 75 */
	table[75].size      = 1;
	table[75].buffer[0] = (unsigned char) 75; /* 0x4b */

	/* store item associated to code 76 */
	table[76].size      = 1;
	table[76].buffer[0] = (unsigned char) 76; /* 0x4c */

	/* store item associated to code 77 */
	table[77].size      = 1;
	table[77].buffer[0] = (unsigned char) 77; /* 0x4d */

	/* store item associated to code 78 */
	table[78].size      = 1;
	table[78].buffer[0] = (unsigned char) 78; /* 0x4e */

	/* store item associated to code 79 */
	table[79].size      = 1;
	table[79].buffer[0] = (unsigned char) 79; /* 0x4f */

	/* store item associated to code 80 */
	table[80].size      = 1;
	table[80].buffer[0] = (unsigned char) 80; /* 0x50 */

	/* store item associated to code 81 */
	table[81].size      = 1;
	table[81].buffer[0] = (unsigned char) 81; /* 0x51 */

	/* store item associated to code 82 */
	table[82].size      = 1;
	table[82].buffer[0] = (unsigned char) 82; /* 0x52 */

	/* store item associated to code 83 */
	table[83].size      = 1;
	table[83].buffer[0] = (unsigned char) 83; /* 0x53 */

	/* store item associated to code 84 */
	table[84].size      = 1;
	table[84].buffer[0] = (unsigned char) 84; /* 0x54 */

	/* store item associated to code 85 */
	table[85].size      = 1;
	table[85].buffer[0] = (unsigned char) 85; /* 0x55 */

	/* store item associated to code 86 */
	table[86].size      = 1;
	table[86].buffer[0] = (unsigned char) 86; /* 0x56 */

	/* store item associated to code 87 */
	table[87].size      = 1;
	table[87].buffer[0] = (unsigned char) 87; /* 0x57 */

	/* store item associated to code 88 */
	table[88].size      = 1;
	table[88].buffer[0] = (unsigned char) 88; /* 0x58 */

	/* store item associated to code 89 */
	table[89].size      = 1;
	table[89].buffer[0] = (unsigned char) 89; /* 0x59 */

	/* store item associated to code 90 */
	table[90].size      = 1;
	table[90].buffer[0] = (unsigned char) 90; /* 0x5a */

	/* store item associated to code 91 */
	table[91].size      = 1;
	table[91].buffer[0] = (unsigned char) 91; /* 0x5b */

	/* store item associated to code 92 */
	table[92].size      = 1;
	table[92].buffer[0] = (unsigned char) 92; /* 0x5c */

	/* store item associated to code 93 */
	table[93].size      = 1;
	table[93].buffer[0] = (unsigned char) 93; /* 0x5d */

	/* store item associated to code 94 */
	table[94].size      = 1;
	table[94].buffer[0] = (unsigned char) 94; /* 0x5e */

	/* store item associated to code 95 */
	table[95].size      = 1;
	table[95].buffer[0] = (unsigned char) 95; /* 0x5f */

	/* store item associated to code 96 */
	table[96].size      = 1;
	table[96].buffer[0] = (unsigned char) 96; /* 0x60 */

	/* store item associated to code 97 */
	table[97].size      = 1;
	table[97].buffer[0] = (unsigned char) 97; /* 0x61 */

	/* store item associated to code 98 */
	table[98].size      = 1;
	table[98].buffer[0] = (unsigned char) 98; /* 0x62 */

	/* store item associated to code 99 */
	table[99].size      = 1;
	table[99].buffer[0] = (unsigned char) 99; /* 0x63 */

	/* store item associated to code 100 */
	table[100].size      = 1;
	table[100].buffer[0] = (unsigned char) 100; /* 0x64 */

	/* store item associated to code 101 */
	table[101].size      = 1;
	table[101].buffer[0] = (unsigned char) 101; /* 0x65 */

	/* store item associated to code 102 */
	table[102].size      = 1;
	table[102].buffer[0] = (unsigned char) 102; /* 0x66 */

	/* store item associated to code 103 */
	table[103].size      = 1;
	table[103].buffer[0] = (unsigned char) 103; /* 0x67 */

	/* store item associated to code 104 */
	table[104].size      = 1;
	table[104].buffer[0] = (unsigned char) 104; /* 0x68 */

	/* store item associated to code 105 */
	table[105].size      = 1;
	table[105].buffer[0] = (unsigned char) 105; /* 0x69 */

	/* store item associated to code 106 */
	table[106].size      = 1;
	table[106].buffer[0] = (unsigned char) 106; /* 0x6a */

	/* store item associated to code 107 */
	table[107].size      = 1;
	table[107].buffer[0] = (unsigned char) 107; /* 0x6b */

	/* store item associated to code 108 */
	table[108].size      = 1;
	table[108].buffer[0] = (unsigned char) 108; /* 0x6c */

	/* store item associated to code 109 */
	table[109].size      = 1;
	table[109].buffer[0] = (unsigned char) 109; /* 0x6d */

	/* store item associated to code 110 */
	table[110].size      = 1;
	table[110].buffer[0] = (unsigned char) 110; /* 0x6e */

	/* store item associated to code 111 */
	table[111].size      = 1;
	table[111].buffer[0] = (unsigned char) 111; /* 0x6f */

	/* store item associated to code 112 */
	table[112].size      = 1;
	table[112].buffer[0] = (unsigned char) 112; /* 0x70 */

	/* store item associated to code 113 */
	table[113].size      = 1;
	table[113].buffer[0] = (unsigned char) 113; /* 0x71 */

	/* store item associated to code 114 */
	table[114].size      = 1;
	table[114].buffer[0] = (unsigned char) 114; /* 0x72 */

	/* store item associated to code 115 */
	table[115].size      = 1;
	table[115].buffer[0] = (unsigned char) 115; /* 0x73 */

	/* store item associated to code 116 */
	table[116].size      = 1;
	table[116].buffer[0] = (unsigned char) 116; /* 0x74 */

	/* store item associated to code 117 */
	table[117].size      = 1;
	table[117].buffer[0] = (unsigned char) 117; /* 0x75 */

	/* store item associated to code 118 */
	table[118].size      = 1;
	table[118].buffer[0] = (unsigned char) 118; /* 0x76 */

	/* store item associated to code 119 */
	table[119].size      = 1;
	table[119].buffer[0] = (unsigned char) 119; /* 0x77 */

	/* store item associated to code 120 */
	table[120].size      = 1;
	table[120].buffer[0] = (unsigned char) 120; /* 0x78 */

	/* store item associated to code 121 */
	table[121].size      = 1;
	table[121].buffer[0] = (unsigned char) 121; /* 0x79 */

	/* store item associated to code 122 */
	table[122].size      = 1;
	table[122].buffer[0] = (unsigned char) 122; /* 0x7a */

	/* store item associated to code 123 */
	table[123].size      = 1;
	table[123].buffer[0] = (unsigned char) 123; /* 0x7b */

	/* store item associated to code 124 */
	table[124].size      = 1;
	table[124].buffer[0] = (unsigned char) 124; /* 0x7c */

	/* store item associated to code 125 */
	table[125].size      = 1;
	table[125].buffer[0] = (unsigned char) 125; /* 0x7d */

	/* store item associated to code 126 */
	table[126].size      = 1;
	table[126].buffer[0] = (unsigned char) 126; /* 0x7e */

	/* store item associated to code 127 */
	table[127].size      = 1;
	table[127].buffer[0] = (unsigned char) 127; /* 0x7f */

	/* store item associated to code 128 */
	table[128].size      = 2;
	table[128].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[128].buffer[1] = (unsigned char) 128; /* 0x80 */

	/* store item associated to code 129 */
	table[129].size      = 2;
	table[129].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[129].buffer[1] = (unsigned char) 129; /* 0x81 */

	/* store item associated to code 130 */
	table[130].size      = 2;
	table[130].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[130].buffer[1] = (unsigned char) 130; /* 0x82 */

	/* store item associated to code 131 */
	table[131].size      = 2;
	table[131].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[131].buffer[1] = (unsigned char) 131; /* 0x83 */

	/* store item associated to code 132 */
	table[132].size      = 2;
	table[132].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[132].buffer[1] = (unsigned char) 132; /* 0x84 */

	/* store item associated to code 133 */
	table[133].size      = 2;
	table[133].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[133].buffer[1] = (unsigned char) 133; /* 0x85 */

	/* store item associated to code 134 */
	table[134].size      = 2;
	table[134].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[134].buffer[1] = (unsigned char) 134; /* 0x86 */

	/* store item associated to code 135 */
	table[135].size      = 2;
	table[135].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[135].buffer[1] = (unsigned char) 135; /* 0x87 */

	/* store item associated to code 136 */
	table[136].size      = 2;
	table[136].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[136].buffer[1] = (unsigned char) 136; /* 0x88 */

	/* store item associated to code 137 */
	table[137].size      = 2;
	table[137].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[137].buffer[1] = (unsigned char) 137; /* 0x89 */

	/* store item associated to code 138 */
	table[138].size      = 2;
	table[138].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[138].buffer[1] = (unsigned char) 138; /* 0x8a */

	/* store item associated to code 139 */
	table[139].size      = 2;
	table[139].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[139].buffer[1] = (unsigned char) 139; /* 0x8b */

	/* store item associated to code 140 */
	table[140].size      = 2;
	table[140].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[140].buffer[1] = (unsigned char) 140; /* 0x8c */

	/* store item associated to code 141 */
	table[141].size      = 2;
	table[141].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[141].buffer[1] = (unsigned char) 141; /* 0x8d */

	/* store item associated to code 142 */
	table[142].size      = 2;
	table[142].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[142].buffer[1] = (unsigned char) 142; /* 0x8e */

	/* store item associated to code 143 */
	table[143].size      = 2;
	table[143].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[143].buffer[1] = (unsigned char) 143; /* 0x8f */

	/* store item associated to code 144 */
	table[144].size      = 2;
	table[144].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[144].buffer[1] = (unsigned char) 144; /* 0x90 */

	/* store item associated to code 145 */
	table[145].size      = 2;
	table[145].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[145].buffer[1] = (unsigned char) 145; /* 0x91 */

	/* store item associated to code 146 */
	table[146].size      = 2;
	table[146].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[146].buffer[1] = (unsigned char) 146; /* 0x92 */

	/* store item associated to code 147 */
	table[147].size      = 2;
	table[147].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[147].buffer[1] = (unsigned char) 147; /* 0x93 */

	/* store item associated to code 148 */
	table[148].size      = 2;
	table[148].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[148].buffer[1] = (unsigned char) 148; /* 0x94 */

	/* store item associated to code 149 */
	table[149].size      = 2;
	table[149].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[149].buffer[1] = (unsigned char) 149; /* 0x95 */

	/* store item associated to code 150 */
	table[150].size      = 2;
	table[150].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[150].buffer[1] = (unsigned char) 150; /* 0x96 */

	/* store item associated to code 151 */
	table[151].size      = 2;
	table[151].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[151].buffer[1] = (unsigned char) 151; /* 0x97 */

	/* store item associated to code 152 */
	table[152].size      = 2;
	table[152].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[152].buffer[1] = (unsigned char) 152; /* 0x98 */

	/* store item associated to code 153 */
	table[153].size      = 2;
	table[153].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[153].buffer[1] = (unsigned char) 153; /* 0x99 */

	/* store item associated to code 154 */
	table[154].size      = 2;
	table[154].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[154].buffer[1] = (unsigned char) 154; /* 0x9a */

	/* store item associated to code 155 */
	table[155].size      = 2;
	table[155].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[155].buffer[1] = (unsigned char) 155; /* 0x9b */

	/* store item associated to code 156 */
	table[156].size      = 2;
	table[156].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[156].buffer[1] = (unsigned char) 156; /* 0x9c */

	/* store item associated to code 157 */
	table[157].size      = 2;
	table[157].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[157].buffer[1] = (unsigned char) 157; /* 0x9d */

	/* store item associated to code 158 */
	table[158].size      = 2;
	table[158].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[158].buffer[1] = (unsigned char) 158; /* 0x9e */

	/* store item associated to code 159 */
	table[159].size      = 2;
	table[159].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[159].buffer[1] = (unsigned char) 159; /* 0x9f */

	/* store item associated to code 160 */
	table[160].size      = 2;
	table[160].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[160].buffer[1] = (unsigned char) 160; /* 0xa0 */

	/* store item associated to code 161 */
	table[161].size      = 1;
	table[161].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 162 */
	table[162].size      = 1;
	table[162].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 163 */
	table[163].size      = 1;
	table[163].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 164 */
	table[164].size      = 2;
	table[164].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[164].buffer[1] = (unsigned char) 164; /* 0xa4 */

	/* store item associated to code 165 */
	table[165].size      = 1;
	table[165].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 166 */
	table[166].size      = 1;
	table[166].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 167 */
	table[167].size      = 1;
	table[167].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 168 */
	table[168].size      = 1;
	table[168].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 169 */
	table[169].size      = 1;
	table[169].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 170 */
	table[170].size      = 1;
	table[170].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 171 */
	table[171].size      = 1;
	table[171].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 172 */
	table[172].size      = 2;
	table[172].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[172].buffer[1] = (unsigned char) 140; /* 0x8c */

	/* store item associated to code 173 */
	table[173].size      = 2;
	table[173].buffer[0] = (unsigned char) 194; /* 0xc2 */
	table[173].buffer[1] = (unsigned char) 173; /* 0xad */

	/* store item associated to code 174 */
	table[174].size      = 1;
	table[174].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 175 */
	table[175].size      = 1;
	table[175].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 176 */
	table[176].size      = 1;
	table[176].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 177 */
	table[177].size      = 1;
	table[177].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 178 */
	table[178].size      = 1;
	table[178].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 179 */
	table[179].size      = 1;
	table[179].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 180 */
	table[180].size      = 1;
	table[180].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 181 */
	table[181].size      = 1;
	table[181].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 182 */
	table[182].size      = 1;
	table[182].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 183 */
	table[183].size      = 1;
	table[183].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 184 */
	table[184].size      = 1;
	table[184].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 185 */
	table[185].size      = 1;
	table[185].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 186 */
	table[186].size      = 1;
	table[186].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 187 */
	table[187].size      = 2;
	table[187].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[187].buffer[1] = (unsigned char) 155; /* 0x9b */

	/* store item associated to code 188 */
	table[188].size      = 1;
	table[188].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 189 */
	table[189].size      = 1;
	table[189].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 190 */
	table[190].size      = 1;
	table[190].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 191 */
	table[191].size      = 2;
	table[191].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[191].buffer[1] = (unsigned char) 159; /* 0x9f */

	/* store item associated to code 192 */
	table[192].size      = 1;
	table[192].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 193 */
	table[193].size      = 2;
	table[193].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[193].buffer[1] = (unsigned char) 161; /* 0xa1 */

	/* store item associated to code 194 */
	table[194].size      = 2;
	table[194].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[194].buffer[1] = (unsigned char) 162; /* 0xa2 */

	/* store item associated to code 195 */
	table[195].size      = 2;
	table[195].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[195].buffer[1] = (unsigned char) 163; /* 0xa3 */

	/* store item associated to code 196 */
	table[196].size      = 2;
	table[196].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[196].buffer[1] = (unsigned char) 164; /* 0xa4 */

	/* store item associated to code 197 */
	table[197].size      = 2;
	table[197].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[197].buffer[1] = (unsigned char) 165; /* 0xa5 */

	/* store item associated to code 198 */
	table[198].size      = 2;
	table[198].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[198].buffer[1] = (unsigned char) 166; /* 0xa6 */

	/* store item associated to code 199 */
	table[199].size      = 2;
	table[199].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[199].buffer[1] = (unsigned char) 167; /* 0xa7 */

	/* store item associated to code 200 */
	table[200].size      = 2;
	table[200].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[200].buffer[1] = (unsigned char) 168; /* 0xa8 */

	/* store item associated to code 201 */
	table[201].size      = 2;
	table[201].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[201].buffer[1] = (unsigned char) 169; /* 0xa9 */

	/* store item associated to code 202 */
	table[202].size      = 2;
	table[202].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[202].buffer[1] = (unsigned char) 170; /* 0xaa */

	/* store item associated to code 203 */
	table[203].size      = 2;
	table[203].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[203].buffer[1] = (unsigned char) 171; /* 0xab */

	/* store item associated to code 204 */
	table[204].size      = 2;
	table[204].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[204].buffer[1] = (unsigned char) 172; /* 0xac */

	/* store item associated to code 205 */
	table[205].size      = 2;
	table[205].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[205].buffer[1] = (unsigned char) 173; /* 0xad */

	/* store item associated to code 206 */
	table[206].size      = 2;
	table[206].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[206].buffer[1] = (unsigned char) 174; /* 0xae */

	/* store item associated to code 207 */
	table[207].size      = 2;
	table[207].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[207].buffer[1] = (unsigned char) 175; /* 0xaf */

	/* store item associated to code 208 */
	table[208].size      = 2;
	table[208].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[208].buffer[1] = (unsigned char) 176; /* 0xb0 */

	/* store item associated to code 209 */
	table[209].size      = 2;
	table[209].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[209].buffer[1] = (unsigned char) 177; /* 0xb1 */

	/* store item associated to code 210 */
	table[210].size      = 2;
	table[210].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[210].buffer[1] = (unsigned char) 178; /* 0xb2 */

	/* store item associated to code 211 */
	table[211].size      = 2;
	table[211].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[211].buffer[1] = (unsigned char) 179; /* 0xb3 */

	/* store item associated to code 212 */
	table[212].size      = 2;
	table[212].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[212].buffer[1] = (unsigned char) 180; /* 0xb4 */

	/* store item associated to code 213 */
	table[213].size      = 2;
	table[213].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[213].buffer[1] = (unsigned char) 181; /* 0xb5 */

	/* store item associated to code 214 */
	table[214].size      = 2;
	table[214].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[214].buffer[1] = (unsigned char) 182; /* 0xb6 */

	/* store item associated to code 215 */
	table[215].size      = 2;
	table[215].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[215].buffer[1] = (unsigned char) 183; /* 0xb7 */

	/* store item associated to code 216 */
	table[216].size      = 2;
	table[216].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[216].buffer[1] = (unsigned char) 184; /* 0xb8 */

	/* store item associated to code 217 */
	table[217].size      = 2;
	table[217].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[217].buffer[1] = (unsigned char) 185; /* 0xb9 */

	/* store item associated to code 218 */
	table[218].size      = 2;
	table[218].buffer[0] = (unsigned char) 216; /* 0xd8 */
	table[218].buffer[1] = (unsigned char) 186; /* 0xba */

	/* store item associated to code 219 */
	table[219].size      = 1;
	table[219].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 220 */
	table[220].size      = 1;
	table[220].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 221 */
	table[221].size      = 1;
	table[221].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 222 */
	table[222].size      = 1;
	table[222].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 223 */
	table[223].size      = 1;
	table[223].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 224 */
	table[224].size      = 2;
	table[224].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[224].buffer[1] = (unsigned char) 128; /* 0x80 */

	/* store item associated to code 225 */
	table[225].size      = 2;
	table[225].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[225].buffer[1] = (unsigned char) 129; /* 0x81 */

	/* store item associated to code 226 */
	table[226].size      = 2;
	table[226].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[226].buffer[1] = (unsigned char) 130; /* 0x82 */

	/* store item associated to code 227 */
	table[227].size      = 2;
	table[227].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[227].buffer[1] = (unsigned char) 131; /* 0x83 */

	/* store item associated to code 228 */
	table[228].size      = 2;
	table[228].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[228].buffer[1] = (unsigned char) 132; /* 0x84 */

	/* store item associated to code 229 */
	table[229].size      = 2;
	table[229].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[229].buffer[1] = (unsigned char) 133; /* 0x85 */

	/* store item associated to code 230 */
	table[230].size      = 2;
	table[230].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[230].buffer[1] = (unsigned char) 134; /* 0x86 */

	/* store item associated to code 231 */
	table[231].size      = 2;
	table[231].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[231].buffer[1] = (unsigned char) 135; /* 0x87 */

	/* store item associated to code 232 */
	table[232].size      = 2;
	table[232].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[232].buffer[1] = (unsigned char) 136; /* 0x88 */

	/* store item associated to code 233 */
	table[233].size      = 2;
	table[233].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[233].buffer[1] = (unsigned char) 137; /* 0x89 */

	/* store item associated to code 234 */
	table[234].size      = 2;
	table[234].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[234].buffer[1] = (unsigned char) 138; /* 0x8a */

	/* store item associated to code 235 */
	table[235].size      = 2;
	table[235].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[235].buffer[1] = (unsigned char) 139; /* 0x8b */

	/* store item associated to code 236 */
	table[236].size      = 2;
	table[236].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[236].buffer[1] = (unsigned char) 140; /* 0x8c */

	/* store item associated to code 237 */
	table[237].size      = 2;
	table[237].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[237].buffer[1] = (unsigned char) 141; /* 0x8d */

	/* store item associated to code 238 */
	table[238].size      = 2;
	table[238].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[238].buffer[1] = (unsigned char) 142; /* 0x8e */

	/* store item associated to code 239 */
	table[239].size      = 2;
	table[239].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[239].buffer[1] = (unsigned char) 143; /* 0x8f */

	/* store item associated to code 240 */
	table[240].size      = 2;
	table[240].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[240].buffer[1] = (unsigned char) 144; /* 0x90 */

	/* store item associated to code 241 */
	table[241].size      = 2;
	table[241].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[241].buffer[1] = (unsigned char) 145; /* 0x91 */

	/* store item associated to code 242 */
	table[242].size      = 2;
	table[242].buffer[0] = (unsigned char) 217; /* 0xd9 */
	table[242].buffer[1] = (unsigned char) 146; /* 0x92 */

	/* store item associated to code 243 */
	table[243].size      = 1;
	table[243].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 244 */
	table[244].size      = 1;
	table[244].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 245 */
	table[245].size      = 1;
	table[245].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 246 */
	table[246].size      = 1;
	table[246].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 247 */
	table[247].size      = 1;
	table[247].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 248 */
	table[248].size      = 1;
	table[248].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 249 */
	table[249].size      = 1;
	table[249].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 250 */
	table[250].size      = 1;
	table[250].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 251 */
	table[251].size      = 1;
	table[251].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 252 */
	table[252].size      = 1;
	table[252].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 253 */
	table[253].size      = 1;
	table[253].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 254 */
	table[254].size      = 1;
	table[254].buffer[0] = 0; /* unsupported translation */

	/* store item associated to code 255 */
	table[255].size      = 1;
	table[255].buffer[0] = 0; /* unsupported translation */

	/* return table created */
	return table;

} /* end axl_babel_build_iso88596_table */

