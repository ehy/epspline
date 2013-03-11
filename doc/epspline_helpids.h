

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
#define IDS_SoftwareIntroSection		"The Software"
#define IDF_SoftwareIntroSection		"epsplinese1.html"

#define IDI_POVRayIntroSection		32
#define IDS_POVRayIntroSection		"Brief Introduction to POV-Ray"
#define IDF_POVRayIntroSection		"epsplinesu1.html"

#define IDI_EpsplineIntroSection		48
#define IDS_EpsplineIntroSection		"Introducing Epspline"
#define IDF_EpsplineIntroSection		"epsplinesu2.html"

#define IDI_AtTheTimeofThisWriting		64
#define IDS_AtTheTimeofThisWriting		"At The Time of This Writing"
#define IDF_AtTheTimeofThisWriting		"epsplinese2.html"

#define IDI_OntheHopeThattheProgramisUseful		402000
#define IDS_OntheHopeThattheProgramisUseful		"On the Hope That the Program is Useful"
#define IDF_OntheHopeThattheProgramisUseful		"epsplinesu3.html"

#define IDI_EpsplinePrismandLatheEditor		301000
#define IDS_EpsplinePrismandLatheEditor		"Epspline: Prism and Lathe Editor"
#define IDF_EpsplinePrismandLatheEditor		"epsplinech2.html"

#define IDI_Usage		303000
#define IDS_Usage		"Usage"
#define IDF_Usage		"epsplinese3.html"

#define IDI_Overview		403000
#define IDS_Overview		"Overview"
#define IDF_Overview		"epsplinesu4.html"

#define IDI_MouseandKey		404000
#define IDS_MouseandKey		"Mouse and Key"
#define IDF_MouseandKey		"epsplinesu5.html"

#define IDI_UsageTable		405000
#define IDS_UsageTable		"Usage Table"
#define IDF_UsageTable		"epsplinesu6.html"

#define IDI_Licenses		202000
#define IDS_Licenses		"Licenses"
#define IDF_Licenses		"epsplinech3.html"

#define IDI_GPL3_Section		80
#define IDS_GPL3_Section		"GNU General Public License"
#define IDF_GPL3_Section		"epsplinese4.html"

#define IDI_PREAMBLE		96
#define IDS_PREAMBLE		"PREAMBLE"
#define IDF_PREAMBLE		"epsplineli2.html"

#define IDI_TERMSANDCONDITIONS		112
#define IDS_TERMSANDCONDITIONS		"TERMS AND CONDITIONS"
#define IDF_TERMSANDCONDITIONS		"epsplineli3.html"

#define IDI_APPLYTHESETERMSTOYOURNEWPROGRAMS		128
#define IDS_APPLYTHESETERMSTOYOURNEWPROGRAMS		"APPLY THESE TERMS TO YOUR NEW PROGRAMS"
#define IDF_APPLYTHESETERMSTOYOURNEWPROGRAMS		"epsplineli4.html"

#define IDI_GPL2_Section		144
#define IDS_GPL2_Section		"GNU General Public License Version 2"
#define IDF_GPL2_Section		"epsplinese5.html"

#define IDI_PreambleGPLv2		160
#define IDS_PreambleGPLv2		"Preamble (GPL v.2)"
#define IDF_PreambleGPLv2		"epsplineli5.html"

#define IDI_TermsandConditionsForCopyingDistributionandModification		176
#define IDS_TermsandConditionsForCopyingDistributionandModification		"Terms and Conditions For Copying, Distribution and Modification"
#define IDF_TermsandConditionsForCopyingDistributionandModification		"epsplineli6.html"

#define IDI_HowtoApplyTheseTermstoYourNewPrograms		192
#define IDS_HowtoApplyTheseTermstoYourNewPrograms		"How to Apply These Terms to Your New Programs"
#define IDF_HowtoApplyTheseTermstoYourNewPrograms		"epsplineli7.html"

#define IDI_FDL_Section		208
#define IDS_FDL_Section		"GNU Free Documentation License"
#define IDF_FDL_Section		"epsplinese6.html"

#define IDI_Preamble		224
#define IDS_Preamble		"Preamble"
#define IDF_Preamble		"epsplineli5.html"

#define IDI_1APPLICABILITYANDDEFINITIONS		240
#define IDS_1APPLICABILITYANDDEFINITIONS		"1. APPLICABILITY AND DEFINITIONS"
#define IDF_1APPLICABILITYANDDEFINITIONS		"epsplineli9.html"

#define IDI_2VERBATIMCOPYING		256
#define IDS_2VERBATIMCOPYING		"2. VERBATIM COPYING"
#define IDF_2VERBATIMCOPYING		"epsplineli10.html"

#define IDI_3COPYINGINQUANTITY		272
#define IDS_3COPYINGINQUANTITY		"3. COPYING IN QUANTITY"
#define IDF_3COPYINGINQUANTITY		"epsplineli11.html"

#define IDI_4MODIFICATIONS		288
#define IDS_4MODIFICATIONS		"4. MODIFICATIONS"
#define IDF_4MODIFICATIONS		"epsplineli12.html"

#define IDI_5COMBININGDOCUMENTS		304
#define IDS_5COMBININGDOCUMENTS		"5. COMBINING DOCUMENTS"
#define IDF_5COMBININGDOCUMENTS		"epsplineli13.html"

#define IDI_6COLLECTIONSOFDOCUMENTS		320
#define IDS_6COLLECTIONSOFDOCUMENTS		"6. COLLECTIONS OF DOCUMENTS"
#define IDF_6COLLECTIONSOFDOCUMENTS		"epsplineli14.html"

#define IDI_7AGGREGATIONWITHINDEPENDENTWORKS		336
#define IDS_7AGGREGATIONWITHINDEPENDENTWORKS		"7. AGGREGATION WITH INDEPENDENT WORKS"
#define IDF_7AGGREGATIONWITHINDEPENDENTWORKS		"epsplineli15.html"

#define IDI_8TRANSLATION		352
#define IDS_8TRANSLATION		"8. TRANSLATION"
#define IDF_8TRANSLATION		"epsplineli16.html"

#define IDI_9TERMINATION		368
#define IDS_9TERMINATION		"9. TERMINATION"
#define IDF_9TERMINATION		"epsplineli17.html"

#define IDI_10FUTUREREVISIONSOFTHISLICENSE		384
#define IDS_10FUTUREREVISIONSOFTHISLICENSE		"10. FUTURE REVISIONS OF THIS LICENSE"
#define IDF_10FUTUREREVISIONSOFTHISLICENSE		"epsplineli18.html"

#define IDI_ADDENDUMHowtousethisLicenseforyourdocuments		400
#define IDS_ADDENDUMHowtousethisLicenseforyourdocuments		"ADDENDUM: How to use this License for your documents"
#define IDF_ADDENDUMHowtousethisLicenseforyourdocuments		"epsplineli19.html"

#define IDI_Index		2000000000
#define IDS_Index		"Index"
#define IDF_Index		"epsplineli20.html"


#endif /* _epspline_helpids_INCLUDED */

