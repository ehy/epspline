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

// The canvas to be drawn on

#ifndef _A_CANVAS_H_
#define _A_CANVAS_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/bitmap.h>
#endif // WX_PRECOMP

#include "cfg.h"
#include "splines.h"

// utility
#include "util.h"
#include "wxutil.h"

// forward declarations
class A_Tabwnd;
class A_Tabpage;
class A_Frame;
class A_Ruler;
class A_Canvas;
class bgimg_manager;


// Buffered DCs not working as hoped, but maybe try again later.
#define USE_wxBufferedDC	0

#ifdef __GNUG__
    #pragma interface
#endif

// For undo and redo: a counted (limited size) stack.
// The std::stack is not suitable.  This *MUST* take
// pointer to allocated objects, as items are deleted
// on stack overflow and destruction.
template <class C>
class counted_stack {
public:
	typedef typename C::value_type value_type;
	typedef typename C::size_type size_type;
	
	static const size_type	default_size = DEFAULT_UNDO_SIZE;
protected:
	C c;
	size_type sz, cnt;
public:

	explicit counted_stack(size_type size = default_size)
		: c(size, value_type(0)), sz(size > 1 ? size : 1), cnt(0) {}
	counted_stack(const C& o)
		: c(o), sz(o.size()), cnt(o.size()) { if ( sz < 1 ) sz = 1; }
	~counted_stack();

	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }

	size_type capacity() const { return sz; }
	size_type count() const { return cnt; }

	// Note: return is *not* a reference.
	value_type top() { return cnt ? c.back() : value_type(0); }
	const value_type top() const
	{ return cnt ? c.back() : value_type(0); }

	void push(const value_type& v);
	void pop() {if ( cnt ) {c.pop_back(); --cnt;}}
	// Here the design of the std::stack is contradicted:
	// popv() returns the popped item; item copy should
	// be efficient -- this will contain pointers.
	value_type popv() {value_type v = top(); pop(); return v;}

	// delete and zero contents, reset count
	void flush();
};

// IDs for canvas right click popup
enum {
	IC_start = 100,
	IC_set_linear = IC_start,
	IC_set_quadratic,
	IC_set_cubic,
	IC_set_bezier,
	IC_set_prism,
	IC_set_lathe,
	IC_set_conicsweep,
	IC_set_linearsweep,
	IC_export,
	IC_exportas,
	IC_save,
	IC_saveas,

	IC_set_props,
	IC_delete,
	IC_move_down,
	IC_move_up,

	IC_set_bg_img,
	IC_rm_bg_img,

	IC_end
};


// base for drawing: most all code should be here
class A_Canvas: public wxScrolledWindow {
protected:
	A_Frame*     a_frame;
	A_Tabpage*   a_tabpg;
	bgimg_manager*	bg_mng;

	bool         aa_draw; // draw anti-aliased curves, or not

	// For guide/snap-to lines
	A_Ruler*     hrule, * vrule;
	wxMenu*      m_pop;
	wxMenu*      m_sel;
	wxPoint      pt_mousedown;

	// Keep some data and state in objects which can be
	// copied and pushed onto undo/redo stacks.
	class DataState {
	public:
		typedef SplineBase       data_type_obj;
		typedef data_type_obj*   data_type_ptr;
		typedef data_type_obj&   data_type_ref;
		typedef data_type_ptr    datatype;
		typedef std::list<datatype>   data_store;
		typedef data_store::iterator itr_t;

		enum ObjTypes { undef, prism, lathe };
		enum SplineTypes { linear, quadratic, cubic, bezier };
		enum SweepTypes { slinear, conic };

		wxString      sFname;
		wxString      sFdir;
		wxString      sSname;
		wxString      sSdir;
		data_store    lst;
		datatype      cur;
		datatype      sel;
		SplinePoint*  selpt;
		ObjTypes      objt;
		SplineTypes   splinet;
		SweepTypes    sweept;
		bool          dirty;

		// horz., vert. guides.
		typedef std::vector<int>  guidestore;
		guidestore   hguides, vguides; // containers
		int*          hgcur, * vgcur;   // current one

		bool IsDirty()         const { return dirty; }
		void SetDirty(bool b = true) { dirty = b; }
		void SetClean()              { SetDirty(false); }

		void UpdateObj(datatype& o);
		datatype NewObj();

		void GotPopup(wxCommandEvent& event);

