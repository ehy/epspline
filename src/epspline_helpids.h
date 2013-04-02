/*
 * Copyright (c) 2013 Ed Hynan
 *
 * This file is part of Epspline
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */


#ifndef _epspline_helpids_INCLUDED 
#define _epspline_helpids_INCLUDED 1

/* name of the wx help "book" file */
#define HELP_BOOK_NAME	"help.zip"

/* Help Key String: macro _HKS */
#ifdef _HKS
#error "Help Key String: macro _HKS was already defined"
#endif

#if defined(HELP_KEYS_USE_TRANSLATIONS)
/* NOTE: if help keys are translated,
 * with this macro, then xgettext must
 * be given option -k_HKS
 */
#define _HKS(s)		_(s)
#else /* do not translate help key strings */
#define _HKS(s)		wxT(s)
#endif


#define IDI_TitlePage		999999999
#define IDS_TitlePage		"Title Page"
#define IDF_TitlePage		"epspline.html"

#define IDI_HelpTOC		1000000000
#define IDS_HelpTOC		"Contents"
#define IDF_HelpTOC		"epsplineli1.html"

#define IDI_Introduction		1000000
#define IDS_Introduction		"Introduction"
#define IDF_Introduction		"epsplinech1.html"

#define IDI_SoftwareIntroSection		16
#define IDS_SoftwareIntroSection		"Purpose"
#define IDF_SoftwareIntroSection		"epsplinese1.html"

#define IDI_RequirementsandStatus		64
#define IDS_RequirementsandStatus		"Requirements and Status"
#define IDF_RequirementsandStatus		"epsplinese2.html"

#define IDI_UsingEpspline		301000
#define IDS_UsingEpspline		"Using Epspline"
#define IDF_UsingEpspline		"epsplinech2.html"

#define IDI_TheMainWindowInterface		80
#define IDS_TheMainWindowInterface		"The Main Window Interface"
#define IDF_TheMainWindowInterface		"epsplinese3.html"

#define IDI_CreatingandEditing		192
#define IDS_CreatingandEditing		"Creating and Editing"
#define IDF_CreatingandEditing		"epsplinese4.html"

#define IDI_Tips		240
#define IDS_Tips		"Tips"
#define IDF_Tips		"epsplinese5.html"

#define IDI_Licenses		202000
#define IDS_Licenses		"Licenses"
#define IDF_Licenses		"epsplinech3.html"

#define IDI_GPL3_Section		304
#define IDS_GPL3_Section		"GNU General Public License"
#define IDF_GPL3_Section		"epsplinese6.html"

#define IDI_GPL2_Section		368
#define IDS_GPL2_Section		"GNU General Public License Version 2"
#define IDF_GPL2_Section		"epsplinese7.html"

#define IDI_FDL_Section		432
#define IDS_FDL_Section		"GNU Free Documentation License"
#define IDF_FDL_Section		"epsplinese8.html"

#define IDI_Index		2000000000
#define IDS_Index		"Index"
#define IDF_Index		"epsplineli2.html"


#endif /* _epspline_helpids_INCLUDED */

