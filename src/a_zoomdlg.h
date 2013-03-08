
// dialog with two spin contols, to get user scale values

#ifndef _A_ZOOMDLG_H_
#define _A_ZOOMDLG_H_

#ifdef __GNUG__
    #pragma interface
#endif

#include "a_spintextdlg.h"
#include "wx/window.h"
#include "wx/string.h"


// Main frame
class A_Zoomdlg : public A_Spintextdialog {
	static wxArrayString	as;
	static const wxArrayString& get_as();
public:
	A_Zoomdlg(wxWindow* parent, wxWindowID id);
	~A_Zoomdlg();

	int GetValueX() {
		return A_Spintextdialog::GetValue(0);
	}
	int GetValueY() {
		return A_Spintextdialog::GetValue(1);
	}
	void SetValueX(int value) {
		A_Spintextdialog::SetValue(0, value);
	}
	void SetValueY(int value) {
		A_Spintextdialog::SetValue(1, value);
	}
	void SetRangeX(int min, int max) {
		A_Spintextdialog::SetRange(0, min, max);
	}
	void SetRangeY(int min, int max) {
		A_Spintextdialog::SetRange(1, min, max);
	}
};

#endif  // _A_ZOOMDLG_H_
