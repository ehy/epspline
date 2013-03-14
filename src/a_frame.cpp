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

#include "wx/colordlg.h"
#include "wx/toolbar.h"
#if wxCHECK_VERSION(2, 4, 0)
#include "wx/filename.h"
#endif
#include "wx/artprov.h"
#include "wx/utils.h"

#ifdef __GNUG__
    #pragma implementation
#endif
#include "a_frame.h"
#include "a_ruler.h"
#include "a_aboutbox.h"

// resources
//

// the application icon
#if !defined(__WXMSW__)
#include "art/epspline.xpm"
#endif
#ifndef APP_ICON_NAME
#define APP_ICON_NAME "epspline"
#endif

// whether the xpm icons should be included
#if     USE_APPDIST_ICONS
#undef  INCLUDE_APPDIST_ICONS
#define INCLUDE_APPDIST_ICONS 1
#else
#define INCLUDE_APPDIST_ICONS 0
#endif

#if  INCLUDE_APPDIST_ICONS
#if defined(__WXMSW__) && defined(USE_OLD_MSW_ICONS)
#include "art/ms/delete.xpm"
#define	exit_xpm delete_xpm
#include "art/ms/copy.xpm"
#include "art/ms/cut.xpm"
#include "art/ms/help.xpm"
#include "art/ms/new.xpm"
#include "art/ms/open.xpm"
#include "art/ms/paste.xpm"
#include "art/ms/find_dc2.xpm"
#define	zoom_xpm find_dc2_xpm
#include "art/ms/save.xpm"
#include "art/ms/undo.xpm"
#include "art/ms/redo.xpm"
#include "art/ms/binoc.xpm"
#define	demo_xpm binoc_xpm
#else
#include "art/dj/Exit.xpm"
#define	exit_xpm Exit_xpm
#include "art/dj/Copy.xpm"
#define	copy_xpm Copy_xpm
#include "art/dj/Cut.xpm"
#define	cut_xpm Cut_xpm
#include "art/Question.xpm"
#define	help_xpm Question_xpm
#include "art/dj/New.xpm"
#define	new_xpm New_xpm
#include "art/dj/Open.xpm"
#define	open_xpm Open_xpm
#include "art/dj/Paste.xpm"
#define	paste_xpm Paste_xpm
#include "art/dj/Magnify.xpm"
#define	zoom_xpm Magnify_xpm
#include "art/dj/Save.xpm"
#define	save_xpm Save_xpm
#include "art/dj/Undo.xpm"
#define	undo_xpm Undo_xpm
#include "art/dj/Redo.xpm"
#define	redo_xpm Redo_xpm
#include "art/dj/Binocular.xpm"
#define	demo_xpm Binocular_xpm
#endif
#endif  // #if  INCLUDE_APPDIST_ICONS

#if 0
// Art provider for main toolbar
class toolbar_art : public wxArtProvider {
protected:
	virtual wxBitmap CreateBitmap(const wxArtID& id, 
						const wxArtClient& client,
						const wxSize size)
	{
		return wxNullBitmap;
	}

};
wxArtProvider::Push(new toolbar_art);
#endif

// A_Frame
//

BEGIN_EVENT_TABLE(A_Frame, wxFrame)
 EVT_CLOSE       (A_Frame::OnQuit)
 EVT_MENU_RANGE  (menu_tool_first, menu_tool_last, A_Frame::OnOption)
END_EVENT_TABLE()


A_Frame::A_Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
	// set the frame icon
	SetIcon(wxICON(epspline));

	menuFile = new wxMenu;
	menuEdit = new wxMenu;
	MenuOpts = new wxMenu; // now called "Tools"
	menuHelp = new wxMenu;

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
	// add flexibility
