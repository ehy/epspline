

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

#define IDI_POVRayIntroSection		32
#define IDS_POVRayIntroSection		"Brief Introduction to POV-Ray"
#define IDF_POVRayIntroSection		"epsplinesu1.html"

#define IDI_EpsplineIntroSection		48
#define IDS_EpsplineIntroSection		"Introducing Epspline"
#define IDF_EpsplineIntroSection		"epsplinesu2.html"

#define IDI_RequirementsandStatus		64
#define IDS_RequirementsandStatus		"Requirements and Status"
#define IDF_RequirementsandStatus		"epsplinese2.html"

#define IDI_UsingEpspline		301000
#define IDS_UsingEpspline		"Using Epspline"
#define IDF_UsingEpspline		"epsplinech2.html"

#define IDI_TheMainWindowInterface		80
#define IDS_TheMainWindowInterface		"The Main Window Interface"
#define IDF_TheMainWindowInterface		"epsplinese3.html"

#define IDI_TheTitleBar		96
#define IDS_TheTitleBar		"The Title Bar"
#define IDF_TheTitleBar		"epsplinesu3.html"

#define IDI_TheMenuBar		112
#define IDS_TheMenuBar		"The Menu Bar"
#define IDF_TheMenuBar		"epsplinesu4.html"

#define IDI_TheToolBar		128
#define IDS_TheToolBar		"The Tool Bar"
#define IDF_TheToolBar		"epsplinesu5.html"

#define IDI_TheTabSelectionArea		144
#define IDS_TheTabSelectionArea		"The Tab Selection Area"
#define IDF_TheTabSelectionArea		"epsplinesu6.html"

#define IDI_TheDrawingAreaandCanvas		160
#define IDS_TheDrawingAreaandCanvas		"The Drawing Area and Canvas"
#define IDF_TheDrawingAreaandCanvas		"epsplinesu7.html"

#define IDI_TheStatusBar		176
#define IDS_TheStatusBar		"The Status Bar"
#define IDF_TheStatusBar		"epsplinesu8.html"

#define IDI_Preferences		192
#define IDS_Preferences		"Preferences"
#define IDF_Preferences		"epsplinese4.html"

#define IDI_GlobalPreferences		208
#define IDS_GlobalPreferences		"Global Preferences"
#define IDF_GlobalPreferences		"epsplinesu9.html"

#define IDI_POVRaySettings		224
#define IDS_POVRaySettings		"POV-Ray Settings"
#define IDF_POVRaySettings		"epsplinesu10.html"

#define IDI_CreatingandEditing		240
#define IDS_CreatingandEditing		"Creating and Editing"
#define IDF_CreatingandEditing		"epsplinese5.html"

#define IDI_CreatingNewObjects		256
#define IDS_CreatingNewObjects		"Creating New Objects"
#define IDF_CreatingNewObjects		"epsplinesu11.html"

#define IDI_ContinuedEditing		304
#define IDS_ContinuedEditing		"Continued Editing"
#define IDF_ContinuedEditing		"epsplinesu12.html"

#define IDI_UsefulTransforms		416
#define IDS_UsefulTransforms		"Useful Transforms"
#define IDF_UsefulTransforms		"epsplinese6.html"

#define IDI_ExampleofExportedSDL		432
#define IDS_ExampleofExportedSDL		"Example of Exported SDL"
#define IDF_ExampleofExportedSDL		"epsplinesu13.html"

#define IDI_ExamplewithaLatheandPrism		448
#define IDS_ExamplewithaLatheandPrism		"Example with a Lathe and Prism"
#define IDF_ExamplewithaLatheandPrism		"epsplinesu14.html"

#define IDI_MouseKeyRef		464
#define IDS_MouseKeyRef		"Mouse and Key Reference"
#define IDF_MouseKeyRef		"epsplinese7.html"

#define IDI_MouseandKeyInputActions		480
#define IDS_MouseandKeyInputActions		"Mouse and Key Input Actions"
#define IDF_MouseandKeyInputActions		"epsplinesu15.html"

#define IDI_Licenses		202000
#define IDS_Licenses		"Licenses"
#define IDF_Licenses		"epsplinech3.html"

#define IDI_GPL3_Section		528
#define IDS_GPL3_Section		"GNU General Public License"
#define IDF_GPL3_Section		"epsplinese8.html"

#define IDI_PREAMBLE		544
#define IDS_PREAMBLE		"PREAMBLE"
#define IDF_PREAMBLE		"epsplineli2.html"

#define IDI_TERMSANDCONDITIONS		560
#define IDS_TERMSANDCONDITIONS		"TERMS AND CONDITIONS"
#define IDF_TERMSANDCONDITIONS		"epsplineli3.html"

