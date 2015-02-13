

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
#define IDF_POVRayIntroSection		"epsplinese1.html#x4-40001.1.1"

#define IDI_EpsplineIntroSection		48
#define IDS_EpsplineIntroSection		"Introducing Epspline"
#define IDF_EpsplineIntroSection		"epsplinese1.html#x4-50001.1.2"

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
#define IDF_TheTitleBar		"epsplinese3.html#x11-90002.1.1"

#define IDI_TheMenuBar		112
#define IDS_TheMenuBar		"The Menu Bar"
#define IDF_TheMenuBar		"epsplinese3.html#x11-100002.1.2"

#define IDI_TheToolBar		128
#define IDS_TheToolBar		"The Tool Bar"
#define IDF_TheToolBar		"epsplinese3.html#x11-150002.1.3"

#define IDI_TheTabSelectionArea		144
#define IDS_TheTabSelectionArea		"The Tab Selection Area"
#define IDF_TheTabSelectionArea		"epsplinese3.html#x11-160002.1.4"

#define IDI_TheDrawingAreaandCanvas		160
#define IDS_TheDrawingAreaandCanvas		"The Drawing Area and Canvas"
#define IDF_TheDrawingAreaandCanvas		"epsplinese3.html#x11-170002.1.5"

#define IDI_TheStatusBar		176
#define IDS_TheStatusBar		"The Status Bar"
#define IDF_TheStatusBar		"epsplinese3.html#x11-180002.1.6"

#define IDI_CreatingandEditing		192
#define IDS_CreatingandEditing		"Creating and Editing"
#define IDF_CreatingandEditing		"epsplinese4.html"

#define IDI_CreatingNewObjects		208
#define IDS_CreatingNewObjects		"Creating New Objects"
#define IDF_CreatingNewObjects		"epsplinese4.html#x12-200002.2.1"

#define IDI_NewObjectSummary		240
#define IDS_NewObjectSummary		"New Object Summary"
#define IDF_NewObjectSummary		"epsplinese4.html#x12-220002.2.1"

#define IDI_ContinuedEditing		256
#define IDS_ContinuedEditing		"Continued Editing"
#define IDF_ContinuedEditing		"epsplinese5.html"

#define IDI_ObjectProperties		272
#define IDS_ObjectProperties		"Object Properties"
#define IDF_ObjectProperties		"epsplinese5.html#x13-240002.3.1"

#define IDI_EpsplineTransformsandCopies		288
#define IDS_EpsplineTransformsandCopies		"Epspline Transforms and Copies"
#define IDF_EpsplineTransformsandCopies		"epsplinese5.html#x13-250002.3.2"

#define IDI_AddingMovingandDeletingPoints		304
#define IDS_AddingMovingandDeletingPoints		"Adding, Moving, and Deleting Points"
#define IDF_AddingMovingandDeletingPoints		"epsplinese5.html#x13-260002.3.3"

#define IDI_DiscontinuousObjects		320
#define IDS_DiscontinuousObjects		"Discontinuous Objects"
#define IDF_DiscontinuousObjects		"epsplinese5.html#x13-270002.3.4"

#define IDI_GuideLines		336
#define IDS_GuideLines		"Guide Lines"
#define IDF_GuideLines		"epsplinese5.html#x13-280002.3.5"

#define IDI_UsefulTransforms		368
#define IDS_UsefulTransforms		"Useful Transforms"
#define IDF_UsefulTransforms		"epsplinese6.html"

#define IDI_ExampleofExportedSDL		384
#define IDS_ExampleofExportedSDL		"Example of Exported SDL"
#define IDF_ExampleofExportedSDL		"epsplinese6.html#x14-300002.4.1"

#define IDI_ExamplewithaLatheandPrism		400
#define IDS_ExamplewithaLatheandPrism		"Example with a Lathe and Prism"
#define IDF_ExamplewithaLatheandPrism		"epsplinese6.html#x14-310002.4.2"

#define IDI_Preferences		416
#define IDS_Preferences		"Preferences"
#define IDF_Preferences		"epsplinese7.html"

#define IDI_GlobalPreferences		432
#define IDS_GlobalPreferences		"Global Preferences"
#define IDF_GlobalPreferences		"epsplinese7.html#x15-330002.5.1"

#define IDI_POVRaySettings		448
#define IDS_POVRaySettings		"POV-Ray Settings"
#define IDF_POVRaySettings		"epsplinese7.html#x15-340002.5.2"

#define IDI_BackgroundImage		464
#define IDS_BackgroundImage		"Background Image"
#define IDF_BackgroundImage		"epsplinese8.html"

#define IDI_BackgroundImageDialogControls		480
#define IDS_BackgroundImageDialogControls		"Background Image Dialog Controls"
#define IDF_BackgroundImageDialogControls		"epsplinese8.html#x16-360002.6.1"

#define IDI_MouseKeyRef		496
#define IDS_MouseKeyRef		"Mouse and Key Reference"
#define IDF_MouseKeyRef		"epsplinese9.html"

#define IDI_MouseandKeyInputActions		512
#define IDS_MouseandKeyInputActions		"Mouse and Key Input Actions"
#define IDF_MouseandKeyInputActions		"epsplinese9.html#x17-380002.7.1"

#define IDI_Licenses		202000
#define IDS_Licenses		"Licenses"
#define IDF_Licenses		"epsplinech3.html"

