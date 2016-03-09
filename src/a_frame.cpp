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

// A Frame for GUI application.

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/colordlg.h>
#include <wx/toolbar.h>
#if wxCHECK_VERSION(2, 4, 0)
#include <wx/filename.h>
#endif
#include <wx/filedlg.h>
#include <wx/artprov.h>
#include <wx/utils.h>

#ifdef __GNUG__
    #pragma implementation
#endif
#include "a_frame.h"
#include "a_ruler.h"
#include "a_aboutbox.h"
#include "a_prefs_manager.h"

// A_Frame
//

BEGIN_EVENT_TABLE(A_Frame, wxFrame)
 EVT_CLOSE       (A_Frame::OnQuit)
#if 0
 EVT_MENU_RANGE  (menu_tool_first, menu_tool_last, A_Frame::OnOption)
#else
 EVT_MENU        (SaveCurves, A_Frame::OnOption)
 EVT_MENU        (SaveAsCurves, A_Frame::OnOption)
 EVT_MENU        (NewTab, A_Frame::OnOption)
 EVT_MENU        (NewTabOpenCurves, A_Frame::OnOption)
 EVT_MENU        (CloseCurves, A_Frame::OnOption)
 EVT_MENU        (Xit_Quit, A_Frame::OnOption)
 EVT_MENU        (EdUndo, A_Frame::OnOption)
 EVT_MENU        (EdRedo, A_Frame::OnOption)
 EVT_MENU        (EdCopy, A_Frame::OnOption)
 EVT_MENU        (EdCut, A_Frame::OnOption)
 EVT_MENU        (EdDel, A_Frame::OnOption)
 EVT_MENU        (EdPaste, A_Frame::OnOption)
 EVT_MENU        (EdMoveDown, A_Frame::OnOption)
 EVT_MENU        (EdMoveUp, A_Frame::OnOption)
 EVT_MENU        (ZoomStd, A_Frame::OnOption)
 EVT_MENU        (ZoomIn, A_Frame::OnOption)
 EVT_MENU        (ZoomOut, A_Frame::OnOption)
 EVT_MENU        (HelpHelp, A_Frame::OnOption)
 EVT_MENU        (HelpDemo, A_Frame::OnOption)
 EVT_MENU        (HelpAbout, A_Frame::OnOption)
 EVT_MENU_RANGE  (menu_tool_seq_start,menu_tool_last, A_Frame::OnOption)
#endif
END_EVENT_TABLE()

