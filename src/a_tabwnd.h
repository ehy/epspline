
// main wxWindows frame class

#ifndef _A_TABWND_H_
#define _A_TABWND_H_

#include "wx/wx.h"
#include "wx/notebook.h"
#include "a_ruler.h"
#include "a_canvas.h"

#ifdef __GNUG__
    #pragma interface
#endif

// fordecl
class A_Frame;

static const int TabWindowID = 1777;

// Main frame
class A_Tabwnd : public wxNotebook {
public:
	A_Tabwnd(A_Frame* parent, wxWindowID id
		, const wxPoint& pos = wxDefaultPosition
		, const wxSize& size = wxDefaultSize
		, long style = 0, const wxString& name = wxT("notebook"));
	~A_Tabwnd();

	void OnPageChanged(wxNotebookEvent& e);
	//void OnPageChanging(wxNotebookEvent& e);

	bool AddPage(wxNotebookPage* page
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

