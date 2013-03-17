
// main wxWindows frame class

#ifndef _A_TABWND_H_
#define _A_TABWND_H_

#include "wx/wx.h"

// no aui in wx 2.6
#if wxCHECK_VERSION(2, 8, 0)
#	ifndef NO_AUI_NOTEBOOK
#		define USE_AUI_NOTEBOOK 1
#	endif
#endif

#if USE_AUI_NOTEBOOK
#	include "wx/aui/auibook.h"
	typedef	wxAuiNotebook	NoteBook_type;
#	define	TABWND_EVENT_T	wxAuiNotebookEvent
#	define	TABWND_PAGE_T	wxWindow
#		if wxCHECK_VERSION(2, 9, 0)
#	define	TABWND_CMD_EVENT_T	wxAuiNotebookEvent
#		else
#	define	TABWND_CMD_EVENT_T	wxCommandEvent
#		endif
	const int wxAuiBaseTabCtrlId = 5380;
#else
#	include "wx/notebook.h"
	typedef	wxNotebook	NoteBook_type;
#	define	TABWND_EVENT_T	wxNotebookEvent
#	define	TABWND_PAGE_T	wxNotebookPage
#endif

#include "a_ruler.h"
#include "a_canvas.h"

#ifdef __GNUG__
    #pragma interface
#endif

// fordecl
class A_Frame;

const int TabWindowID = 1777;

// Main frame
class A_Tabwnd : public NoteBook_type {
public:
	A_Tabwnd(A_Frame* parent, wxWindowID id
		, const wxPoint& pos = wxDefaultPosition
		, const wxSize& size = wxDefaultSize
		, long style = 0);
	~A_Tabwnd();

	void OnPageChanged(TABWND_EVENT_T& e);
	//void OnPageChanging(TABWND_EVENT_T& e);
#	if USE_AUI_NOTEBOOK
	void OnButton(TABWND_CMD_EVENT_T& e);
#	endif

	bool AddPage(TABWND_PAGE_T* page
		, const wxString& text
		, bool select = FALSE
		, int imageId = -1);

	void SetCurPageText(const wxString& text) {
		SetPageText(GetSelection(), text);
	}

	void UpdateCurPageText();
protected:
	A_Frame* owner;
private:
	DECLARE_EVENT_TABLE()
};

#endif  // _A_TABWND_H_