A_Frame::A_Frame(
	const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(0, -1, title, pos, size)
{
	// set the frame icons
	SetIcons(iconBundle);

	menuFile = new wxMenu(wxMENU_TEAROFF);
	menuEdit = new wxMenu(wxMENU_TEAROFF);
	MenuOpts = new wxMenu(wxMENU_TEAROFF); // now called "Tools"
	menuHelp = new wxMenu(wxMENU_TEAROFF);

	// stock menu strings are not working in wx 2.6
#if ! wxCHECK_VERSION(2, 8, 0)
#	define  GetItemLabelText GetLabel
#	undef  USE_NONSTOCK_MENU_STRINGS
#	define USE_NONSTOCK_MENU_STRINGS 1
#endif
	// stock menu strings are not working in MSW;
	// assuming they only work in wxGTK 2
#if ! defined(__WXGTK20__)
#	undef  USE_NONSTOCK_MENU_STRINGS
#	define USE_NONSTOCK_MENU_STRINGS 1
#endif

	// use a macros to append menu items; done initially
	// for switching to use of wx stock items , but might also
	// add flexibility -- for different types set ikind
	// before for macro, and then reset ikind to wxITEM_NORMAL
	wxItemKind ikind = wxITEM_NORMAL;
#	undef  MNADD0   // 0 ARGS *after menu pointer and id args*
#	undef  MNADD0NS // 0 ARGS *after menu pointer and id args*
#	undef  MNADD1   // *only* 'tip' arg
#	undef  MNADD1NS // *only* 'tip' arg
#	undef  MNADD2   // use 'str' and 'tip'
#	undef  MNADD2NS // use 'str' and 'tip'
#	if USE_NONSTOCK_MENU_STRINGS
#	define MNADD0(mp, id, str, tip) \
		(mp)->Append(id, str, tip, ikind)
#	define MNADD1(mp, id, str, tip) \
		(mp)->Append(id, str, tip, ikind)
#	define MNADD2(mp, id, str, tip) \
		(mp)->Append(id, str, tip, ikind)
#	else
#	define MNADD0(mp, id, str, tip) \
		(mp)->Append(id)
#	define MNADD1(mp, id, str, tip) \
		(mp)->Append(id, wxEmptyString, tip)
#	define MNADD2(mp, id, str, tip) \
		(mp)->Append(id, str, tip, ikind)
#	endif
	// not wx stock item
#	define MNADD0NS(mp, id, str, tip) \
		(mp)->Append(id, str, tip, ikind)
#	define MNADD1NS(mp, id, str, tip) \
		(mp)->Append(id, str, tip, ikind)
#	define MNADD2NS(mp, id, str, tip) \
		(mp)->Append(id, str, tip, ikind)

// hack macro for wx stock items w/o accelerators when wanted
#	if wxCHECK_VERSION(2, 8, 0)
#		define MNACCHACK(mp, id, accl) do { \
			wxMenuItem* p = (mp)->FindItem(id); \
			if ( p ) { \
				wxString l = p->GetItemLabelText(); \
				if ( l == p->GetItemLabel() ) { \
					p->SetItemLabel(l + wxT("\t") + accl); \
				} \
			} \
		} while ( 0 )
#	else
#		define MNACCHACK(mp, id, accl) do {;} while ( 0 )
#	endif

	// File:
	MNADD0(menuFile, SaveCurves
	, _("&Save\tCtrl-S"), _("Save to current file"));
	MNADD0(menuFile, SaveAsCurves
	, _("Save &As"), _("Save to a new file"));
	MNADD1(menuFile, NewTab
	, _("Open &New Tab"), _("Open a new tab to create new file"));
	MNADD0NS(menuFile, OpenCurves
	, _("Open He&re"), _("Load file into this tab"));
	MNADD1(menuFile, NewTabOpenCurves
	, _("&Open\tCtrl-O"), _("Load file in a new tab"));
	MNADD0(menuFile, CloseCurves
	, _("&Close\tCtrl-W"), _("Close current file"));
	MNADD0NS(menuFile, CloseTab
	, _("Close Ta&b"), _("Close current tab"));
	MNADD0NS(menuFile, ExportCurves
	, _("&Export\tCtrl-E"), _("Export to POV-Ray include file"));
	MNADD0NS(menuFile, ExportAsCurves
	, _("Expor&t As"), _("Export to a new POV-Ray include file"));
	menuFile->AppendSeparator();
	MNADD0(menuFile, Xit_Quit
	, _("&Quit\tCtrl-Q"), _("Quit this program"));

	// Edit:
	MNADD0(menuEdit, EdUndo
	, _("&Undo\tCtrl-Z"), _("Undo last change"));
	MNACCHACK(menuEdit, EdUndo, _("Ctrl-Z"));
	MNADD0(menuEdit, EdRedo
	, _("&Redo\tCtrl-R"), _("Redo undone change"));
	MNACCHACK(menuEdit, EdRedo, _("Ctrl-R"));
	menuEdit->AppendSeparator();
	MNADD0(menuEdit, EdCopy
	,_("&Copy\tCtrl-C"), _("Copy selected curve"));
	//MNADD0NS(menuEdit, EdCopyAll
	//, _("Copy All"), _("Copy all curves"));
	MNADD0(menuEdit, EdCut
	, _("Cu&t\tCtrl-T"), _("Cut selected curve"));
	MNADD0(menuEdit, EdDel
	, _("&Delete\tCtrl-X"), _("Delete selected curve"));
	MNADD0(menuEdit, EdPaste
	, _("&Paste\tCtrl-V"), _("Put copied curve"));
	MNADD0NS(menuEdit, EdCopyGlobal
	,_("Copy Global"), _("Copy selected curve globally"));
	//MNADD0NS(menuEdit, EdCopyAllGlobal
	//, _("Copy All Global"), _("Copy all curves globally"));
	MNADD0NS(menuEdit, EdCutGlobal
	, _("Cut Global"), _("Cut selected curve globally"));
	MNADD0NS(menuEdit, EdPasteGlobal
	, _("Paste Global"), _("Put globally copied curve"));
	menuEdit->AppendSeparator();
	MNADD1(menuEdit, EdMoveDown
	, _("Move Down"), _("Move selected object down in stacking order"));
	MNADD1(menuEdit, EdMoveUp
	, _("Move Up"), _("Move selected object up in stacking order"));
	menuEdit->AppendSeparator();
	MNADD2NS(menuEdit, EdGlobalPreferences
	, _("Pre&ferences"), _("Edit application preferences"));

	// Tools:
	MNADD0NS(MenuOpts, SetBGImage
	, _("Set &Background Image\tCtrl-B"),
	_("Set or change a background image on the drawing area"));
	MNACCHACK(MenuOpts, SetBGImage, _("Ctrl-B"));

	ikind = wxITEM_CHECK;
	MNADD0NS(MenuOpts, HideBGImage
	, _("&Hide Background Image"),
	_("Toggle visibility of a background image on the drawing area"));
	MenuOpts->Check(HideBGImage, false);

	ikind = wxITEM_NORMAL;
	MNADD0NS(MenuOpts, RmBGImage
	, _("&Remove Background Image"),
	_("Remove a background image on the drawing area"));
	MenuOpts->AppendSeparator();
	MNADD0NS(MenuOpts, SetUserScale
	, _("&Set Scale"), _("Set scale of view"));
	MNADD0NS(MenuOpts, CycleUserScale
	, _("&Cycle Scale"),
	_("Cycle through normal, large, and small scale of view"));

	ikind = wxITEM_CHECK;
	MNADD0NS(MenuOpts, Toggle_AA_Lines
	, _("Use &Quick Drawing"),
	_("Toggle drawing type between slow and pretty or fast and ugly"));

	ikind = wxITEM_NORMAL;
	MNADD0NS(MenuOpts, SaveClientImage
	, _("Save Visable To File"),
	_("Save the visible drawing area to a raster image file"));

	// Help:
	MNADD1(menuHelp, HelpHelp
	, _("&Help\tCtrl-H"), _("Show application help"));
	MNADD1NS(menuHelp, HelpHelpTableOfContents
	, _("Help &Contents"), _("Show application help table of contents"));
	menuHelp->AppendSeparator();
	MNADD2(menuHelp, HelpDemo
	, _("&Preview")
	, _("Run POV-Ray on a test file with the current curves"));
	// Ubuntu Unity fights: wants label to be stock "Apply"
	// Sigh, it's still "Apply" -- Ubuntu is soooo *special*
	menuHelp->SetLabel(HelpDemo, _("&Preview"));
	menuHelp->AppendSeparator();
	MNADD0(menuHelp, HelpAbout
	, _("&About"), _("Show about dialog"));

	// append the freshly created menu to the menu bar ...
	wxMenuBar *menuBar = new wxMenuBar(wxMB_DOCKABLE);
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuEdit, _("&Edit"));
	menuBar->Append(MenuOpts, _("&Tools"));
	menuBar->Append(menuHelp, _("&Help"));

	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);

	menuBar->SetThemeEnabled(true);

	// Set up toolbar
	CreateToolBar(wxNO_BORDER|wxHORIZONTAL|wxTB_FLAT|wxTB_DOCKABLE, -1);
	wxToolBar* tb = GetToolBar();
	tb->SetMargins(2, 2);
	tb->SetToolPacking(2);
	tb->SetToolSeparation(5);
	tb->SetThemeEnabled(true);

	// wx bug (2.8): all of wxMenuItem::Get{Name,Text,Label{.Text}}
	// are returning an empty string (GetHelp() is working); the
	// GetLabel{,Text} functions work when used above (but that
	// was before the item was added to wxMenu) -- so, default
	// text is needed