		DataState();
		DataState(const DataState&);
		~DataState();

		DataState& operator = (const DataState&);
	};

	// callback for bg image manager: tell us to update
	typedef /*bgimg_manager::cb_update_arg*/ void* bg_update_arg;
	static void bg_update(bg_update_arg);

public:
	A_Canvas(A_Frame* parent, A_Tabpage* realparent, bool aa = true);
	virtual ~A_Canvas();

	typedef DataState::data_type_obj data_type_obj;
	typedef DataState::data_type_ptr data_type_ptr;
	typedef DataState::data_type_ref data_type_ref;
	typedef DataState::datatype      datatype;
	typedef DataState::data_store    data_store;
	typedef DataState::itr_t         itr_t;

	// take a device context and paint to region(s) passed
	// in wxRegion& re -- intended as core of OnPaint handler --
	// may be useful elsewhere
	// this does *not* draw selected box or highlights,
	// or transform decorations
	void DCPaint(wxDC& dc, wxRegion& re);
	// take a device context and paint to rect, which is
	// modified by DeviceToLogical[XY] functions (therefore
	// must be in device coords, and not wanted to be const)
	// -- intended as core of OnPaint handler --
	// may be useful elsewhere
	// this does *not* draw selected box or highlights,
	// or transform decorations
	void DCPaint(wxDC& dc, wxRect& rr);
	// override parent dc prep for addl. ops; call parent
	void PrepareDC(wxDC& dc);
	
