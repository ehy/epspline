// A Frame for GUI application.

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
#include "a_tabpage.h"

// A_Tabpage
//

#if 0
BEGIN_EVENT_TABLE(A_Tabpage, wxNotebook)
	EVT_NOTEBOOK_PAGE_CHANGED  (TabWindowID, A_Tabpage::OnPageChanged)
	EVT_NOTEBOOK_PAGE_CHANGING (TabWindowID, A_Tabpage::OnPageChanging)
END_EVENT_TABLE()
#endif


A_Tabpage::A_Tabpage(wxWindow* parent, wxWindowID id
	, A_Canvas* can, A_Ruler* hr, A_Ruler* vr
	, const wxPoint& pos, const wxSize& size
	, long style, const wxString& name)
       : tabpage_parent_class(parent, id, pos, size, style, name)
       , canvas(can), hrule(hr), vrule(vr)
{
}

A_Tabpage::~A_Tabpage()
{
}