#	undef  MENU2TOOLBAR
#	define MENU2TOOLBAR(mn, tb, id, art, txdef) do { \
		wxMenuItem* mi = (mn)->FindItem(id); \
		wxString tx = mi->GetItemLabelText(); \
		if ( tx.IsEmpty() ) tx = txdef; \
		wxString hp = mi->GetHelp(); \
		wxBitmap bm = wxArtProvider::GetBitmap(art, wxART_TOOLBAR); \
		(tb)->AddTool(id, tx, bm, wxNullBitmap,wxITEM_NORMAL, tx, hp); \
	} while ( 0 )

	MENU2TOOLBAR(menuFile, tb, Xit_Quit, wxART_QUIT, _("Quit"));
	tb->AddSeparator();
	MENU2TOOLBAR(menuFile, tb, NewTab, wxART_NEW, _("Open New Tab"));
	MENU2TOOLBAR(menuFile, tb, NewTabOpenCurves, wxART_FILE_OPEN, _("Open"));
	MENU2TOOLBAR(menuFile, tb, SaveCurves, wxART_FILE_SAVE, _("Save"));
	tb->AddSeparator();
	MENU2TOOLBAR(menuEdit, tb, EdPaste, wxART_PASTE, _("Paste"));
	MENU2TOOLBAR(menuEdit, tb, EdCopy, wxART_COPY, _("Copy"));
	MENU2TOOLBAR(menuEdit, tb, EdCut, wxART_CUT, _("Cut"));
	tb->AddSeparator();
	MENU2TOOLBAR(menuEdit, tb, EdMoveDown, wxART_GO_DOWN, _("Move Down"));
	MENU2TOOLBAR(menuEdit, tb, EdMoveUp, wxART_GO_UP, _("Move Up"));
	tb->AddSeparator();
	MENU2TOOLBAR(menuEdit, tb, EdUndo, wxART_UNDO, _("Undo"));
	MENU2TOOLBAR(menuEdit, tb, EdRedo, wxART_REDO, _("Redo"));
	tb->AddSeparator();
	MENU2TOOLBAR(MenuOpts, tb, CycleUserScale, wxART_GO_TO_PARENT, _("Cycle Scale"));
	MENU2TOOLBAR(menuHelp, tb, HelpDemo, wxART_TICK_MARK, _("Preview"));
	MENU2TOOLBAR(menuHelp, tb, HelpHelp, wxART_HELP, _("Show application help table of contents"));
#	undef  MENU2TOOLBAR

	tb->Realize();

	// create a status bar
#	if OLD_STATBAR_SET
#if defined(__WXMSW__)
	wxStatusBar* sb = CreateStatusBar(4, wxST_SIZEGRIP);
#else
	wxStatusBar* sb = CreateStatusBar(4, 0);
