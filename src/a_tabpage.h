
// main wxWindows frame class

#ifndef _A_TABPAGE_H_
#define _A_TABPAGE_H_

#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/notebook.h"
#include "a_ruler.h"
#include "a_canvas.h"

#ifdef __GNUG__
    #pragma interface
#endif

// For the working areas of the tabs of the main interface

// NOTE using wxWindow as parent (wxNotebookPage is typedef of wxWindow)
// does *not* automatically size the page; wxPanel works
//typedef wxNotebookPage tabpage_parent_class;
typedef wxPanel tabpage_parent_class;

class A_Tabpage : public tabpage_parent_class {
public:
	A_Tabpage(wxWindow* parent, wxWindowID id
		, A_Canvas* canvas, A_Ruler* hr, A_Ruler* vr
		, const wxPoint& pos = wxDefaultPosition
		, const wxSize& size = wxDefaultSize
		, long style = 0, const wxString& name = wxT("tabpage"));
	~A_Tabpage();

	void SetCanvas(A_Canvas* can) { canvas = can; }
	void SetHRule(A_Ruler* ph) { hrule = ph; }
	void SetVRule(A_Ruler* pv) { vrule = pv; }
	A_Canvas* GetCanvas() { return canvas; }
	A_Ruler* GetHRule() { return hrule; }
	A_Ruler* GetVRule() { return vrule; }

protected:
	A_Canvas* canvas;
	A_Ruler*  hrule;
	A_Ruler*  vrule;
private:
	//DECLARE_EVENT_TABLE()
};

#endif  // _A_TABPAGE_H_