#	undef  MNADD0   // 0 ARGS *after menu pointer and id args*
#	undef  MNADD0NS // 0 ARGS *after menu pointer and id args*
#	undef  MNADD1   // *only* 'tip' arg
#	undef  MNADD1NS // *only* 'tip' arg
#	undef  MNADD2   // use 'str' and 'tip'
#	undef  MNADD2NS // use 'str' and 'tip'
#	if USE_NONSTOCK_MENU_STRINGS
#	define MNADD0(mp, id, str, tip) \
		(mp)->Append(id, str, tip)
#	define MNADD1(mp, id, str, tip) \
		(mp)->Append(id, str, tip)
#	define MNADD2(mp, id, str, tip) \
		(mp)->Append(id, str, tip)
#	else
#	define MNADD0(mp, id, str, tip) \
		(mp)->Append(id)
#	define MNADD1(mp, id, str, tip) \
		(mp)->Append(id, wxEmptyString, tip)
#	define MNADD2(mp, id, str, tip) \
		(mp)->Append(id, str, tip)
#	endif
	// not wx stock item
#	define MNADD0NS(mp, id, str, tip) \
		(mp)->Append(id, str, tip)
#	define MNADD1NS(mp, id, str, tip) \
		(mp)->Append(id, str, tip)
#	define MNADD2NS(mp, id, str, tip) \
		(mp)->Append(id, str, tip)

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

	// Tools:
	MNADD0NS(MenuOpts, SetUserScale
	, _("&Set Scale"), _("Set scale of view"));
	MNADD0NS(MenuOpts, CycleUserScale
	, _("&Cycle Scale"),
	_("Cycle through normal, large, and small scale of view"));
	MNADD0NS(MenuOpts, Toggle_AA_Lines
	, _("&Toggle Quick Drawing"),
	_("Toggle drawing type between slow and pretty or fast and ugly"));
	MNADD0NS(MenuOpts, SaveClientImage
	, _("Save Visable To File"),
	_("Save the visible drawing area to a raster image file"));

	// Help:
	MNADD1(menuHelp, HelpHelp
	, _("&Help\tCtrl-H"), _("Show application help"));
	menuHelp->AppendSeparator();
	MNADD2(menuHelp, HelpDemo
	, _("&Preview")
	, _("Run POV-Ray on a test file with the current curves"));
	menuHelp->AppendSeparator();
	MNADD0(menuHelp, HelpAbout
	, _("&About"), _("Show about dialog"));

	// now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, _("&File"));
	menuBar->Append(menuEdit, _("&Edit"));
	menuBar->Append(MenuOpts, _("&Tools"));
	menuBar->Append(menuHelp, _("&Help"));

	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);

	// Set up toolbar
	CreateToolBar(wxNO_BORDER|wxHORIZONTAL|wxTB_FLAT|wxTB_DOCKABLE, -1);
	wxToolBar* tb = GetToolBar();
	tb->SetMargins(2, 2);
	tb->SetToolPacking(2);
	tb->SetToolSeparation(5);