#endif
#if wxCHECK_VERSION(2, 4, 0)
	int swid[4] = {-2, -1 , -1, -1};
#else
	int swid[4] = {-1, -1 , -1, -1};
#endif
	sb->SetStatusWidths(4, swid);
#	else
	wxStatusBar* sb = CreateStatusBar(3, 0);
	int swid[3] = {-3, -1 , -1};
	sb->SetStatusWidths(3, swid);
#	endif

	sb->SetThemeEnabled(true);

	wxString ts; // temp string
	ts.Printf(_("Welcome to \"%s\""),
		(const wxChar*)wxGetApp().GetAppTitle().c_str());
	SetStatusText(ts);

	// TODO (since originally copying from example app)
	mapMode = wxMM_TEXT;
	xUserScale = 1.0;
	yUserScale = 1.0;
	xLogicalOrigin = 0;
	yLogicalOrigin = 0;
	xAxisReversed = yAxisReversed = FALSE;

	// canvas uses this bool to draw either pretty and slow
	// anti-aliased lines, or quick and ugly lines with wxDC::Draw...
	aadraw = wxGetApp().IsDisplayLocal();
	if ( const char* p = std::getenv("EPSPLINE_QUICK_CANVAS") ) {	
		if ( ! std::strcmp(p, "1") || ! std::strcmp(p, "true") ) {
			aadraw = false;
		} else {
			aadraw = true;
		}
	}
	// (un)check menu item
	MenuOpts->Check(Toggle_AA_Lines, ! aadraw);

	tabwnd = new A_Tabwnd(this, TabWindowID
		, wxDefaultPosition, wxDefaultSize
#	if USE_AUI_NOTEBOOK
		, wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_WINDOWLIST_BUTTON);
	// TODO, SOMEDAY MAYBE: write art provider for AUI notebook;
	// derives from wxAuiTabArt; see wx/aui/auibook.h
	//tabwnd->SetArtProvider(new wxAuiSimpleTabArt); // prefer default
#	else
		, wxRAISED_BORDER);
#	endif

	SetThemeEnabled(true);
	tabwnd->SetAutoLayout(true);
	wxBoxSizer* szrMain = new wxBoxSizer(wxVERTICAL);
#if wxCHECK_VERSION(2, 6, 0)
	szrMain->Add(tabwnd, 1, wxGROW, 0);
#else
	wxNotebookSizer* nbs = new wxNotebookSizer(tabwnd);
	szrMain->Add(nbs, 1, wxGROW|wxALIGN_CENTER, 0);
#endif
	SetSizer(szrMain);
	SetAutoLayout(true);
	Layout();

	unsigned nopen = wxGetApp().GetNumFiles();
	if ( nopen > 0 ) {
		nopen = Open_FileArray(wxGetApp().GetFileArray());
	}
	if ( nopen == 0 ) {
		/* TRANSLATORS: this string is meant for an interface tab
		 * that does not have a file associated with it. If it
		 * had a file, the file name would be displayed.
		 * This string is symbolic, or suggestive; it is not a
		 * word (but, of course, you may use words if that is more
		 * sensible, and will not be confused with a file-name).
		 */
		NewPage(_("[--]"));
	}

	tabwnd->SetSelection(0);
	tabwnd->UpdateCurPageText();
	GetCurPage()->GetCanvas()->SetActive(true);
	GetCurPage()->GetCanvas()->UpdateStatusBar();
	
#	if wxUSE_DRAG_AND_DROP
	// put drop target (from our internal class) in place
	SetDropTarget(new a_frame_filedroptarget(this));
#	endif // wxUSE_DRAG_AND_DROP

}

A_Frame::~A_Frame()
{
}

A_Tabpage*
A_Frame::NewPage(wxString title)
{
	A_Tabpage_managed* pagewnd =
		new A_Tabpage_managed(this, tabwnd, -1);

	A_Canvas* canvas = pagewnd->GetCanvas();

	if ( canvas == 0 ) {
		pagewnd->Destroy();
		return 0;
	}

	const int scrollunit = 15;
	const int vitualfactor = 14;
	canvas->SetScrollbars(
		scrollunit, scrollunit,
		scrollunit * vitualfactor, scrollunit * vitualfactor);

	if ( !tabwnd->AddPage(pagewnd, title) ) {
		pagewnd->Destroy();
		return 0;
	}

	Layout();
	return pagewnd;
}

void
A_Frame::ErrorBox(const wxString& msg, const wxString& titletail)
const
{
	//wxString tit = wxGetApp().GetAppTitle();
	wxString tit = wxGetApp().GetBinName();

	wxString ttl;
	// TRANSLATORS: %1$s is application binary name ("epspline")
	// and %2$s is an additional message suitable for an error
	// messsage box
	ttl.Printf(_("%1$s: %2$s"), tit.c_str(), titletail.c_str());

	wxMessageBox(msg, ttl
		, wxCENTRE | wxICON_ERROR | wxOK
		, const_cast<A_Frame*>(this));
}