#define IDI_GPL3_Section		544
#define IDS_GPL3_Section		"GNU General Public License"
#define IDF_GPL3_Section		"epsplinese10.html"

#define IDI_Preamble_GPL_3		560
#define IDS_Preamble_GPL_3		"PREAMBLE"
#define IDF_Preamble_GPL_3		"epsplinese10.html#x19-410003.1"

#define IDI_TERMSANDCONDITIONS		576
#define IDS_TERMSANDCONDITIONS		"TERMS AND CONDITIONS"
#define IDF_TERMSANDCONDITIONS		"epsplinese10.html#x19-420003.1"

#define IDI_APPLYTHESETERMSTOYOURNEWPROGRAMS		592
#define IDS_APPLYTHESETERMSTOYOURNEWPROGRAMS		"APPLY THESE TERMS TO YOUR NEW PROGRAMS"
#define IDF_APPLYTHESETERMSTOYOURNEWPROGRAMS		"epsplinese10.html#x19-4300017"

#define IDI_GPL2_Section		608
#define IDS_GPL2_Section		"GNU General Public License Version 2"
#define IDF_GPL2_Section		"epsplinese11.html"

#define IDI_Preamble_GPL_2		624
#define IDS_Preamble_GPL_2		"Preamble GPL 2"
#define IDF_Preamble_GPL_2		"epsplinese11.html#x20-450003.2"

#define IDI_TermsandConditionsForCopyingDistributionandModification		640
#define IDS_TermsandConditionsForCopyingDistributionandModification		"Terms and Conditions For Copying, Distribution and Modification"
#define IDF_TermsandConditionsForCopyingDistributionandModification		"epsplinese11.html#x20-460003.2"

#define IDI_HowtoApplyTheseTermstoYourNewPrograms		656
#define IDS_HowtoApplyTheseTermstoYourNewPrograms		"How to Apply These Terms to Your New Programs"
#define IDF_HowtoApplyTheseTermstoYourNewPrograms		"epsplinese11.html#x20-470003.2"

#define IDI_FDL_Section		672
#define IDS_FDL_Section		"GNU Free Documentation License"
#define IDF_FDL_Section		"epsplinese12.html"

#define IDI_Preamble		688
#define IDS_Preamble		"Preamble"
#define IDF_Preamble		"epsplinese12.html#x21-490003.3"

#define IDI_1APPLICABILITYANDDEFINITIONS		704
#define IDS_1APPLICABILITYANDDEFINITIONS		"1. APPLICABILITY AND DEFINITIONS"
#define IDF_1APPLICABILITYANDDEFINITIONS		"epsplinese12.html#x21-500003.3"

#define IDI_2VERBATIMCOPYING		720
#define IDS_2VERBATIMCOPYING		"2. VERBATIM COPYING"
#define IDF_2VERBATIMCOPYING		"epsplinese12.html#x21-510003.3"

#define IDI_3COPYINGINQUANTITY		736
#define IDS_3COPYINGINQUANTITY		"3. COPYING IN QUANTITY"
#define IDF_3COPYINGINQUANTITY		"epsplinese12.html#x21-520003.3"

#define IDI_4MODIFICATIONS		752
#define IDS_4MODIFICATIONS		"4. MODIFICATIONS"
#define IDF_4MODIFICATIONS		"epsplinese12.html#x21-530003.3"

#define IDI_5COMBININGDOCUMENTS		768
#define IDS_5COMBININGDOCUMENTS		"5. COMBINING DOCUMENTS"
#define IDF_5COMBININGDOCUMENTS		"epsplinese12.html#x21-540003.3"

#define IDI_6COLLECTIONSOFDOCUMENTS		784
#define IDS_6COLLECTIONSOFDOCUMENTS		"6. COLLECTIONS OF DOCUMENTS"
#define IDF_6COLLECTIONSOFDOCUMENTS		"epsplinese12.html#x21-550003.3"

#define IDI_7AGGREGATIONWITHINDEPENDENTWORKS		800
#define IDS_7AGGREGATIONWITHINDEPENDENTWORKS		"7. AGGREGATION WITH INDEPENDENT WORKS"
#define IDF_7AGGREGATIONWITHINDEPENDENTWORKS		"epsplinese12.html#x21-560003.3"

#define IDI_8TRANSLATION		816
#define IDS_8TRANSLATION		"8. TRANSLATION"
#define IDF_8TRANSLATION		"epsplinese12.html#x21-570003.3"

#define IDI_9TERMINATION		832
#define IDS_9TERMINATION		"9. TERMINATION"
#define IDF_9TERMINATION		"epsplinese12.html#x21-580003.3"

#define IDI_10FUTUREREVISIONSOFTHISLICENSE		848
#define IDS_10FUTUREREVISIONSOFTHISLICENSE		"10. FUTURE REVISIONS OF THIS LICENSE"
#define IDF_10FUTUREREVISIONSOFTHISLICENSE		"epsplinese12.html#x21-590003.3"

#define IDI_11RELICENSING		864
#define IDS_11RELICENSING		"11. RELICENSING"
#define IDF_11RELICENSING		"epsplinese12.html#x21-600003.3"

#define IDI_ADDENDUMHowtousethisLicenseforyourdocuments		880
#define IDS_ADDENDUMHowtousethisLicenseforyourdocuments		"ADDENDUM: How to use this License for your documents"
#define IDF_ADDENDUMHowtousethisLicenseforyourdocuments		"epsplinese12.html#x21-610003.3"

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

