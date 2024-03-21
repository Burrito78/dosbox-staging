/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *
 *  Copyright (C) 2020-2023  The DOSBox Staging Team
 *  Copyright (C) 2002-2021  The DOSBox Team
 *
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
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef DOSBOX_DOS_LOCALE_H
#define DOSBOX_DOS_LOCALE_H

#include "setup.h"

#include <cstdint>
#include <string>

// ***************************************************************************
// List of countries
// ***************************************************************************

// Sources of the country numbers:
// - MS-DOS 6.22, COUNTRY.TXT file
// - PC-DOS 2000, HELP COUNTRY command, information table
// - DR-DOS 7.03, HELP, Table 9-2: Country Codes and Code Pages
// - FreeDOS 1.3, country.asm (source code)
// - Paragon PTS DOS 2000 Pro manual
// - OS/2 Warp 4.52, keyboard.pdf file
// - https://en.wikipedia.org/wiki/List_of_country_calling_codes
//   (used for remaining countries, especially where we have keyboard layout)

// TODO: 'Ralph Brown Interrupt List' also mentions countries listed below.
//       Moreover, OS/2 Warp 4.52 contains definitions which can be used for
//       historic settings for several countries.
// 51  = Peru                          53  = Cuba
// 93  = Afghanistan                   94  = Sri Lanka
// 98  = Iran                          112 = Belarus
// 218 = Libya                         220 = Gambia
// 221 = Senegal                       222 = Maruitania
// 223 = Mali                          224 = African Guinea
// 225 = Ivory Coast                   226 = Burkina Faso
// 228 = Togo                          230 = Mauritius
// 231 = Liberia                       232 = Sierra Leone
// 233 = Ghana                         235 = Chad
// 236 = Centra African Republic       237 = Cameroon
// 238 = Cape Verde Islands            239 = Sao Tome and Principe
// 240 = Equatorial Guinea             241 = Gabon
// 242 = Congo                         243 = Zaire
// 244 = Angola                        245 = Guinea-Bissau
// 246 = Diego Garcia                  247 = Ascension Isle
// 248 = Seychelles                    249 = Sudan
// 250 = Rwhanda                       251 = Ethiopia
// 252 = Somalia                       253 = Djibouti
// 254 = Kenya                         255 = Tanzania
// 256 = Uganda                        257 = Burundi
// 259 = Mozambique                    260 = Zambia
// 261 = Madagascar                    262 = Reunion Island
// 263 = Zimbabwe                      264 = Namibia
// 265 = Malawi                        266 = Lesotho
// 267 = Botswana                      268 = Swaziland
// 269 = Comoros                       270 = Mayotte
// 290 = St. Helena                    297 = Aruba
// 299 = Greenland                     350 = Gibraltar
// 357 = Cyprus                        373 = Moldova
// 500 = Falkland Islands              501 = Belize
// 508 = St. Pierre and Miquelon       509 = Haiti
// 590 = Guadeloupe                    592 = Guyana
// 594 = French Guiana                 596 = Martinique / French Antilles
// 597 = Suriname                      599 = Netherland Antilles
// 670 = Saipan / N. Mariana Island    671 = Guam
// 672 = Norfolk Island (Australia) / Christmas Island/Cocos Islands / Antarctica
// 673 = Brunei Darussalam             674 = Nauru
// 675 = Papua New Guinea              676 = Tonga Islands
// 677 = Solomon Islands               678 = Vanuatu
// 679 = Fiji                          680 = Palau
// 681 = Wallis & Futuna               682 = Cook Islands
// 683 = Niue                          684 = American Samoa
// 685 = Western Samoa                 686 = Kiribati
// 687 = New Caledonia                 688 = Tuvalu
// 689 = French Polynesia              690 = Tokealu
// 691 = Micronesia                    692 = Marshall Islands
// 809 = Antigua and Barbuda / Anguilla / Bahamas / Barbados / Bermuda
//       British Virgin Islands / Cayman Islands / Dominica
//       Dominican Republic / Grenada / Jamaica / Montserra
//       St. Kitts and Nevis / St. Lucia / St. Vincent and Grenadines
//       Trinidad and Tobago / Turks and Caicos
// 850 = North Korea                   853 = Macao
// 855 = Cambodia                      856 = Laos
// 880 = Bangladesh                    960 = Maldives
// 964 = Iraq                          969 = Yemen
// 971 = United Arab Emirates          975 = Bhutan
// 977 = Nepal                         995 = Myanmar (Burma)