A_Tabpage*
A_Frame::GetNumPage(unsigned num)
{
	if ( num >= tabwnd->GetPageCount() )
		return 0;

	#if defined(__SC__) && !defined(_CPPRTTI)
	A_Tabpage* t =
		(A_Tabpage*)tabwnd->GetPage(num);
	#else
	A_Tabpage* t =
	dynamic_cast<A_Tabpage*>(tabwnd->GetPage(num));
	#endif
	return t;
}

A_Tabpage*
A_Frame::GetCurPage()
{
	if ( tabwnd->GetPageCount() < 1 )
		return 0;

	int n = tabwnd->GetSelection();
	if ( n < 0 )
		return 0;

	return GetNumPage(unsigned(n));
}

void
A_Frame::GetAllPagePtrs(std::vector<A_Tabpage*>& v)
{
	unsigned cnt = tabwnd->GetPageCount();
	clear(v);
	v.reserve(cnt);
	
	for ( unsigned i = 0; i < cnt; i++ ) {
		if ( A_Tabpage* t = GetNumPage(i) ) {
			v.push_back(t);
		}
	}
}

unsigned
A_Frame::GetPageNum(const A_Tabpage* p)
{
	unsigned cnt = tabwnd->GetPageCount();
	
	for ( unsigned i = 0; i < cnt; i++ ) {
		if ( p == GetNumPage(i) ) {
			return i;
		}
	}

	return std::numeric_limits<unsigned>::max();
}

bool
A_Frame::QueryDirtyData()
{
	std::vector<A_Tabpage*> v;
	std::vector<A_Tabpage*>::iterator i;

	GetAllPagePtrs(v);

	for ( i = v.begin(); i != v.end(); i++ ) {
		A_Tabpage* t = *i;

		if ( t->GetCanvas()->IsDirty() ) {
			return true;
		}
	}

	return false;
}

bool
A_Frame::PreOnQuit(bool force)
{
	// close open dialogs
	CloseDialogs();

	wxString names(wxT(""));
	unsigned ndirt = 0;
	std::vector<A_Tabpage*> v;
	std::vector<A_Tabpage*>::iterator i;

	GetAllPagePtrs(v);

	for ( i = v.begin(); i != v.end(); i++ ) {
		A_Tabpage* t = *i;

		if ( t->GetCanvas()->IsDirty() ) {
			wxString fn(t->GetCanvas()->GetCurFullpath());
			if ( ndirt ) {
				names += wxT("\n");
			}
			names += fn;
			ndirt++;
		}
	}

	// dirty data and cannot cancel:
	if ( ndirt != 0 && force ) {
		// in the forced case, even a message dialog is N.G., as
		// testing under MSW has shown (cannot even click button
		// fast enough under logoff/shutdown), so try to force
		// save of dirty data.
		for ( i = v.begin(); i != v.end(); i++ ) {
			A_Tabpage* t = *i;
			if ( t->GetCanvas()->IsDirty() ) {
				t->GetCanvas()->ForceSave();
			}
		}
	// dirty data and *can* cancel:
	} else if ( ndirt != 0 ) {
		int bstyle = wxCENTRE|wxICON_QUESTION|wxYES_NO|wxCANCEL;
		wxString msg(_("There are unsaved changes to:\n\n"));
		msg += names + _("\n\nDo you want to save the data?");
		msg += _("\n(Select Cancel to continue.)");

		wxString ttl = wxGetApp().GetAppTitle() + _(": save changes?");
		int rep = ::wxMessageBox(msg, ttl, bstyle, this);

		switch ( rep ) {
		case wxYES:
			for ( i = v.begin(); i != v.end(); i++ ) {
				A_Tabpage* t = *i;
				if ( t->GetCanvas()->IsDirty() ) {
					t->GetCanvas()->Save();
				}
			}
			break;
		case wxNO:
			break;
		case wxCANCEL:
		default:
			// cancel
			return false;
		}
	}

	return true;
}

void
A_Frame::OnQuit(wxCloseEvent& e)
{
	// arg has reversed sense of e.CanVeto()
	bool pre_cancel = PreOnQuit(e.CanVeto() == false);

	if ( e.CanVeto() && pre_cancel == false ) {
		e.Veto(true);
		return;
	}

	// Because this instance is the parent of the preference dialog,
	// this must trigger its deletion before this is gone.
	wxGetApp().delete_prefs_dialog();

	if ( wxConfigBase* pConfig = wxGetApp().GetCfgPtr() ) {
		// save state -- per display where applicable
		long mn, mx;
		mn  = IsIconized() ? 1 : 0;
		pConfig->Write(wxT("iconized"), mn);
		mx  = IsMaximized() ? 1 : 0;
		pConfig->Write(wxT("maximized"), mx);

		if ( mn == 0 && mx == 0 ) {
			int x, y, w, h;
			GetSize(&w, &h);
			GetPosition(&x, &y);
			pConfig->Write(wxT("x"), (long)x);
			pConfig->Write(wxT("y"), (long)y);
			pConfig->Write(wxT("w"), (long)w);
			pConfig->Write(wxT("h"), (long)h);
		}
	}

	// UPDATE 11-24-15: this line had been pre-proc-conditional on
	// ! wxCHECK_VERSION(2, 8, 0) || defined(__WXMSW__)
	// but after a new segfault with a certain wx 3.0.2 build led me
	// to try again, the 2,8 problem was gone.  It had seemed to
	// prevent config data save.
	// UPDATE 03.08.2016: once again, explicit quit prevents
	// config data save; re-commented.
	//wxGetApp().QuitHelp();

	Destroy();
}

