// dialog with two spin contols, to get user scale values

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __GNUG__
    #pragma implementation
#endif
#include "a_zoomdlg.h"

A_Zoomdlg::A_Zoomdlg(wxWindow* parent, wxWindowID id)
	: A_Spintextdialog(parent, id, _("Set Scale"), 2
	, get_as(), _("Set scale (zoom) as percent:"))
{

}

A_Zoomdlg::~A_Zoomdlg()
{

}

wxArrayString	A_Zoomdlg::as;

const wxArrayString&
A_Zoomdlg::get_as()
{
	static int init;
	
	if ( !init ) {
		as.Add(_("Scale X: "));
		as.Add(_("Scale Y: "));
		init = 1;
	}
	
	return as;
}
