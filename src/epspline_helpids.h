

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

#define IDI_UsefulTransforms		240
#define IDS_UsefulTransforms		"Useful Transforms"
#define IDF_UsefulTransforms		"epsplinese5.html"

#define IDI_Tips		288
#define IDS_Tips		"Tips"
#define IDF_Tips		"epsplinese6.html"

#define IDI_Licenses		202000
#define IDS_Licenses		"Licenses"
#define IDF_Licenses		"epsplinech3.html"

#define IDI_GPL3_Section		304
#define IDS_GPL3_Section		"GNU General Public License"
#define IDF_GPL3_Section		"epsplinese7.html"

#define IDI_GPL2_Section		368
#define IDS_GPL2_Section		"GNU General Public License Version 2"
#define IDF_GPL2_Section		"epsplinese8.html"

#define IDI_FDL_Section		432
#define IDS_FDL_Section		"GNU Free Documentation License"
#define IDF_FDL_Section		"epsplinese9.html"

#define IDI_ListFigures		3000000000
#define IDS_ListFigures		"List of Figures"
#define IDF_ListFigures		"epsplineli2.html"

#define IDI_ListTables		4000000000
#define IDS_ListTables		"List of Tables"
#define IDF_ListTables		"epsplineli3.html"

#define IDI_Index		2000000000
#define IDS_Index		"Index"
#define IDF_Index		"epsplineli4.html"


#endif /* _epspline_helpids_INCLUDED */