void
A_Frame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	A_Aboutbox ab(this, -1
		, wxString(_("About "))+wxGetApp().GetAppTitle());
	ab.Centre(wxBOTH);
	ab.ShowModal();
}

void
A_Frame::OnOption(wxCommandEvent& event)
{
	int s = -1;
	A_Canvas* canvas = 0;

	switch ( event.GetId() ) {
		case Xit_Quit:
			Close(false);
			return;
		case NewTab:
			if ( NewPage(wxT("[--]")) ) {
				tabwnd->SetSelection(
					tabwnd->GetPageCount() - 1);
				// Argh! after this hack being MSW-only for sooo long,
				// it has appeared (intermittently) under GTK with
				// the wx 2.9.4 (devel); only when using AUI notebook,
				// and DND'ing many files. Just make it unconditional.
				#if 1 || defined(__WXMSW__)
				// 'doze version doesn't layout new tabs
				// until sized.  sheesh.  This causes
				// a slight flash, but it has to be.
				wxSize s(GetSize());
				s.x -= 1;
				SetSize(s);
				s.x += 1;
				SetSize(s);
				#endif
			}
			return;
		case NewTabOpenCurves:
			s = tabwnd->GetSelection();
			{
				A_Tabpage* t = GetCurPage();
				if ( t != 0 )
					canvas = t->GetCanvas();
				
				wxFileDialog d(this,
					_("Please select one or more files"),
					canvas ? canvas->GetCurDirName() : ch2wxs(""),
					canvas ? canvas->GetCurFileName() : ch2wxs(""),
					wxT("*.pse"),
					wxFD_OPEN | wxFD_MULTIPLE);
			
				if ( d.ShowModal() == wxID_OK ) {
					wxArrayString as;
					d.GetPaths(as);
					if ( Open_FileArray(as) < 1 ) {
						if ( s >= 0 ) {
							tabwnd->SetSelection(s);
						} else if ( (s = tabwnd->GetPageCount())>0 ) {
							tabwnd->SetSelection(s - 1);
						}
					}
				}
			}
			return;
		case HelpAbout:
			OnAbout(event);
			return;
	}

	if ( (s = tabwnd->GetSelection()) < 0 ) {
		event.Skip();
		return;
	}

	A_Tabpage* t = GetCurPage();
	if ( t != 0 ) {
		canvas = t->GetCanvas();
	} else {
		event.Skip();
		return;
	}

	if ( canvas == 0 ) {
		event.Skip();
		return;
	}

	switch ( event.GetId() ) {
		case ExportCurves:
			canvas->Export();
			break;
		case ExportAsCurves:
			canvas->ExportAs();
			break;
		case SaveCurves:
			canvas->Save();
			break;
		case SaveAsCurves:
			canvas->SaveAs();
			tabwnd->UpdateCurPageText();
			break;
		case OpenCurves:
			canvas->Open();
			tabwnd->UpdateCurPageText();
			break;
		case CloseTab:
			if ( canvas->CloseOpt() ) {
				tabwnd->DeletePage(s);
				if ( (t = GetCurPage()) != 0 ) {
					if ( (canvas = t->GetCanvas()) == 0 )
						return;
					canvas->SetActive(true);
					canvas->IdleUpdate();
					canvas->UpdateStatusBar();
					StatusBarAntiClobberHack();
				} else
					return;
			}
			break;
		case CloseCurves:
			if ( canvas->CloseOpt() )
				tabwnd->UpdateCurPageText();
			break;
		case EdMoveDown:
			canvas->MoveDown();
			break;
		case EdMoveUp:
			canvas->MoveUp();
			break;
		case EdRedo:
			canvas->Redo();
			StatusBarAntiClobberHack();
			break;
		case EdUndo:
			canvas->Undo();
			StatusBarAntiClobberHack();
			break;
		case EdCopy:
			canvas->clipCopy();
			break;
		case EdCut:
			canvas->clipCut();
			StatusBarAntiClobberHack();
			break;
		case EdPaste:
			canvas->clipPaste();
			StatusBarAntiClobberHack();
			break;
		case EdCopyGlobal:
			canvas->clipCopyGlobal();
			break;
		case EdCutGlobal:
			canvas->clipCutGlobal();
			StatusBarAntiClobberHack();
			break;
		case EdPasteGlobal:
			canvas->clipPasteGlobal();
			StatusBarAntiClobberHack();
			break;
		case EdDel:
			canvas->DelSelPt();
			StatusBarAntiClobberHack();
			break;
		case EdGlobalPreferences:
			wxGetApp().show_prefs_dialog(true);
			break;
		case SetBGImage:
			canvas->DoSetBGImg();
			break;
		case RmBGImage:
			canvas->DoRmBGImg();
			break;
		case HideBGImage:
			{
				bool do_hide = false;
				wxMenuItem* pi =
					GetMenuBar()->FindItem(int(HideBGImage));
				if ( pi ) {
					do_hide = pi->IsChecked();
				}
				canvas->DoHideBGImg(do_hide);
			}
			break;
		case SetUserScale:
			canvas->DoSetScale();
			StatusBarAntiClobberHack();
			break;
		case CycleUserScale:
			canvas->DoCycleScale();
			StatusBarAntiClobberHack();
			break;
		case Toggle_AA_Lines:
			{
				wxMenuItem* pi =
					GetMenuBar()->FindItem(int(Toggle_AA_Lines));
				if ( pi ) {
					aadraw = ! pi->IsChecked();
				}
				std::vector<A_Tabpage*> v;
				GetAllPagePtrs(v);
				std::vector<A_Tabpage*>::iterator i = v.begin();
				while ( i != v.end() ) {
					(*i++)->GetCanvas()->use_AA(aadraw);
				}
			}
			break;
		case SaveClientImage:
			canvas->SaveBGImage();
			return;
		case HelpDemo:
			canvas->Demo();
			return;
		case HelpHelp:
		case HelpHelpTableOfContents:
			if ( ! wxGetApp().ShowHelp(
					event.GetId() == HelpHelp ?
					IDI_MouseKeyRef :
					IDI_HelpTOC
				) ) {
				wxMessageBox(
				 _("The help documents were not loaded successfully."),
				 _("Help Is Not Available"),
				 wxOK | wxICON_ERROR);
			}
			return;
		default:
			event.Skip();
			// skip Refresh()
			return;
	}

	canvas->Refresh();
}

