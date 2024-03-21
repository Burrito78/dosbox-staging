/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *
 *  Copyright (C) 2024-2024  The DOSBox Staging Team
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

#if !defined(WIN32)

#include "dos_locale.h"

#include "checks.h"
#include "dosbox.h"
#include "string_utils.h"

#include <clocale>
#include <string>

CHECK_NARROWING();

static bool is_language_generic(const std::string language)
{
	// XXX - should it go into the table?
	return iequals(language, "C") || iequals(language, "POSIX");
}

// Split locale string into language and territory, drop the rest
static std::pair<std::string, std::string> split_locale(const char *value)
{
	std::string tmp = value; // language[_TERRITORY][.codeset][@modifier]

	tmp = tmp.substr(0, tmp.rfind('@')); // strip the modifier
	tmp = tmp.substr(0, tmp.rfind('.')); // strip the codeset
	
	std::pair<std::string, std::string> result = {};

	result.first = tmp.substr(0, tmp.find('_')); // get the language
	lowcase(result.first);

	const auto position = tmp.rfind('_');
	if (position != std::string::npos) {
		result.second = tmp.substr(position + 1); // get the teritory
		upcase(result.second);
	}

	return result;
}

static const std::map<std::string, DosCountry> PosixToDosCountry = {
	// XXX describe sorting order and search order
	{ "EU",    DosCountry::International  }, // European Union
	{ "EZ",    DosCountry::International  }, // Eurozone
	{ "UN",    DosCountry::International  }, // United Nations
	{ "XX",    DosCountry::International  }, // unknown state
	{ "XZ",    DosCountry::International  }, // international waters
	{ "US",    DosCountry::UnitedStates   },
	{ "QM",    DosCountry::UnitedStates   }, // used by ISRC
	{ "fr_CA", DosCountry::CanadaFrench   },
	// XXX DosCountry::LatinAmerica
	{ "CA",    DosCountry::CanadaEnglish  },
	{ "RU",    DosCountry::Russia         },
	{ "SU",    DosCountry::Russia         }, // Soviet Union
	{ "EG",    DosCountry::Egypt          },
	// XXX DosCountry::SouthAfrica
	{ "GR",    DosCountry::Greece         },
	{ "NL",    DosCountry::Netherlands    },
	{ "AN",    DosCountry::Netherlands    }, // Netherlands Antilles
	{ "BE",    DosCountry::Belgium        },
	{ "FR",    DosCountry::France         },
	{ "CP",    DosCountry::France         }, // Clipperton Island
	{ "FQ",    DosCountry::France         }, // French Southern and Antarctic Territories
	{ "FX",    DosCountry::France         }, // France, Metropolitan
	{ "ES",    DosCountry::Spain          },
	{ "EA",    DosCountry::Spain          }, // Ceuta, Melilla
	{ "IC",    DosCountry::Spain          }, // Canary Islands
	{ "XA",    DosCountry::Spain          }, // Canary Islands, used by Switzerland
	{ "HU",    DosCountry::Hungary        },
	{ "YU",    DosCountry::Yugoslavia     },
	{ "IT",    DosCountry::Italy          },
	{ "VA",    DosCountry::Italy          }, // Vatican City
	{ "RO",    DosCountry::Romania        },
	{ "CH",    DosCountry::Switzerland    },
	{ "CZ",    DosCountry::Czechia        },
	{ "CS",    DosCountry::Czechia        }, // Czechoslovakia
	{ "AT",    DosCountry::Austria        },
	{ "GB",    DosCountry::UnitedKingdom  },
	{ "UK",    DosCountry::UnitedKingdom  },
	{ "AC",    DosCountry::UnitedKingdom  }, // Ascension Island
	{ "CQ",    DosCountry::UnitedKingdom  }, // Island of Sark
	{ "DG",    DosCountry::UnitedKingdom  }, // Diego Garcia
	{ "TA",    DosCountry::UnitedKingdom  }, // Tristan da Cunha
	{ "XI",    DosCountry::UnitedKingdom  }, // Northern Ireland
	{ "DK",    DosCountry::Denmark        },
	{ "SE",    DosCountry::Sweden         },
	{ "NO",    DosCountry::Norway         },
	{ "NQ",    DosCountry::Norway         }, // Dronning Maud Land
	{ "PL",    DosCountry::Poland         },
	{ "DE",    DosCountry::Germany        },
	{ "DD",    DosCountry::Germany        }, // German Democratic Republic
	{ "MX",    DosCountry::Mexico         },
	{ "AR",    DosCountry::Argentina      },
	{ "BR",    DosCountry::Brazil         },
	{ "CL",    DosCountry::Chile          },
	{ "CO",    DosCountry::Colombia       },
	{ "VE",    DosCountry::Venezuela      },
	{ "MY",    DosCountry::Malaysia       },
	{ "AU",    DosCountry::Australia      },
	{ "ID",    DosCountry::Indonesia      },
	{ "PH",    DosCountry::Philippines    },
	{ "NZ",    DosCountry::NewZealand     },
	{ "SG",    DosCountry::Singapore      },
	{ "TH",    DosCountry::Thailand       },
	{ "KZ",    DosCountry::Kazakhstan     },
	{ "JP",    DosCountry::Japan          },
	{ "KR",    DosCountry::SouthKorea     },
	{ "VN",    DosCountry::Vietnam        },
	{ "VD",    DosCountry::Vietnam        }, // North Vietnam
	{ "CN",    DosCountry::China          },
	{ "TR",    DosCountry::Turkey         },
	{ "IN",    DosCountry::India          },
	{ "PK",    DosCountry::Pakistan       },
	// XXX DosCountry::AsiaEnglish
	{ "MA",    DosCountry::Morocco        },
	{ "DZ",    DosCountry::Algeria        },
	{ "TN",    DosCountry::Tunisia        },
	{ "NE",    DosCountry::Niger          },
	{ "BJ",    DosCountry::Benin          },
	{ "DY",    DosCountry::Benin          }, // Dahomey
	{ "NG",    DosCountry::Nigeria        },
	{ "FO",    DosCountry::FaroeIslands   },
	{ "PT",    DosCountry::Portugal       },
	{ "LU",    DosCountry::Luxembourg     },
	{ "IE",    DosCountry::Ireland        },
	{ "IS",    DosCountry::Iceland        },
	{ "AL",    DosCountry::Albania        },
	{ "MT",    DosCountry::Malta          },
	{ "FI",    DosCountry::Finland        },
	{ "AX",    DosCountry::Finland        }, // Åland Islands
	{ "BG",    DosCountry::Bulgaria       },
	{ "LT",    DosCountry::Lithuania      },
	{ "LV",    DosCountry::Latvia         },
	{ "EE",    DosCountry::Estonia        },
	{ "AM",    DosCountry::Armenia        },
	{ "BY",    DosCountry::Belarus        },
	{ "UA",    DosCountry::Ukraine        },
	{ "RS",    DosCountry::Serbia         },
	{ "ME",    DosCountry::Montenegro     },
	{ "SI",    DosCountry::Slovenia       },
	// XXX DosCountry::BosniaLatin
	// XXX DosCountry::BosniaCyrillic
	{ "MK",    DosCountry::NorthMacedonia },
	{ "SK",    DosCountry::Slovakia       },
        { "GT",    DosCountry::Guatemala      },
        { "SV",    DosCountry::ElSalvador     },
        { "HN",    DosCountry::Honduras       },
        { "NI",    DosCountry::Nicaragua      },
        { "CR",    DosCountry::CostaRica      },
        { "PA",    DosCountry::Panama         },
        { "BO",    DosCountry::Bolivia        },
	{ "EC",    DosCountry::Ecuador        },
        { "PY",    DosCountry::Paraguay       },
        { "UY",    DosCountry::Uruguay        },
	// XXX DosCountry::Arabic
	// XXX DosCountry::HongKong
	// XXX DosCountry::Taiwan
	// XXX DosCountry::Lebanon
	// XXX DosCountry::Jordan
	// XXX DosCountry::Syria
	// XXX DosCountry::Kuwait
	// XXX DosCountry::SaudiArabia
	// XXX DosCountry::Yemen
	// XXX DosCountry::Oman
	// XXX DosCountry::Emirates
	// XXX DosCountry::Israel
	// XXX DosCountry::Bahrain
	// XXX DosCountry::Qatar
	// XXX DosCountry::Mongolia
	// XXX DosCountry::Tajikistan
	// XXX DosCountry::Turkmenistan
	// XXX DosCountry::Azerbaijan
	// XXX DosCountry::Georgia
	// XXX DosCountry::Kyrgyzstan
	// XXX DosCountry::Uzbekistan

	// XXX https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2
	
	// XXX Officially assigned code elements
	// AD, AE, AF, AG, AI, AO, AQ, AS, AW, AZ, BA,
	// BB, BD, BF, BH, BI, BL, BM, BN, BQ, BS, BT, BV,
	// BW, BZ, CC, CD, CF, CG, CI, CK, CM, CU,
	// CV, CW, CX, CY, DJ, DM, DO, EH, ER,
	// ET, FJ, FK, FM, GA, GD, GE, GF, GG, GH, GI, GL, GM,
	// GN, GP, GQ, GS, GU, GW, GY, HK, HM, HT, IL,
	// IM, IO, IQ, IR, JE, JM, JO, KE, KG, KH, KI, KM, KN,
	// KP, KW, KY, LA, LB, LC, LI, LK, LR, LS, LY,
	// MC, MD, MF, MG, MH, ML, MM, MN, MO, MP, MQ, MR, MS, MU,
	// MV, MW, MZ, NA, NC, NF, NP, NR, NU,
	// OM, PE, PF, PG, PM, PN, PR, PS, PW, QA, RE,
	// RW, SA, SB, SC, SD, SH, SJ, SL, SM, SN,
	// SO, SR, SS, ST, SX, SY, SZ, TC, TD, TF, TG, TJ, TK, TL, TM,
	// TO, TT, TV, TW, TZ, UG, UM, UZ, VC, VG, VI,
	// VU, WF, WS, YE, YT, ZA, ZM, ZW
	// XXX misc
	//   QO - Outlying Oceania
	//   XK - Kosovo (temporary designation)
	//   BU - Burma
	//   NT - Neutral Zone, Divided between Iraq (IQ) and Saudi Arabia (SA) 
	//   TP - East Timor
	//   ZR - Zaire
	//   CT - Canton and Enderbury Islands
	//   HV - Upper Volta
	//   JT - Johnston Island
	//   MI - Midway Islands
	//   NH - New Hebrides
	//   PC - Pacific Islands (Trust Territory)
	//   PU - United States Miscellaneous Pacific Islands
	//   PZ - Panama Canal Zone
	//   RH - Southern Rhodesia
	//   WK - Wake Island
	//   YD - Yemen, Democratic, aka South Yemen

};


static DosCountry get_dos_country(const int category, const DosCountry fallback)
{
	const auto value_ptr = setlocale(category, "");
	if (!value_ptr) {
		return fallback;
	}

	const auto [language, teritory] = split_locale(value_ptr);

	if (is_language_generic(language)) {
		return DosCountry::International;
	}




/* XXX

*/






	return fallback; // XXX
}

HostLocale DOS_DetectHostLocale()
{
	HostLocale locale = {};

	locale.country   = get_dos_country(LC_ALL,      locale.country);
	locale.numeric   = get_dos_country(LC_NUMERIC,  locale.country);
	locale.time_date = get_dos_country(LC_TIME,     locale.country);
	locale.currency  = get_dos_country(LC_MONETARY, locale.country);

	// XXX locale.currency



	// XXX detect the following:
	// XXX locale.messages_language // XXX LC_MESSAGES
	// XXX locale.keyboard_layout

	return locale;
}

#endif