#if INCLUDE_APPDIST_ICONS && \
		(! defined(__WXGTK20__) || defined(USE_APPDIST_ICONS))
	wxBitmap* bm;
	bm = new wxBitmap(exit_xpm);
	tb->AddTool(Xit_Quit, _("Exit"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Exit"), _("Quit this program"));
	delete bm;
	tb->AddSeparator();
	bm = new wxBitmap(new_xpm);
	tb->AddTool(NewTab, _("New Tab"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("New Tab"), _("Open a new tab"));
	delete bm;
	bm = new wxBitmap(open_xpm);
	tb->AddTool(NewTabOpenCurves, _("Open Curves"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Open Curves"), _("Load curves from file in a new tab"));
	delete bm;
	bm = new wxBitmap(save_xpm);
	tb->AddTool(SaveCurves, _("Save Curves"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Save Curves"), _("Save curves to file"));
	delete bm;
	tb->AddSeparator();
	bm = new wxBitmap(paste_xpm);
	tb->AddTool(EdPaste, _("Paste Curve"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Paste Curve"), _("Put copied curve"));
	delete bm;
	bm = new wxBitmap(copy_xpm);
	tb->AddTool(EdCopy, _("Copy Curve"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Copy Curve"), _("Copy selected curve"));
	delete bm;
	bm = new wxBitmap(cut_xpm);
	tb->AddTool(EdCut, _("Cut Curve"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Cut Curve"), _("Cut selected curve"));
	delete bm;
	tb->AddSeparator();
	bm = new wxBitmap(undo_xpm);
	tb->AddTool(EdUndo, _("Undo Change"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Undo Change"), _("Undo last change"));
	delete bm;
	bm = new wxBitmap(redo_xpm);
	tb->AddTool(EdRedo, _("Redo Change"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Redo Change"), _("Redo undone change"));
	delete bm;
	tb->AddSeparator();
	bm = new wxBitmap(zoom_xpm);
	//tb->AddTool(SetUserScale, _("Set Scale"), *bm, wxNullBitmap, wxITEM_NORMAL
	//	, _("Set Scale"), _("Set Scale of View"));
	tb->AddTool(CycleUserScale, _("Cycle Scale")
		, *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Cycle Scale"),
		_("Cycle through normal, large, and small scale of view"));
	delete bm;
	bm = new wxBitmap(demo_xpm);
	tb->AddTool(HelpDemo, _("Demo Curves")
		, *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Demo Curves")
		, _("Run Povray on a test file with the current curves"));
	delete bm;
	#if 0
	bm = new wxBitmap(print_xpm);
	tb->AddTool(FilePrint, _("Print"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Print"), _("Print Objects"));
	delete bm;
	#endif
	tb->AddSeparator();
	bm = new wxBitmap(help_xpm);
	tb->AddTool(HelpHelp, _("Help"), *bm, wxNullBitmap, wxITEM_NORMAL
		, _("Help"), _("Application help"));
	delete bm;
#else  // ..._APPDIST_ICONS
#	undef  MENU2TOOLBAR
	// wx bug (2.8): all of wxMenuItem::Get{Name,Text,Label{.Text}}
	// are returning an empty string (GetHelp() is working); the
	// GetLabel{,Text} functions work when used above (but that
	// was before the item was added to wxMenu) -- so, default
	// text is needed
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
	MENU2TOOLBAR(menuHelp, tb, HelpHelp, wxART_HELP, _("Show application help"));
#endif // ..._APPDIST_ICONS

	tb->Realize();

	// create a status bar
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
	wxString ts; // temp string
	ts.Printf(_("Welcome to \"%s\""),
		(const wxChar*)wxGetApp().GetAppTitle().c_str());
	SetStatusText(ts);

	m_mapMode = wxMM_TEXT;
	m_xUserScale = 1.0;
	m_yUserScale = 1.0;
	m_xLogicalOrigin = 0;
	m_yLogicalOrigin = 0;
	m_xAxisReversed = m_yAxisReversed = FALSE;
	m_backgroundMode = wxSOLID;

	// canvas uses this bool to draw either pretty and slow
	// anit-aliased lines, or quick and ugly lines with wxDC::Draw...
	aadraw = wxGetApp().IsDisplayLocal();
	if ( const char* p = std::getenv("EPSPLINE_QUICK_CANVAS") ) {	
		if ( ! std::strcmp(p, "1") || ! std::strcmp(p, "true") ) {
			aadraw = false;
		} else {
			aadraw = true;
		}
	}

	tabwnd = new A_Tabwnd(this, TabWindowID
		, wxDefaultPosition, wxDefaultSize
		, wxRAISED_BORDER
		, wxT("tabwindow"));

	tabwnd->SetAutoLayout(true);
	wxBoxSizer* szrMain = new wxBoxSizer(wxVERTICAL);
#if wxCHECK_VERSION(2, 6, 0)
	szrMain->Add(tabwnd, 1, wxGROW|wxALIGN_CENTER, 0);
#else
	wxNotebookSizer* nbs = new wxNotebookSizer(tabwnd);
	szrMain->Add(nbs, 1, wxGROW|wxALIGN_CENTER, 0);
#endif
	SetSizer(szrMain);
	SetAutoLayout(true);

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
	
	// put drop target (from our internal class) in place
	SetDropTarget(new a_frame_filedroptarget(this));
}

A_Frame::~A_Frame()
{
	// Do not delete: segfault; the wx lib takes it over.
	//delete droptarget;
}

A_Tabpage*
A_Frame::NewPage(wxString title)
{
	wxWindowDC dc(this);
	dc.SetMapMode(wxMM_TEXT);
	wxFont fnt, of = dc.GetFont();
	#if defined(__WXMSW__)
	fnt.SetStyle(wxNORMAL);
	fnt.SetFamily(wxSWISS);
	fnt.SetFaceName(wxT("")); // BUG: Always set to MS Sans Serif, a raster.
	#else
	fnt = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	fnt.SetWeight(wxLIGHT);
	#endif
	#if !defined(__WXMOTIF__)
	fnt.SetPointSize(7);
	#endif
	dc.SetFont(fnt);
	wxSize sz;
	dc.GetTextExtent(wxString(wxT("00000")), &sz.x, &sz.y);
	dc.SetFont(of);
	sz.y = sz.x;
	#if defined(__WXMSW__)
	// The MS build seems to need this to get the 3d look . . .
	int style = wxDOUBLE_BORDER;
	#else
	int style = wxRAISED_BORDER;
	#endif
	// . . . for these the GetTextExtent hack is insufficient:
	#if defined(__WXMSW__) || defined(__WXX11__)
	sz.x += 6;
	sz.y += 6;
	#elif !defined(__WXGTK__)
	sz.x += 2 * wxSystemSettings::GetMetric(wxSYS_EDGE_Y);
	sz.y += 2 * wxSystemSettings::GetMetric(wxSYS_EDGE_Y);
	#endif
	wxColor fclr(0x00, 0x00, 0x00);
	wxColor tclr(0x40, 0x40, 0xFF);

	wxBoxSizer* szrTop  = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* szrBot  = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* szrMain = new wxBoxSizer(wxVERTICAL);

	A_Tabpage* pagewnd = new A_Tabpage(tabwnd, -1
		, 0, 0, 0
		, wxDefaultPosition, wxDefaultSize
		, 0);

	A_Canvas* canvas;
	if ( const char* p = std::getenv("EPSPLINE_BUFFER_CANVAS") ) {	
		if ( ! std::strcmp(p, "1") || ! std::strcmp(p, "true") ) {
			canvas = new A_BUFDCCanvas(this, pagewnd, aadraw);
		} else {
			canvas = new A_Canvas(this, pagewnd, aadraw);
		}
	} else {
		// A platform's own double buffering makes
		// the buffered canvas redundant and *slow*.
		// The platform macro tests were found in dcbuffer.h;
		// so far only GTK2 is tested.
		// Note GTK2 no longer really needs this test since
		// finding 'SetDoubleBuffered(bool)', which is used in
		// buffered canvas ctor; using that, GTK2 performance is
		// not perceptibly different either way.
#		if defined(__WXGTK20__) || \
			defined(__WXMAC__) || defined(__WXDFB__)
			canvas = new A_Canvas(this, pagewnd, aadraw);
#		else // defined(__WXGTK__)
			canvas = new A_BUFDCCanvas(this, pagewnd, aadraw);
#		endif // defined(__WXGTK__)
	}

	const int scrollunit = 15;
	const int vitualfactor = 14;
	canvas->SetScrollbars(
		scrollunit, scrollunit,
		scrollunit * vitualfactor, scrollunit * vitualfactor);

	// top-left button
	wxWindow* tlbut = new wxWindow(pagewnd, -1
			, wxDefaultPosition, sz, style);
	szrTop->Add(tlbut
		, 0, wxALIGN_LEFT|wxALIGN_TOP, 0);
	// top ruler
	A_Ruler* hr = new A_Ruler(pagewnd
		, -1, fnt, fclr, tclr
		, wxDefaultPosition, sz, style);
	hr->SetType(rlr_horz);
	szrTop->Add(hr, 1
		, wxALIGN_RIGHT|wxALIGN_LEFT|wxALIGN_TOP|wxGROW
		, 0);
	canvas->SetHRule(hr);
	// side ruler
	A_Ruler* vr = new A_Ruler(pagewnd
		, -1, fnt, fclr, tclr
		, wxDefaultPosition, sz, style);
	vr->SetType(rlr_vert);
	szrBot->Add(vr, 0
		, wxGROW|wxALIGN_LEFT|wxALIGN_TOP|wxALIGN_BOTTOM
		, 0);
	canvas->SetVRule(vr);
	szrBot->Add(canvas, 1, wxGROW|wxALL, 0);
	// add sub-sizers
	szrMain->Add(szrTop, 0
		, wxGROW|wxALIGN_RIGHT|wxALIGN_LEFT|wxALIGN_TOP);
	szrMain->Add(szrBot, 1, wxGROW|wxALL);
#if wxCHECK_VERSION(2, 4, 0)
	tlbut->SetThemeEnabled(true);
	hr->SetThemeEnabled(true);
	vr->SetThemeEnabled(true);
#endif

	if ( !tabwnd->AddPage(pagewnd, title) ) {
		pagewnd->Destroy();
		return 0;
	}

	// put main sizer
	pagewnd->SetSizer(szrMain);
	pagewnd->SetCanvas(canvas);
	pagewnd->SetHRule(hr);
	pagewnd->SetVRule(vr);
	pagewnd->SetAutoLayout(true);

	return pagewnd;
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
A_Frame::ErrorBox(const wxString& msg, const wxString& titletail)
const
{
	//wxString ttl = wxGetApp().GetAppTitle();
	wxString ttl = wxGetApp().GetBinName();

	if ( titletail != wxT("") ) {
		ttl += _(":  ");
		ttl += titletail;
	}

	wxMessageBox(msg, ttl
		, wxCENTRE | wxICON_EXCLAMATION | wxOK
		, const_cast<A_Frame*>(this));
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

void
A_Frame::OnQuit(wxCloseEvent& e)
{
	wxString names(wxT(""));
	unsigned ndirt = 0;
	std::vector<A_Tabpage*> v;

	GetAllPagePtrs(v);
	std::vector<A_Tabpage*>::iterator i = v.begin();
	while ( i != v.end() ) {
		A_Tabpage* t = *i++;

		if ( t->GetCanvas()->IsDirty() ) {
			wxString fn(t->GetCanvas()->GetCurFullpath());
			if ( ndirt ) {
				names += wxT("\n");
			}
			names += fn;
			ndirt++;
		}
	}

	if ( ndirt ) {
		int bstyle = wxCENTRE|wxICON_QUESTION|wxYES_NO;
		wxString msg(_("There are unsaved changes to \n"));
		msg += names + _("\nDo you want to save the data?");
		if ( e.CanVeto() ) {
			bstyle |= wxCANCEL;
			msg += _("\n(Select Cancel to continue.)");
		}
		wxString ttl = wxGetApp().GetAppTitle() + _(": save?");
		int rep = ::wxMessageBox(msg, ttl, bstyle, this);

		switch ( rep ) {
		case wxYES:
			i = v.begin();
			while ( i != v.end() ) {
				A_Tabpage* t = *i++;
				if ( t->GetCanvas()->IsDirty() )
					t->GetCanvas()->Save();
			}
			break;
		case wxNO:
			break;
		case wxCANCEL:
			if ( e.CanVeto() ) {
				e.Veto(true);
				return;
			}
			break;
		}
	}

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

	// Do not quit help browser window: prevents config data save.
	// With wx 2.6 it is necessary to quit help or it lingers.
#	if ! wxCHECK_VERSION(2, 8, 0)
	wxGetApp().QuitHelp();
#	endif

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
				#ifdef __WXMSW__
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
			if ( A_Tabpage* t = NewPage(wxT("[--]")) ) {
				canvas = t->GetCanvas();
				canvas->Open();
				if  ( canvas->GetCurFileName() == wxT("") ) {
					tabwnd->DeletePage(
						tabwnd->GetPageCount() - 1);
					if ( s >= 0 )
						tabwnd->SetSelection(s);
				} else
					tabwnd->SetSelection(
						tabwnd->GetPageCount() - 1);
				#ifdef __WXMSW__
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
		case HelpAbout:
			OnAbout(event);
			return;
	}

	if ( (s = tabwnd->GetSelection()) < 0 )
		return;

	A_Tabpage* t = GetCurPage();
	if ( t != 0 )
		canvas = t->GetCanvas();
	else
		return;

	if ( canvas == 0 )
		return;

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
				} else
					return;
			}
			break;
		case CloseCurves:
			if ( canvas->CloseOpt() )
				tabwnd->UpdateCurPageText();
			break;
		case EdRedo:
			canvas->Redo();
			break;
		case EdMoveDown:
			canvas->MoveDown();
			break;
		case EdMoveUp:
			canvas->MoveUp();
			break;
		case EdUndo:
			canvas->Undo();
			break;
		case EdCopy:
			canvas->clipCopy();
			break;
		case EdCut:
			canvas->clipCut();
			break;
		case EdPaste:
			canvas->clipPaste();
			break;
		case EdCopyGlobal:
			canvas->clipCopyGlobal();
			break;
		case EdCutGlobal:
			canvas->clipCutGlobal();
			break;
		case EdPasteGlobal:
			canvas->clipPasteGlobal();
			break;
		case EdDel:
			canvas->DelSel();
			break;
		case SetUserScale:
			canvas->DoSetScale();
			break;
		case CycleUserScale:
			canvas->DoCycleScale();
			break;
		case Toggle_AA_Lines:
			{
				std::vector<A_Tabpage*> v;
				GetAllPagePtrs(v);
				std::vector<A_Tabpage*>::iterator i = v.begin();
				while ( i != v.end() ) {
					(*i++)->GetCanvas()->toggle_AA();
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
			if ( ! wxGetApp().ShowHelp() ) {
				wxMessageBox(
				 _("The help documents were not loaded successfully."),
				 _("Help Is Not Available"),
				 wxOK | wxICON_ERROR);
			}
			return;
		default:
			// skip Refresh()
			return;
	}

	canvas->Refresh();
}

void
A_Frame::PrepareDC(wxDC& dc)
{
	dc.SetMapMode( m_mapMode );
	dc.SetUserScale( m_xUserScale, m_yUserScale );
	dc.SetLogicalOrigin( m_xLogicalOrigin, m_yLogicalOrigin );
	dc.SetAxisOrientation( !m_xAxisReversed, m_yAxisReversed );
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

// return number successfully opened
unsigned
A_Frame::Open_FileArray(const wxArrayString& af)
{
	unsigned ret = 0, n = af.GetCount();
	for ( unsigned i = 0; i < n; i++ ) {
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
			page->GetCanvas()->Open(fil);
			if ( ! (page->GetCanvas()->GetCurFileName()).IsEmpty() ) {
				ret++;
			} else {
				tabwnd->DeletePage(tabwnd->GetPageCount() - 1);
			}
		}
	}

	// Wow, this is still needed w/ wx 2.8!
	#ifdef __WXMSW__
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

	unsigned n = tabwnd->GetPageCount() - 1;
	tabwnd->SetSelection(n);
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