#define IDI_APPLYTHESETERMSTOYOURNEWPROGRAMS		576
#define IDS_APPLYTHESETERMSTOYOURNEWPROGRAMS		"APPLY THESE TERMS TO YOUR NEW PROGRAMS"
#define IDF_APPLYTHESETERMSTOYOURNEWPROGRAMS		"epsplineli4.html"

#define IDI_GPL2_Section		592
#define IDS_GPL2_Section		"GNU General Public License Version 2"
#define IDF_GPL2_Section		"epsplinese9.html"

#define IDI_Preamble		608
#define IDS_Preamble		"Preamble"
#define IDF_Preamble		"epsplineli5.html"

#define IDI_TermsandConditionsForCopyingDistributionandModification		624
#define IDS_TermsandConditionsForCopyingDistributionandModification		"Terms and Conditions For Copying, Distribution and Modification"
#define IDF_TermsandConditionsForCopyingDistributionandModification		"epsplineli6.html"

#define IDI_HowtoApplyTheseTermstoYourNewPrograms		640
#define IDS_HowtoApplyTheseTermstoYourNewPrograms		"How to Apply These Terms to Your New Programs"
#define IDF_HowtoApplyTheseTermstoYourNewPrograms		"epsplineli7.html"

#define IDI_FDL_Section		656
#define IDS_FDL_Section		"GNU Free Documentation License"
#define IDF_FDL_Section		"epsplinese10.html"

#define IDI_Preamble		672
#define IDS_Preamble		"Preamble"
#define IDF_Preamble		"epsplineli5.html"

#define IDI_1APPLICABILITYANDDEFINITIONS		688
#define IDS_1APPLICABILITYANDDEFINITIONS		"1. APPLICABILITY AND DEFINITIONS"
#define IDF_1APPLICABILITYANDDEFINITIONS		"epsplineli9.html"

#define IDI_2VERBATIMCOPYING		704
#define IDS_2VERBATIMCOPYING		"2. VERBATIM COPYING"
#define IDF_2VERBATIMCOPYING		"epsplineli10.html"

#define IDI_3COPYINGINQUANTITY		720
#define IDS_3COPYINGINQUANTITY		"3. COPYING IN QUANTITY"
#define IDF_3COPYINGINQUANTITY		"epsplineli11.html"

#define IDI_4MODIFICATIONS		736
#define IDS_4MODIFICATIONS		"4. MODIFICATIONS"
#define IDF_4MODIFICATIONS		"epsplineli12.html"

#define IDI_5COMBININGDOCUMENTS		752
#define IDS_5COMBININGDOCUMENTS		"5. COMBINING DOCUMENTS"
#define IDF_5COMBININGDOCUMENTS		"epsplineli13.html"

#define IDI_6COLLECTIONSOFDOCUMENTS		768
#define IDS_6COLLECTIONSOFDOCUMENTS		"6. COLLECTIONS OF DOCUMENTS"
#define IDF_6COLLECTIONSOFDOCUMENTS		"epsplineli14.html"

#define IDI_7AGGREGATIONWITHINDEPENDENTWORKS		784
#define IDS_7AGGREGATIONWITHINDEPENDENTWORKS		"7. AGGREGATION WITH INDEPENDENT WORKS"
#define IDF_7AGGREGATIONWITHINDEPENDENTWORKS		"epsplineli15.html"

#define IDI_8TRANSLATION		800
#define IDS_8TRANSLATION		"8. TRANSLATION"
#define IDF_8TRANSLATION		"epsplineli16.html"

#define IDI_9TERMINATION		816
#define IDS_9TERMINATION		"9. TERMINATION"
#define IDF_9TERMINATION		"epsplineli17.html"

#define IDI_10FUTUREREVISIONSOFTHISLICENSE		832
#define IDS_10FUTUREREVISIONSOFTHISLICENSE		"10. FUTURE REVISIONS OF THIS LICENSE"
#define IDF_10FUTUREREVISIONSOFTHISLICENSE		"epsplineli18.html"

#define IDI_11RELICENSING		848
#define IDS_11RELICENSING		"11. RELICENSING"
#define IDF_11RELICENSING		"epsplineli19.html"

#define IDI_ADDENDUMHowtousethisLicenseforyourdocuments		864
#define IDS_ADDENDUMHowtousethisLicenseforyourdocuments		"ADDENDUM: How to use this License for your documents"
#define IDF_ADDENDUMHowtousethisLicenseforyourdocuments		"epsplineli20.html"

#define IDI_ListFigures		3000000000
#define IDS_ListFigures		"List of Figures"
#define IDF_ListFigures		"epsplineli21.html"

#define IDI_ListTables		4000000000
#define IDS_ListTables		"List of Tables"
#define IDF_ListTables		"epsplineli22.html"

#define IDI_Index		2000000000
#define IDS_Index		"Index"
#define IDF_Index		"epsplineli23.html"


#endif /* _epspline_helpids_INCLUDED */