enum class DosCountry : uint16_t {
	International   = 0,   // internal, not used by any DOS
	UnitedStates    = 1,   // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2 (*)
	CanadaFrench    = 2,   // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	LatinAmerica    = 3,   // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	CanadaEnglish   = 4,   // MS-DOS,                                   OS/2 (*)
	Russia          = 7,   // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Egypt           = 20,  //                                           OS/2
	SouthAfrica     = 27,  // MS-DOS,                                   OS/2
	Greece          = 30,  // MS-DOS, PC-DOS,         FreeDOS,          OS/2
	Netherlands     = 31,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Belgium         = 32,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	France          = 33,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Spain           = 34,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2 (*)
	// 35 - unofficial duplicate of Bulgaria
	Hungary         = 36,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS,          OS/2
	Yugoslavia      = 38,  // MS-DOS, PC-DOS,         FreeDOS,          OS/2
	Italy           = 39,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Romania         = 40,  // MS-DOS, PC-DOS,         FreeDOS,          OS/2
	Switzerland     = 41,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Czechia         = 42,  // MS-DOS, PC-DOS,                           OS/2 (*)
	Austria         = 43,  // MS-DOS                  FreeDOS,          OS/2
	UnitedKingdom   = 44,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Denmark         = 45,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Sweden          = 46,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Norway          = 47,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Poland          = 48,  // MS-DOS, PC-DOS,         FreeDOS,          OS/2
	Germany         = 49,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Mexico          = 52,  // MS-DOS,                                   OS/2
	Argentina       = 54,  // MS-DOS,                 FreeDOS,          OS/2
	Brazil          = 55,  // MS-DOS, PC-DOS,         FreeDOS,          OS/2
	Chile           = 56,  // MS-DOS
	Colombia        = 57,  // MS-DOS,                                   OS/2
	Venezuela       = 58,  // MS-DOS,                                   OS/2
	Malaysia        = 60,  // MS-DOS,                 FreeDOS
	Australia       = 61,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS,          OS/2 (*)
	Indonesia       = 62,  //                                           OS/2
	Philippines     = 63,
	NewZealand      = 64,  // MS-DOS,                                   OS/2
	Singapore       = 65,  // MS-DOS,                 FreeDOS,          OS/2
	Thailand        = 66,  // WIN-ME,                                   OS/2
	Kazakhstan      = 77,
	Japan           = 81,  // MS-DOS, PC-DOS,         FreeDOS, Paragon, OS/2
	SouthKorea      = 82,  // MS-DOS,                 FreeDOS, Paragon, OS/2
	Vietnam         = 84,
	China           = 86,  // MS-DOS,                 FreeDOS, Paragon, OS/2
	// 88 - duplicate of Taiwan
	Turkey          = 90,  // MS-DOS, PC-DOS, DR-DOS, FreeDOS,          OS/2
	India           = 91,  // MS-DOS,                 FreeDOS,          OS/2
	Pakistan        = 92,  //                                           OS/2
	AsiaEnglish     = 99,  //                                           OS/2
	Morocco         = 212, //                                           OS/2
	Algeria         = 213, //                                           OS/2
	Tunisia         = 216, //                                           OS/2
	Niger           = 227,
	Benin           = 229,
	Nigeria         = 234,
	FaroeIslands    = 298,
	Portugal        = 351, // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Luxembourg      = 352, //                                           OS/2
	Ireland         = 353, // MS-DOS,                                   OS/2
	Iceland         = 354, // MS-DOS, PC-DOS,                           OS/2
	Albania         = 355, // MS-DOS, PC-DOS,                           OS/2
	Malta           = 356,
	Finland         = 358, // MS-DOS, PC-DOS, DR-DOS, FreeDOS, Paragon, OS/2
	Bulgaria        = 359, // MS-DOS, PC-DOS,         FreeDOS,          OS/2
	Lithuania       = 370, //                                           OS/2
	Latvia          = 371, //                                           OS/2
	Estonia         = 372, // WIN-ME,                                   OS/2
	Armenia         = 374,
	Belarus         = 375, // WIN-ME                  FreeDOS,          OS/2
	Ukraine         = 380, // WIN-ME                  FreeDOS,          OS/2
	Serbia          = 381, // MS-DOS, PC-DOS,         FreeDOS,          OS/2 (*)
	Montenegro      = 382,
	// 384 - duplicate of Croatia
	Croatia         = 385, // MS-DOS, PC-DOS,         FreeDOS,          OS/2 (*)
	Slovenia        = 386, // MS-DOS, PC-DOS,         FreeDOS,          OS/2
	BosniaLatin     = 387, // MS-DOS, PC-DOS,         FreeDOS,          OS/2
	BosniaCyrillic  = 388, //         PC-DOS,
	NorthMacedonia  = 389, // MS-DOS, PC-DOS,         FreeDOS,          OS/2
	Slovakia        = 421, // MS-DOS, PC-DOS,                           OS/2 (*)
	// 422 - used for Slovakia by CP-DOS and OS/2
        Guatemala       = 502, //                                           OS/2
        ElSalvador      = 503, //                                           OS/2
        Honduras        = 504, //                                           OS/2
        Nicaragua       = 505, //                                           OS/2
        CostaRica       = 506, //                                           OS/2
        Panama          = 507, //                                           OS/2
        Bolivia         = 591, //                                           OS/2
	Ecuador         = 593, // MS-DOS,                                   OS/2
        Paraguay        = 595, //                                           OS/2
        Uruguay         = 598, //                                           OS/2
	Arabic          = 785, // MS-DOS,                 FreeDOS, Paragon, OS/2
	HongKong        = 852, // MS-DOS
	Taiwan          = 886, // MS-DOS,                                   OS/2 (*)
	Lebanon         = 961, //                                           OS/2
	Jordan          = 962, //                                           OS/2
	Syria           = 963, //                                           OS/2
	Kuwait          = 965, //                                           OS/2
	SaudiArabia     = 966, //                                           OS/2
	Yemen           = 967, //                                           OS/2
	Oman            = 968, //                                           OS/2
	Emirates        = 971, //                                           OS/2
	Israel          = 972, // MS-DOS,                 FreeDOS, Paragon, OS/2	
	Bahrain         = 973, //                                           OS/2
	Qatar           = 974, //                                           OS/2
	Mongolia        = 976,
	Tajikistan      = 992,
	Turkmenistan    = 993,
	Azerbaijan      = 994,
	Georgia         = 995,
	Kyrgyzstan      = 996,
	Uzbekistan      = 998,