bool
A_Frame::CloseTabIffCanvasAllows(int tabnum)
{
	// Ouch. Found that the tab number from the AUI
	// wxAuiNotebookEvent (wx 2.9) can be one-off; possibly
	// only after dragging a change in tab order.
#	if 0
	if ( tabnum < 0 ) {
		return false;
	}

	A_Canvas* canvas = 0;
	A_Tabpage* t = GetNumPage(unsigned(tabnum));
#	else
	// ... so, rely on GetCurPage() -- when close button is
	// clicked, it should be (better be) button on current page.
	A_Canvas* canvas = 0;
	A_Tabpage* t = GetCurPage();
	if ( t ) tabnum = (int)GetPageNum(t);
#	endif
	if ( t != 0 ) {
		canvas = t->GetCanvas();
	} else {
		return false;
	}

	if ( canvas == 0 ) {
		return false;
	}
	
	if ( canvas->CloseOpt() ) {
		tabwnd->DeletePage(tabnum);
		if ( (t = GetCurPage()) != 0 ) {
			if ( (canvas = t->GetCanvas()) == 0 ) {
				return true;
			}
			canvas->SetActive(true);
			canvas->IdleUpdate();
			canvas->UpdateStatusBar();
			StatusBarAntiClobberHack();
		}
	} else {
		return false;
	}

	return true;
}

// This unfortunate hack can be called just *after* a
// menu or toolbar event handler has made a change in
// the same status bar pane used by the menu/tb help.
// This hack is bad because wxFrameBase::m_oldStatusText
// is undocument, and it, or the method using it (DoGiveHelp()),
// might change with any version. But there seems no other way . . . .
void
A_Frame::StatusBarAntiClobberHack()
{
	// Hack needed so that Menu/Toolbar event handler,
	// such as Undo/Redo, can update
	// status-bar, because toolbar always clobbers
	// the updated text when it replaces saved text.
	if ( wxStatusBar* sb = GetStatusBar() ) {
		int pane = GetStatusBarPane();
		wxString s = sb->GetStatusText(pane);
		// set current text as saved text;
		// now it's clobbered with itself
		wxFrameBase::m_oldStatusText = s;
	}
}

// this should provide set names of all dialog windows that
// are given this as parent -- a hand maintained list of app's
// dialogs, will not include message dialogs or pickers etc.
void
A_Frame::GetDialogNames(wxArrayString& out)
{
	static const wxChar* l[] = {
		wxT("about_data_dialog"),
		wxT("global_pref_dialog"),
		wxT("spline_properties")
	};

	out.Clear();
	out.Alloc(A_SIZE(l));
	
	for ( size_t i = 0; i < A_SIZE(l); i++ ) {
		out.Add(l[i]);
	}
}

// for shutdown; close shown dialogs
void
A_Frame::CloseDialogs()
{
	wxArrayString dlgs;
	
	GetDialogNames(dlgs);
	
	for ( size_t i = 0; i < dlgs.Count(); i++ ) {
		wxDialog* dp = dynamic_cast<wxDialog*>
			(wxWindow::FindWindowByName(dlgs[i], this));

		if ( dp ) {
			if ( dp->IsModal() ) {
				dp->EndModal(wxID_OK);
			} else {
				dp->Close(true);
			}
		}
	}
}

