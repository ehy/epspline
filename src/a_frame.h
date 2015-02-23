/*
 * Copyright (c) 2013 Ed Hynan
 *
 * This file is part of Epspline
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

// main wxWindows frame class

#ifndef _A_FRAME_H_
#define _A_FRAME_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/dnd.h>
#endif // WX_PRECOMP

#include "epspline.h"
#include "a_canvas.h"
#include "a_tabwnd.h"
#include "a_tabpage.h"
// help topic IDs
#include "epspline_helpids.h"

#ifdef __GNUG__
    #pragma interface
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the toolbar and the menu commands
// Why? Because this enum existed before one of:
//		- the wx* IDs existed (this was started w/ wx 2.1)
//		- _I knew_ the wx* IDs existed (most likely)
//		- I understood I should use them (quite possible)
// assignement of wx* IDs is new; mostly to avail
// the code of stock strings and icons where possible
// (GTK2).
enum menu_and_toolbar_IDs {

	// auxiliary items
	menu_tool_separator = wxID_SEPARATOR,

	// lowest ID used in wx stock menu items
	menu_tool_first = wxID_LOWEST,

// arrange this enum so that IDs that can be assigned
// wx stock constants appear first, and put items not
// covered by wx stock IDs after; this means sections-
// per-menu will be repeated as necessary

// stock items:
// File
	SaveCurves        = wxID_SAVE,
	SaveAsCurves      = wxID_SAVEAS,
	NewTab            = wxID_NEW,
	NewTabOpenCurves  = wxID_OPEN,
	CloseCurves       = wxID_CLOSE,
	Xit_Quit          = wxID_EXIT,
// Edit
	EdUndo            = wxID_UNDO,
	EdRedo            = wxID_REDO,
	EdCopy            = wxID_COPY,
	EdCut             = wxID_CUT,
	EdDel             = wxID_DELETE,
	EdPaste           = wxID_PASTE,
	EdMoveDown        = wxID_DOWN,
	EdMoveUp          = wxID_UP,
// Tools
	ZoomStd           = wxID_ZOOM_100,
	ZoomIn            = wxID_ZOOM_IN,
	ZoomOut           = wxID_ZOOM_OUT,
// Help
	HelpHelp          = wxID_HELP,
	HelpDemo          = wxID_APPLY,
	HelpAbout         = wxID_ABOUT,

// mark end of stock IDs
	// highest ID used in wx stock menu items
	menu_tool_stock_end = wxID_HIGHEST,
// mark start of sequentially enumerated
	menu_tool_seq_start,

// non-stock items:
// File
	OpenCurves = menu_tool_seq_start,
	CloseTab,
	ExportCurves,
	ExportAsCurves,
// Edit
	EdCopyAll,  // TODO
	EdCopyGlobal,
	EdCutGlobal,
	EdCopyAllGlobal,  // TODO
	EdPasteGlobal,
	EdGlobalPreferences,
// Tools
	SetBGImage,
	RmBGImage,
	HideBGImage,
	SetUserScale,
	CycleUserScale,
	Toggle_AA_Lines,
	SaveClientImage,
// Help
	HelpHelpTableOfContents,

	menu_tool_last = HelpHelpTableOfContents
};

// TODO
#if 0
enum {
	MapMode_Text = menu_tool_last + 1,
	MapMode_Lometric,
	MapMode_Twips,
	MapMode_Points,
	MapMode_Metric,

	UserScale_StretchHoriz,
	UserScale_ShrinkHoriz,
	UserScale_StretchVertic,
	UserScale_ShrinkVertic,
	UserScale_Restore,

	AxisMirror_Horiz,
	AxisMirror_Vertic,

	LogicalOrigin_MoveDown,
	LogicalOrigin_MoveUp,
	LogicalOrigin_MoveLeft,
	LogicalOrigin_MoveRight,

	Colour_TextForeground,
	Colour_TextBackground,
	Colour_Background,
	Colour_BackgroundMode,

	MenuOption_Last = Colour_BackgroundMode
};
#endif


// Main window; and, file drop target
class A_Frame : public wxFrame {
public:
	A_Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
	virtual ~A_Frame();

	bool QueryDirtyData();
	bool PreOnQuit(bool force);
	void OnQuit(wxCloseEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnOption(wxCommandEvent& event);

	// this should provide set names of all dialog windows that
	// are given this as parent -- a hand maintained list of app's
	// dialogs, will not include message dialogs or pickers etc.
	void GetDialogNames(wxArrayString& out);

	// whether to set canvas to do anit-aliasing
	bool GetAADraw() const {
		return aadraw;
	}

	// for shutdown; close shown dialogs
	void CloseDialogs();

	void SetTitlePrefix(const wxString& pre);
	bool SelectColour(wxColour& c);
	void PrepareDC(wxDC& dc);

	// Menu/Toolbar stuff:
	// Open files in new tabs and return count successfully opened.
	// used internally, but public as it may be used externally.
	unsigned Open_FileArray(const wxArrayString& af);
	// Close a tab iff a check with its canvas allows it.
	bool CloseTabIffCanvasAllows(int tabnum);

	// ErrorBox is obvious, but ``titletail'' should be short,
	// As it is appended to app title string.
	void ErrorBox(const wxString& msg, const wxString& titletail =
			 _("Error")) const;

	// call on prefs update; will refresh
	void PreferenceChanged();

	// Menu/Toolbar stuff:
	// {en,dis}able helpers
	void enableExportCurves(bool b) {menuFile->Enable(ExportCurves, b);}
	void enableExportAsCurves(bool b)
		{menuFile->Enable(ExportAsCurves, b);}
	void enableSaveCurves(bool b) {
		menuFile->Enable(SaveCurves, b);
		GetToolBar()->EnableTool(SaveCurves, b);
	}
	void enableSaveAsCurves(bool b) {menuFile->Enable(SaveAsCurves, b);}
	void enableOpenCurves(bool b) {
		menuFile->Enable(OpenCurves, b);
		GetToolBar()->EnableTool(OpenCurves, b);
	}
	void enableNewTab(bool b) {menuFile->Enable(NewTab, b);}
	void enableNewTabOpenCurves(bool b)
		{menuFile->Enable(NewTabOpenCurves, b);}
	void enableCloseCurves(bool b) {menuFile->Enable(CloseCurves, b);}
	void enableCloseTab(bool b) {menuFile->Enable(CloseTab, b);}
	void enableEdUndo(bool b) {
		menuEdit->Enable(EdUndo, b);
		GetToolBar()->EnableTool(EdUndo, b);
	}
	void enableEdRedo(bool b) {
		menuEdit->Enable(EdRedo, b);
		GetToolBar()->EnableTool(EdRedo, b);
	}
	void enableEdMoveDown(bool b) {menuEdit->Enable(EdMoveDown, b);}
	void enableEdMoveUp(bool b) {menuEdit->Enable(EdMoveUp, b);}
	void enableEdCopy(bool b) {
		menuEdit->Enable(EdCopy, b);
		GetToolBar()->EnableTool(EdCopy, b);
	}
	void enableEdCopyAll(bool b) {
		menuEdit->Enable(EdCopyAll, b);
	}
	void enableEdCut(bool b) {
		menuEdit->Enable(EdCut, b);
		GetToolBar()->EnableTool(EdCut, b);
	}
	void enableEdDel(bool b) {
		menuEdit->Enable(EdDel, b);
	}
	void enableEdPaste(bool b) {
		menuEdit->Enable(EdPaste, b);
		GetToolBar()->EnableTool(EdPaste, b);
	}
	void enableEdCopyGlobal(bool b) {
		menuEdit->Enable(EdCopyGlobal, b);
		GetToolBar()->EnableTool(EdCopyGlobal, b);
	}
	void enableEdCopyAllGlobal(bool b) {
		menuEdit->Enable(EdCopyAllGlobal, b);
	}
	void enableEdCutGlobal(bool b) {
		menuEdit->Enable(EdCutGlobal, b);
		GetToolBar()->EnableTool(EdCutGlobal, b);
	}
	void enableEdPasteGlobal(bool b) {
		menuEdit->Enable(EdPasteGlobal, b);
		GetToolBar()->EnableTool(EdPasteGlobal, b);
	}
	void enableSetBGImg(bool b) {
		MenuOpts->Enable(SetBGImage, b);
	}
	void enableRmBGImg(bool b) {
		MenuOpts->Enable(RmBGImage, b);
	}
	void enableHideBGImg(bool b, bool chk = false) {
		MenuOpts->Check(HideBGImage, chk);
		MenuOpts->Enable(HideBGImage, b);
	}
	void enableViewScale(bool b) {
		MenuOpts->Enable(SetUserScale, b);
	}
	void enableViewScaleCycle(bool b) {
		MenuOpts->Enable(CycleUserScale, b);
		GetToolBar()->EnableTool(CycleUserScale, b);
	}
	void enableSaveClientImage(bool b) {
		MenuOpts->Enable(SaveClientImage, b);
		//GetToolBar()->EnableTool(SaveClientImage, b);
	}
	void enableHelpHelp(bool b) {
		menuHelp->Enable(HelpHelp, b);
		GetToolBar()->EnableTool(HelpHelp, b);
	}
	void enableHelpDemo(bool b) {
		menuHelp->Enable(HelpDemo, b);
		GetToolBar()->EnableTool(HelpDemo, b);
	}
	void enableHelpAbout(bool b) {menuHelp->Enable(HelpAbout, b);}

protected:
#	if wxCHECK_VERSION(2, 9, 0)
	wxMappingMode         mapMode;
#	else
	int         mapMode;
#	endif
	double      xUserScale;
	double      yUserScale;
	int         xLogicalOrigin;
	int         yLogicalOrigin;
	bool        xAxisReversed, yAxisReversed;
	A_Tabwnd*   tabwnd;
	wxMenu* menuFile, * menuEdit, * menuHelp, * MenuOpts;
	// whether to set canvas to do anit-aliasing
	bool aadraw;

	// A hack to work-around bug in long help display in status bar.
	// See comments in the method definition.
	void StatusBarAntiClobberHack();

	A_Tabpage* NewPage(wxString title);
	A_Tabpage* GetCurPage();
	A_Tabpage* GetNumPage(unsigned num);
	unsigned   GetPageNum(const A_Tabpage* p);
	void       GetAllPagePtrs(std::vector<A_Tabpage*>& v);

private:
#	if wxUSE_DRAG_AND_DROP
	// An internal class to be a file drop target
	class a_frame_filedroptarget : public wxFileDropTarget {
		friend class A_Frame;
		A_Frame* af;
		a_frame_filedroptarget(); // no definition of default
		a_frame_filedroptarget(A_Frame* fr) : af(fr) {}
		// file drop target virtual override
		virtual bool OnDropFiles(
			wxCoord x, wxCoord y, const wxArrayString& filenames);
	};
	// a_frame_filedroptarget::OnDropFiles() will call
	// A_Frame::OnDropFiles(), which we want private,
	// so the method must be a friend
	friend bool a_frame_filedroptarget::OnDropFiles(
		wxCoord x, wxCoord y, const wxArrayString& filenames);
	// file drop target, for our drop-target's virtual override
	// of the same name (which just calls this)
	virtual bool OnDropFiles(
		wxCoord x, wxCoord y, const wxArrayString& filenames);
#	endif // wxUSE_DRAG_AND_DROP

	// WX macro to setup event handling
	DECLARE_EVENT_TABLE()
};

#endif  // _A_FRAME_H_