	// (*) Remarks:
	// - MS-DOS, PC-DOS, and OS/2 use country code 381 for both Serbia and Montenegro
	// - MS-DOS and PC-DOS use country code 61 also for International English
	// - PC-DOS uses country code 381 also for Yugoslavia Cyrillic
	// - MS-DOS (contrary to PC-DOS or Windows ME) uses code 384 (not 385)
	//   for Croatia, FreeDOS follows MS-DOS standard - this was most likely
	//   a bug in the OS, Croatia calling code is 385
	// - PC-DOS and OS/2 use country code 421 for Czechia and 422 for Slovakia
	// - FreeDOS uses country code 042 for Czechoslovakia
	// - FreeDOS calls country code 785 Middle-East, MS-DOS calls it Arabic South
	// - Paragon PTS DOS uses country code 61 only for Australia
	// - Paragon PTS DOS and OS/2 use country code 88 for Taiwan, DOS 6.22 uses
	//   this code as a duplicate of 886, probably for compatibility
	// - OS/2 uses country code 1 also for Canada, English, it does not use 4
	// - OS/2 uses country 34 also for Catalunya

	// FreeDOS also supports the following, not yet handled here:
	// - Belgium/Dutch        40032
	// - Belgium/French       41032
	// - Belgium/German       42032
	// - Spain/Spanish        40034
	// - Spain/Catalan        41034
	// - Spain/Gallegan       42034
	// - Spain/Basque         43034
	// - Switzerland/German   40041
	// - Switzerland/French   41041
	// - Switzerland/Italian  42041
};

struct HostLocale { // XXX use it
	// XXX
	std::string messages = {};
	std::string keyboard = {};

	DosCountry country     = DosCountry::International;

	DosCountry numeric     = DosCountry::International;
	DosCountry time_date   = DosCountry::International;
	DosCountry currency    = DosCountry::International;
};

HostLocale DOS_DetectHostLocale();















constexpr uint16_t DefaultCodePage437 = 437;

std::string DOS_GenerateListCountriesMessage();

bool DOS_SetCountry(const uint16_t country_id);
uint16_t DOS_GetCountry();
void DOS_RefreshCountryInfo(const bool keyboard_layout_changed = false);

std::string DOS_GetBundledCodePageFileName(const uint16_t code_page);

uint16_t DOS_GetCodePageFromCountry(const uint16_t country);

std::string DOS_CheckLanguageToLayoutException(const std::string& language_code);

uint16_t DOS_GetDefaultCountry();

bool DOS_GetCountryFromLayout(const std::string& layout, uint16_t& country);

std::string DOS_GetLayoutFromHost();

// Lifecycle

void DOS_Locale_Init(Section* sec);

// We need a separate function to support '--list-countries' command line switch
// (and possibly others in the future) - it needs translated strings, but does
// not initialize DOSBox fully.
void DOS_Locale_AddMessages();

#endif