void
A_Frame::PrepareDC(wxDC& dc)
{
	dc.SetMapMode( mapMode );
	dc.SetUserScale( xUserScale, yUserScale );
	dc.SetLogicalOrigin( xLogicalOrigin, yLogicalOrigin );
	dc.SetAxisOrientation( !xAxisReversed, yAxisReversed );
}

void
A_Frame::SetTitlePrefix(const wxString& pre)
{
	wxString t = pre + _(": ") + wxGetApp().GetAppTitle();
	SetTitle(t);
}

bool
A_Frame::SelectColour(wxColour& c)
{
	wxColourData data;
	wxColourDialog dialog(this, &data);

	if ( dialog.ShowModal() == wxID_OK ) {
		c = dialog.GetColourData().GetColour();
		return true;
	}

	return false;
}

// call on prefs update; will refresh
void
A_Frame::PreferenceChanged()
{
	std::vector<A_Tabpage*> v;
	GetAllPagePtrs(v);
	std::vector<A_Tabpage*>::iterator i = v.begin();
	while ( i != v.end() ) {
		(*i++)->GetCanvas()->PreferenceChanged();
	}
}

// return number successfully opened
unsigned
A_Frame::Open_FileArray(const wxArrayString& af)
{
	unsigned ret = 0, n = af.GetCount();

	// Special case: the current tab is empty and has
	// no file name, and we're given exactly one filename,
	// assume the intent is to open that file in the
	// current tab.
	if ( n == 1 ) {
		if ( A_Tabpage* page = GetCurPage() ) {
			wxString nm = page->GetCanvas()->GetCurFileName();
			if ( nm.IsEmpty() ) {
				if ( ! page->GetCanvas()->IsDirty() ) {
					// if not "dirty", and has no filename, then
					// user cannot have started editing on the
					// tab's canvas because the only way the dirty-
					// flag would be cleared (excepting 'undo')
					// is by saving the changes, implying a name.
					page->GetCanvas()->Open(af[0]);
					nm = page->GetCanvas()->GetCurFileName();
					if ( nm.IsEmpty() ) {
						// open failure; setting n = 0 passes next loop
						n = 0;
					} else {
						// success
						tabwnd->UpdateCurPageText();
						page->GetCanvas()->SetActive(true);
						page->GetCanvas()->UpdateStatusBar();
						ret = 1;
					}
				}
			}
		}
	}

	// Note counter init from ret
	for ( unsigned i = ret; i < n; i++ ) {
		wxString fil(af[i]);
		wxString tn;

#if wxCHECK_VERSION(2, 4, 0)
		wxFileName fn(fil);
		tn = fn.GetFullName();
#else
		tn = ::wxFileNameFromPath(fil);
#endif

		A_Tabpage* page = NewPage(tn);

		if ( page ) {
			unsigned pn = GetPageNum(page);
			page->GetCanvas()->Open(fil);
			if ( ! (page->GetCanvas()->GetCurFileName()).IsEmpty() ) {
				// name not empty, open succeeded
				tabwnd->SetSelection(pn);
				ret++;
			} else {
				// name is empty, open failed
				tabwnd->DeletePage(pn);
			}
		}
	}

	// Argh! after this hack being MSW-only for sooo long,
	// it has appeared (intermittently) under GTK with
	// the wx 2.9.4 (devel); only when using AUI notebook,
	// and DND'ing many files. Just make it unconditional.
	#if 1 || defined(__WXMSW__)
	// Wow, this is still needed w/ wx 2.8!
	if ( ret ) {
		// 'doze version doesn't layout new tabs
		// until sized.  sheesh.  This causes
		// a slight flash, but it has to be.
		wxSize s(GetSize());
		s.x -= 1;
		SetSize(s);
		s.x += 1;
		SetSize(s);
	}
	#endif

	return ret;
}

#	if wxUSE_DRAG_AND_DROP
// file drop target called by internal derived class
bool
A_Frame::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& afn)
{
#	if wxCHECK_VERSION(2, 8, 0)
	if ( ! tabwnd->GetRect().Contains(int(x), int(y)) ) {
		return false;
	}
#	else
	if ( ! InRect(tabwnd->GetRect(), wxPoint(int(x), int(y))) ) {
		return false;
	}
#	endif

	if ( Open_FileArray(afn) == 0 ) {
		return false;
	}

	tabwnd->UpdateCurPageText();
	GetCurPage()->GetCanvas()->SetActive(true);
	GetCurPage()->GetCanvas()->UpdateStatusBar();

	return true;
}

// file drop target virtual override of A_Frame internal class
bool
A_Frame::a_frame_filedroptarget::OnDropFiles(
	wxCoord x, wxCoord y,
	const wxArrayString& afn)
{
	return af->OnDropFiles(x, y, afn);
}
#	endif // wxUSE_DRAG_AND_DROP
