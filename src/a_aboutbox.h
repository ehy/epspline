
// The Help->About dialog box

#ifndef _A_ABOUTBOX_H_
#define _A_ABOUTBOX_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/dialog.h"
#include "wx/notebook.h"
#endif // WX_PRECOMP

#include "cfg.h"
#include "stdcc.h"

#ifdef __GNUG__
    #pragma interface
#endif

class A_Aboutbox : public wxDialog {
public:
	A_Aboutbox(wxWindow* parent, int id, const wxString& title);
	~A_Aboutbox();
};

#endif  // _A_ABOUTBOX_H_
