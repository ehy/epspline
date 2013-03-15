// A Frame for GUI application.

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all _"standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __GNUG__
    #pragma implementation
#endif
#include "a_tabwnd.h"
#include "a_frame.h"

// A_Tabwnd
//

#if USE_AUI_NOTEBOOK
BEGIN_EVENT_TABLE(A_Tabwnd, NoteBook_type)
	EVT_AUINOTEBOOK_PAGE_CHANGED  (TabWindowID, A_Tabwnd::OnPageChanged)
	//EVT_AUINOTEBOOK_PAGE_CHANGING (TabWindowID, A_Tabwnd::OnPageChanging)
END_EVENT_TABLE()
#else
BEGIN_EVENT_TABLE(A_Tabwnd, NoteBook_type)
	EVT_NOTEBOOK_PAGE_CHANGED  (TabWindowID, A_Tabwnd::OnPageChanged)
	//EVT_NOTEBOOK_PAGE_CHANGING (TabWindowID, A_Tabwnd::OnPageChanging)
END_EVENT_TABLE()
#endif

A_Tabwnd::A_Tabwnd(A_Frame* parent, wxWindowID id
	, const wxPoint& pos, const wxSize& size, long style)
       : NoteBook_type(parent, id, pos, size, style
#if USE_AUI_NOTEBOOK
       )
#else
       , wxT("main_notebook"))
#endif
       , owner(parent)
{
}

A_Tabwnd::~A_Tabwnd()
{
}

bool
A_Tabwnd::AddPage(TABWND_PAGE_T* page, const wxString& text
		, bool select,
#if USE_AUI_NOTEBOOK
		int /*imageId*/)
#else
		int imageId)
#endif
{
	int s = GetSelection();
	if ( s >= 0 ) {
		#if defined(__SC__) && !defined(_CPPRTTI)
		A_Tabpage* t = (A_Tabpage*)GetPage(s);
		#else
		A_Tabpage* t = dynamic_cast<A_Tabpage*>(GetPage(s));
		#endif
		if ( t != 0 )
			t->GetCanvas()->SetActive(false);
	}

#if USE_AUI_NOTEBOOK
	return wxAuiNotebook::AddPage(page, text, select);
#else
	return wxNotebook::AddPage(page, text, select, imageId);
#endif
}

void
A_Tabwnd::UpdateCurPageText()
{
	if ( GetPageCount() < 1 )
		return;

	#if defined(__SC__) && !defined(_CPPRTTI)
	A_Tabpage* t = (A_Tabpage*)GetPage(GetSelection());
	#else
	A_Tabpage* t = dynamic_cast<A_Tabpage*>(GetPage(GetSelection()));
	#endif
	if ( t == 0 ) {
		return;
	}
	wxString n(t->GetCanvas()->GetCurFileName());
	if ( n == wxT("") )
		n = wxT("[--]");
	SetCurPageText(n);
	owner->SetTitlePrefix(n);
	t->GetCanvas()->SetActive(true);
	t->GetCanvas()->UpdateStatusBar();

}

void
A_Tabwnd::OnPageChanged(TABWND_EVENT_T& e)
{
	A_Tabpage* t = 0;
	int nold = e.GetOldSelection(), nnew = e.GetSelection();

	if ( nold >= 0 ) {
		#if defined(__SC__) && !defined(_CPPRTTI)
		t = (A_Tabpage*)GetPage(nold);
		#else
		t = dynamic_cast<A_Tabpage*>(GetPage(nold));
		#endif
		if ( t != 0 ) {
			t->GetCanvas()->SetActive(false);
		}
	}
	// test for old as well as new: wx 2.2, MSW, Symantec C++ 7.6
	// raises access violation at initial GetPage(nnew = 0) -- at which
	// time nold is -1.  Must be hitting an old bug from 2.2.
	// Add __WXMSW__ conditional as nec.
	if ( nold >= 0 && nnew >= 0 ) {
		#if defined(__SC__) && !defined(_CPPRTTI)
		t = (A_Tabpage*)GetPage(nnew);
		#else
		t = dynamic_cast<A_Tabpage*>(GetPage(nnew));
		#endif
		if ( t == 0 ) {
			e.Skip();
			return;
		}
		A_Canvas* canvas = t->GetCanvas();
		if ( canvas ) {
			canvas->SetActive(true);
			wxString n(canvas->GetCurFileName());
			if ( n == wxT("") )
				n = wxT("[--]");
			SetPageText(nnew, n);
			owner->SetTitlePrefix(n);
			canvas->IdleUpdate();
			canvas->UpdateStatusBar();
		}
	}
	e.Skip();
}

#if 0
void
A_Tabwnd::OnPageChanging(TABWND_EVENT_T& e)
{
	e.Skip();
}
#endif