	// wx event handlers
	void OnIdle(wxIdleEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEraseBG(wxEraseEvent& event);
	void OnScroll(wxScrollWinEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseLDown(wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseLUp  (wxMouseEvent& event);
	void OnMouseRDown(wxMouseEvent& event);
	void OnMouseRUp  (wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp  (wxKeyEvent& event);
	void GotPopup(wxCommandEvent& event);

	void SetHRule(A_Ruler* p = 0);
	void SetVRule(A_Ruler* p = 0);

	bool IsDirty() const { return D ? D->IsDirty() : false; }
	void SetDirty(bool b = true);
	void SetClean() { SetDirty(false); }
	void UpdateStatusBar();

	void ErrorBox(const wxString& msg,
		const wxString& titletail = _("Error")) const;

	// a default padding for various Refresh() calls
	// sdotsz is a constant from splines.h
	static const unsigned refresh_pad = (sdotsz + 1) / 2 + 1;
	
	// call on prefs update; will refresh
	void PreferenceChanged();

	void DrawSelPt();
	void DrawSelPt(wxDC& dc);
	void DrawSelBox(datatype s);
	void DrawSelBox(wxDC& dc, datatype s);
	void DrawCur();
	void DrawCur(wxDC& dc);
	void DelSelPt();
	void RefreshObjBox(data_type_ptr o = 0, wxDC* dc = 0);
	void Export();
	void ExportAs();
	void ExportFILE(FILE*, const wxString& name, bool indemo = false);
	void Undo();
	void Redo();
	void MoveDown();
	void MoveUp();
	void Open(wxString filename);
	void Open();
	void Close();
	void ForceSave(bool namechange = true);
	void Save();
	void SaveAs();
	void Demo();
	bool CloseOpt(bool force = false);

	void DoRClickMenu(int x, int y);
	void DoSetScale();
	void DoCycleScale();

	// For internal clipboard that can hold one object
	void            clipCopy();
	void            clipCut();
	void            clipPaste();
	// For internal global clipboard that can hold one object
	void            clipCopyGlobal();
	void            clipCutGlobal();
	void            clipPasteGlobal();

	// For deleting selection
	void DelSel();

	// File name strings for current data
	wxString GetCurFileName() const {
		return D ? D->sSname : wxString(wxT(""));
	}
	wxString GetCurDirName() const {
		return D ? D->sSdir : wxString(wxT(""));
	}
	wxString GetCurFullpath() const;

	// updates menu items, etc. on response to
	// idle event or public call
	void IdleUpdate();
	void SetActive(bool beactive = true) { activetab = beactive; }

	// whether to slowly draw anti-aliased curves, or quick and ugly
	void use_AA(bool aa = true) { aa_draw = aa; Refresh(); }
	bool query_AA() const { return aa_draw; }
	void toggle_AA() { use_AA(!query_AA()); }

	// provide the background wxImage (const) for, e.g., saving
	const wxImage* GetBGImage() const { return im_main; }

	// save the background wxImage to a file; this is done in this
	// class because current data file directory is here.
	bool SaveBGImage(const wxImage* p = 0) const;

	enum movements {
		arrow = 1,
		page = 10,
		end_movements
	};

protected:
	DataState*       D;
	wxString         filecomment;
	wxCursor*        arrowcursor, * curcursor;
	wxCursor*        scalecursor, * shearcursor, * rotatecursor;
	bool             creating;
	bool             movingsel;
	bool             movingpoint;
	bool             movingtrans, movingtransctr;
	bool             transforming, scaling, shearing, rotating;
	bool             resized; // set in OnSize, used in OnIdle
	bool             in_rulehdlr; // A_Ruler passing mouse events
	bool             activetab; // our tab is active
	wxPoint          transctrpt;

	// For internal clipboard that can hold one object
	datatype     clip;
	// For internal clipboard that can hold one object
	static datatype     clip_global;

	typedef counted_stack<std::deque<DataState*> > undo_stack;
	typedef counted_stack<std::deque<DataState*> > redo_stack;
	static undo_stack::size_type	undo_size;

	undo_stack ustack;
	redo_stack rstack;

	void PushUndo();
	void PopUndo();

	// scale, shear, rotate
	or_enum transtype;
	wxPoint transpt;
	int scaleconstrain, shearconstrain;
	double rotateconstrain;
	static int scaleconstrainval;
	static int shearconstrainval;
	static double rotateconstrainval;
	bool DoTransform(wxDC& dc, bool constrain, bool extmode = false);
	void DrawTransCtr(wxDC& dc, bool clear, bool draw, wxPoint pt);
	bool TestTransCtr(wxPoint pt);
	void GetTransCtrRect(wxRect& r);
	void DrawTransformHandles(wxDC& dc, datatype s);
	or_enum TestTransformHandles(datatype s, const wxPoint& pt);
	static int szhandles;

	static const int def_grid_space = 15;
	// draw bg grid: takes rectangle in device coordinates
	// 'wid' is pixels between grid-lines -- 0 or less means no lines
	// 'pad' is to inflate the rect.; generally should be 0
	void DrawGrid(
		wxDC& dc, const wxRect& r,
		int wid = def_grid_space, int pad = 0);
	// draw bg grid: takes rectangle in 'logical' coordinates
	// 'wid' is pixels between grid-lines -- 0 or less means no lines
	void DrawGridLogical(
		wxDC& dc, const wxRect& r, int wid = def_grid_space);
	// draw bg grid: this draws onto wxImage because as of wx 2.9
	// there is no portably working way to draw into wxBitmap raw data
	// (don't even mention wxPixelData!')
	void DrawGridOnRast(wxImage& im, const wxRect& r,
		bool do_draw_grid = true, int wid = def_grid_space,
		wxColour bg = wxColour(0xFF, 0xFF, 0xFF),  // backround color
		wxColour rlc = wxColour(0xFF, 0x00, 0x00), // rules color
		wxColour grc = wxColour(0xE0, 0xE0, 0xFF)  // grid color
		);

	// paint visible client area to a wxImage --
	// arg is ** because this method will create or
	// recreate as necessary and assign to caller's
	// variable -- arg logrc recieves logical coords
	// that were drawn to image, and
	// arg thickness is line thickness
	void ImagePaint(
		wxImage** ipp, wxRect& logrc, double thickness = 1.0);
	
	// for drawing background into image, for anti-aliases lines
	// see DrawGridOnRast, OnPaint
	wxImage*		im_main;
	// data re. horz. & vert. guides:
	wxColour     guidecolor;
	wxPen        guidepen;
	// container typedef and members moved in to DataState
	// where they belong.
#	if 0
	typedef std::vector<int>  guidestore;
	guidestore   hguides, vguides; // containers
	int*         hgcur, * vgcur;   // current one
#	endif
	int          guidetol;         // tolerance for guide snapping
	bool         guidesnap;        // do or do not snap
	// Scale for dc
	int          xscale, yscale;
	// Called from on On MouseMove to handle guides as apropriate
	bool         CheckGuide(wxDC& dc, wxMouseEvent& e);
	bool         PickupGuide(wxDC& dc, wxMouseEvent& e);
	bool         GuideSnap(wxPoint& p);
	bool         GuideSnap(wxPoint& p, bool& bx, bool& by);
	bool         GuideSnap(wxRect& r);

	// helpers for tool/menu bars
	void enableExportCurves(bool b);
	void enableExportAsCurves(bool b);
	void enableSaveCurves(bool b);
	void enableSaveAsCurves(bool b);
	void enableOpenCurves(bool b);
	void enableCloseCurves(bool b);
	void enableEdUndo(bool b);
	void enableEdRedo(bool b);
	void enableEdMoveDown(bool b);
	void enableEdMoveUp(bool b);
	void enableEdCopy(bool b);
	void enableEdCopyAll(bool b);
	void enableEdCut(bool b);
	void enableEdDel(bool b);
	void enableEdPaste(bool b);
	void enableEdCopyGlobal(bool b);
	void enableEdCopyAllGlobal(bool b);
	void enableEdCutGlobal(bool b);
	void enableEdPasteGlobal(bool b);
	void enableHelpDemo(bool b);
	void enableRemoveBackgroundImage(bool b);

	void enable_set_linear(bool b)
	{m_pop->Enable(IC_set_linear, b);}
	void enable_set_quadratic(bool b)
	{m_pop->Enable(IC_set_quadratic, b);}
	void enable_set_cubic(bool b)
	{m_pop->Enable(IC_set_cubic, b);}
	void enable_set_bezier(bool b)
	{m_pop->Enable(IC_set_bezier, b);}
	void enable_set_prism(bool b)
	{m_pop->Enable(IC_set_prism, b);}
	void enable_set_lathe(bool b)
	{m_pop->Enable(IC_set_lathe, b);}
	void enable_set_conicsweep(bool b)
	{m_pop->Enable(IC_set_conicsweep, b);}
	void enable_set_linearsweep(bool b)
	{m_pop->Enable(IC_set_linearsweep, b);}
	void enable_export(bool b)
	{m_pop->Enable(IC_export, b); enableExportCurves(b);}
	void enable_exportas(bool b)
	{m_pop->Enable(IC_exportas, b); enableExportAsCurves(b);}
	void enable_save(bool b)
	{m_pop->Enable(IC_save, b); enableSaveCurves(b);}
	void enable_saveas(bool b)
	{m_pop->Enable(IC_saveas, b); enableSaveAsCurves(b);}

	void enable_set_props(bool b)
	{m_sel->Enable(IC_set_props, b);}
	void enable_delete(bool b)
	{m_sel->Enable(IC_delete, b);}
	void enable_move_down(bool b)
	{m_sel->Enable(IC_move_down, b); enableEdMoveDown(b);}
	void enable_move_up(bool b)
	{m_sel->Enable(IC_move_up, b); enableEdMoveUp(b);}

private:
	// handle guide-drag on/off graduated scales
	static void DragCallback(wxWindow* wndCB, wxMouseEvent& e, A_Ruler* r);
	void        DragCallbackHandler(wxMouseEvent& e, A_Ruler* r);

	DECLARE_EVENT_TABLE()
};


// A_Canvas' stack type
template <class C>
counted_stack<C>::~counted_stack()
{
	delete_contained(c);
}

template <class C>
void
counted_stack<C>::flush()
{
	delete_clear_contained(c);
	cnt = 0;
}

template <class C>
void
counted_stack<C>::push(const value_type& v)
{
	if ( cnt > 0 && cnt == sz ) {
		value_type t = c.front();
		c.pop_front();
		delete t;
		--cnt;
	}
	c.push_back(v);
	++cnt;
}

// for drawing with buffer on DC -- necessary e.g. on msw,
// not nec. on gtk, elsewhere unknown -- should mostly work
// in event handler overrides
class A_BUFDCCanvas: public A_Canvas {
private:
	wxBitmap*		bm_main;
	DECLARE_EVENT_TABLE()
protected:
public:
	A_BUFDCCanvas(
		A_Frame* parent, A_Tabpage* realparent, bool aa = true);
	virtual ~A_BUFDCCanvas();

	void PrepareDC(wxDC& dc);
	void OnPaint(wxPaintEvent& event);
	void OnEraseBG(wxEraseEvent& event);
	void OnScroll(wxScrollWinEvent& event);
	void OnSize(wxSizeEvent& event);
};


// Quick  demo
// TODO: put this in proper place
long
DoPovDemo(A_Canvas& canvas, A_Canvas::data_store& lst, wxString* err = 0);

#endif  // _A_CANVAS_H_

