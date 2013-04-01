/*
 ~  Simple spline drawing editor to help produce
 ~  POV include files for spline-based POV objects.
 ~
 ~  The Canvas class, derived from wxWindow's scrolled window.
 ~
 ~  Ed Hynan -- Sat Mar 18 11:15:35 EST 2000
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#if wxCHECK_VERSION(2, 4, 0)
#include <wx/filename.h>
#endif
#include <wx/dcbuffer.h>
#include "wx/bitmap.h"
#include "wx/rawbmp.h"
#endif // WX_PRECOMP

#ifdef __GNUG__
    #pragma implementation
#endif

#ifdef __WXMSW__ // this condition is old, for Symantec C++ 7.[56]
#include <float.h>  // for DBL_MAX
#else
#include <cfloat>
#endif

// auto_ptr . . .
#include <memory>
#include <limits>

// for setting "C" locale in a block
#include "clocnumeric.h"

#include "epspline.h"
#include "a_canvas.h"
#include "a_frame.h"
#include "a_tabpage.h"
#include "a_tabwnd.h"
#include "a_ruler.h"
#include "a_zoomdlg.h"

#include "swap_vals.h"
#include "drawlines.h"
namespace aap = ANTI_ALIAS_LINE_NAMESPACE_NAME;


// Argh!
#if wxCHECK_VERSION(2, 9, 0)
#define WXK_NEXT WXK_PAGEDOWN
#define WXK_PRIOR WXK_PAGEUP
#define wxOPEN wxFD_OPEN
#define wxSAVE wxFD_SAVE
#define wxOVERWRITE_PROMPT wxFD_OVERWRITE_PROMPT
#endif

// A_Canvas

int    A_Canvas::szhandles          = TRANSFORM_HANDLE_SIZE;
int    A_Canvas::scaleconstrainval  = TRANSFORM_SCALE_CONTRAINT;
int    A_Canvas::shearconstrainval  = TRANSFORM_SHEAR_CONTRAINT;
double A_Canvas::rotateconstrainval = TRANSFORM_ROTATE_CONTRAINT;
// SunStudio CC is broken re. templates: See this URL:
// http://technopark02.blogspot.com/2005/05/behavior-of-sun-c-compiler-while.html
#if ! defined(__SUNPRO_CC) || 1
A_Canvas::undo_stack::size_type	A_Canvas::undo_size = 
	A_Canvas::undo_stack::default_size;
#else
A_Canvas::undo_stack::size_type	A_Canvas::undo_size =
	DEFAULT_UNDO_SIZE;
#endif

A_Canvas::datatype     A_Canvas::clip_global;

// Padding and allocators for rasters: the wxBitmap allocator
// is for the buffered canvas, and the wxImage allocator for
// the image used to draw anti-aliased lines; and the padding
// is because in the necessary conversion from wxImage to wxBitmap
// for drawing on a wxDC, there is garbage at the edges (which I
// believe is a wx bug, not mine), and the use of padding moves it
// away from visible area. See OnPaint.
// These are left out of the canvas class def. because it is just noisy
// hackish stuff and the class is too large anyway.
const int bgpad  = 4;
const int bgpad2 = bgpad * 2;
inline wxBitmap* alloc_bitmap(size_t w, size_t h)
{
	return new wxBitmap(w + bgpad2, h + bgpad2);
}
inline wxImage* alloc_image(size_t w, size_t h, bool clear = false)
{
	return new wxImage(w + bgpad2, h + bgpad2, clear);
}

// WX Event Tables
BEGIN_EVENT_TABLE(A_Canvas, wxScrolledWindow)
	EVT_IDLE         		(A_Canvas::OnIdle)
	EVT_PAINT        		(A_Canvas::OnPaint)
	EVT_ERASE_BACKGROUND	(A_Canvas::OnEraseBG)
	EVT_SCROLLWIN    		(A_Canvas::OnScroll)
	EVT_SIZE         		(A_Canvas::OnSize)
	EVT_MOTION       		(A_Canvas::OnMouseMove)
	EVT_ENTER_WINDOW 		(A_Canvas::OnMouseEnter)
	EVT_LEAVE_WINDOW 		(A_Canvas::OnMouseLeave)
	EVT_LEFT_DOWN    		(A_Canvas::OnMouseLDown)
	EVT_LEFT_UP      		(A_Canvas::OnMouseLUp)
	EVT_RIGHT_DOWN   		(A_Canvas::OnMouseRDown)
	EVT_RIGHT_UP     		(A_Canvas::OnMouseRUp)
	EVT_KEY_DOWN     		(A_Canvas::OnKeyDown)
	EVT_KEY_UP       		(A_Canvas::OnKeyUp)
	EVT_MENU_RANGE   		(IC_start, IC_end, A_Canvas::GotPopup)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(A_BUFDCCanvas, A_Canvas)
	EVT_PAINT        		(A_BUFDCCanvas::OnPaint)
	EVT_ERASE_BACKGROUND	(A_BUFDCCanvas::OnEraseBG)
	EVT_SCROLLWIN    		(A_BUFDCCanvas::OnScroll)
	EVT_SIZE         		(A_BUFDCCanvas::OnSize)
END_EVENT_TABLE()

A_BUFDCCanvas::A_BUFDCCanvas(
	A_Frame* parent, A_Tabpage* realparent, bool aa)
	: A_Canvas(parent, realparent, aa)
	, bm_main(0) // allocated just before use
{
	// SetDoubleBuffered(false); because A_BUFDCCanvas does it.
	// Really want the three following platform macros, as found
	// in dcbuffer.h, but I only see SetDoubleBuffered() in gtk/window.h
	// Actually this call should be made unconditionally -- surely
	// it should exist as a NOP where it doesn't apply -- but it is
	// undocumented and I don't know if it is undefined for some
	// platforms.
//#if defined(__WXMAC__) || defined(__WXGTK20__) || defined(__WXDFB__)
//#	ifdef __WXGTK20__
#	if defined(__WXMAC__) || defined(__WXGTK20__) || \
		defined(__WXDFB__) || defined(__WXMSW__)
		// Hope for reports if compilation fails
	SetDoubleBuffered(false);
#	endif

	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

A_BUFDCCanvas::~A_BUFDCCanvas()
{
	delete bm_main;
}

A_Canvas::A_Canvas(A_Frame* parent, A_Tabpage* realparent, bool aa)
	: wxScrolledWindow(realparent)
	, a_frame(parent), a_tabpg(realparent), aa_draw(aa)
	, hrule(0), vrule(0)
	, pt_mousedown(INT_MIN, INT_MIN)
	, creating(false), movingsel(false)
	, movingpoint(false), movingtrans(false), movingtransctr(false)
	, transforming(false), scaling(false)
	, shearing(false), rotating(false)
	, resized(false), in_rulehdlr(false), activetab(false)
	, clip(0)
	, ustack(undo_size), rstack(undo_size)
	, transtype(or_not)
	, scaleconstrain(0), shearconstrain(0), rotateconstrain(0.0)
	, im_main(0) // allocated just before use
	, guidecolor(0xFF, 0, 0x40), guidepen(guidecolor, 1, wxSOLID)
	, hgcur(0), vgcur(0), guidetol(5), guidesnap(true)
	, xscale(100), yscale(100)
{
	D = new DataState;
	arrowcursor = new wxCursor(wxCURSOR_ARROW);
	scalecursor = new wxCursor(wxCURSOR_SIZING);
	shearcursor = new wxCursor(wxCURSOR_SIZENWSE);
	rotatecursor = new wxCursor(wxCURSOR_BULLSEYE);
	SetBackgroundColour(wxColour(255,255,255));
	curcursor = arrowcursor;

	m_pop = new wxMenu(_("Curves"), wxMENU_TEAROFF);
	m_pop->Append(IC_set_linear, _("Set Linear Spline"),
	_("Set next object as a linear spline, or convert selected curve"));
	m_pop->Append(IC_set_quadratic, _("Set Quadratic Spline"),
	_("Set next object as a quadratic spline, or convert selected curve"));
	m_pop->Append(IC_set_cubic, _("Set Cubic Spline"),
	_("Set next object as a cubic spline, or convert selected curve"));
	m_pop->Append(IC_set_bezier, _("Set Bezier Spline"),
	_("Set next object as a bezier spline, or convert selected curve"));
	m_pop->AppendSeparator();
	m_pop->Append(IC_set_prism, _("Set Prism"),
	_("Set next object as a POV prism, or convert selected curve"));
	m_pop->Append(IC_set_lathe, _("Set Lathe"),
	_("Set next object as a POV lathe, or convert selected curve"));
	m_pop->AppendSeparator();
	m_pop->Append(IC_save, _("Save"),
	_("Save objects to current file name"));
	m_pop->Append(IC_saveas, _("Save As"),
	_("Save objects to new file"));
	m_pop->Append(IC_export, _("Export"),
	_("Export objects to current file name"));
	m_pop->Append(IC_exportas, _("Export As"),
	_("Export objects to new file"));

	m_sel = new wxMenu(_("Selection"), wxMENU_TEAROFF);
	m_sel->Append(IC_set_props, _("Set object properties"),
	_("Set Properties such as texture, interior"));
	m_sel->Append(IC_delete, _("Delete object"),
	_("Delete the currently selected object"));
	m_sel->Append(IC_move_down, _("Move object down"),
	_("Move selected object down in stacking order"));
	m_sel->Append(IC_move_up, _("Move object up"),
	_("Move selected object up in stacking order"));
}

A_Canvas::~A_Canvas()
{
	delete im_main;
	delete D;
	delete m_pop;
	delete m_sel;
	delete clip;
	delete scalecursor;
	delete shearcursor;
	delete rotatecursor;
	delete arrowcursor;
}

void
A_Canvas::DragCallback(wxWindow* wndCB, wxMouseEvent& e, A_Ruler* r)
{
	if ( wndCB && r )
		#if defined(__SC__) && !defined(_CPPRTTI)
		((A_Canvas*)wndCB)->DragCallbackHandler(e, r);
		#else
		dynamic_cast<A_Canvas*>(wndCB)->DragCallbackHandler(e, r);
		#endif
}

void
A_Canvas::DragCallbackHandler(wxMouseEvent& e, A_Ruler* r)
{
	// Note: Neither wxX11 or wxMotif deliver an event with
	// the shift key down flag!  Could it be because A_Ruler derives
	// from a wxWindow?  Investigate.

	wxSize sz = r->GetSize();

	// Sentry: In some versions of wx* these events are delivered
	// by the toolkit, in others not.  This lets OnMouse{Enter|Leave}
	// distinguish our events from unreliable ones that wx* might
	// or might not send.
	in_rulehdlr = true;

	do {
		wxMouseEvent me(e);
		if ( r == hrule ) {
			int min = e.Leaving() ? -sz.y : 0;
			if ( (me.m_y = e.m_y - sz.y) < min ) {
				break;
			}
			me.m_x = e.m_x;
		} else if ( r == vrule ) {
			int min = e.Leaving() ? -sz.x : 0;
			if ( (me.m_x = e.m_x - sz.x) < min ) {
				break;
			}
			me.m_y = e.m_y;
		} else {
			fprintf(stderr,"internal error:DragCallbackHandler\n");
			break;
		}

		me.SetEventObject(this);
		#if defined(__WXX11__) || defined(__WXMOTIF__)
		me.m_shiftDown = r->GetShiftDown();
		#endif

		wxEvtHandler* eh = GetEventHandler();
		if ( eh == NULL ) {
			fprintf(stderr, "No wxEvtHandler* eh!\n");
			break;
		}
		if ( me.Leaving() ) {
			me.SetEventType(wxEVT_ENTER_WINDOW);
		} else if ( me.Entering() ) {
			me.SetEventType(wxEVT_LEAVE_WINDOW);
		}
		eh->ProcessEvent(me);
	} while ( 0 );

	in_rulehdlr = false;
}

void
A_Canvas::SetHRule(A_Ruler* p) {
	if ( hrule ) hrule->SetDragHandler(0, 0);
	hrule = p;
	if ( hrule ) hrule->SetDragHandler(DragCallback, this);
}

void
A_Canvas::SetVRule(A_Ruler* p) {
	if ( vrule ) vrule->SetDragHandler(0, 0);
	vrule = p;
	if ( vrule ) vrule->SetDragHandler(DragCallback, this);
}

void
A_Canvas::OnIdle(wxIdleEvent& event)
{
	IdleUpdate();
	event.Skip();
}

void
A_Canvas::IdleUpdate()
{
	if ( activetab == false )
		return;

	SetCursor(*curcursor);

	enable_saveas(D->lst.size() > 0);
	enable_exportas(D->lst.size() > 0);
	enableEdUndo(ustack.count() > 0);
	enableEdRedo(rstack.count() > 0);
	enableCloseCurves(D->sSname != wxT("") || D->lst.size() > 0);

	bool dirty = IsDirty();
#	if 1
	enable_save(dirty);
#	else
	if ( D->sSname != wxT("") || !dirty )
		enable_save(dirty);
#	endif
	// Enable Export() if there's a name and >0 objects.
	// A dirty flag would need to be distinct from the
	// data file's dirty flag, and it's not worth it.
	// No problem overwriting POV include file with no
	// changes.
	enable_export(D->lst.size() > 0 && D->sFname != wxT(""));

	bool b = D->sel != 0;
	enable_set_props(b);
	enable_delete(b);
	enable_move_down(b && D->lst.size() > 1);
	enable_move_up(b && D->lst.size() > 1);
	enableEdCopy(b);
	enableEdCopyGlobal(b);
	//enableEdCopyAll(b);
	//enableEdCopyAllGlobal(b);
	enableEdCut(b);
	enableEdCutGlobal(b);
	enableEdDel(b);

	enableEdPaste(clip != 0);
	enableEdPasteGlobal(clip_global != 0);
	enableHelpDemo(D->lst.size() > 0);

	// hack for rules graduations accuracy: see OnScrollWindow()
	if ( resized ) {
		resized = false;
		int x, y;
		GetViewStart(&x, &y);
		int xs, ys;
		GetScrollPixelsPerUnit(&xs, &ys);
		x *= xs; y *= ys;
		if ( hrule ) hrule->OffsRefresh(x);
		if ( vrule ) vrule->OffsRefresh(y);
	}
}

void
A_Canvas::UpdateStatusBar()
{
	wxString s;
#	if OLD_STATBAR_SET
	s.Printf(_("scale: %u%% x, %u%% y"), xscale, yscale);
	a_frame->SetStatusText(s, 2);
	s = IsDirty() ? _("modified") : _("not modified");
	a_frame->SetStatusText(s, 3);
#	else
	wxString t;
	if ( IsDirty() ) {
		/* TRANSLATORS: this symbol (*) indicates that the
		 * file has changes that have not been saved.  Change
		 * it if it is confusing, but the string must be
		 * very short because it is displayed in a small
		 * status-bar field, with other text. A space, or
		 * something equivalent, is need at the end because
		 * text is appended.
		 */
		s += _("* ");
	}
	/* TRANSLATORS: these two numbers are the scale of
	 * the view on each of the two axes.  Change 'x' and 'y'
	 * if they are confusing, but the strings must be
	 * very short because they are displayed in a small
	 * status-bar field, with other text.
	 */
	t.Printf(_("%u%% x, %u%% y"), xscale, yscale);
	a_frame->SetStatusText(s + t, 2);
#	endif

	if ( D->sel ) {
		s = D->sel->GetDescription();
	} else {
		s = wxT("");
	}

	a_frame->SetStatusText(s, 0);
}

void
A_Canvas::SetDirty(bool b) {
	if ( D )
		D->SetDirty(b);
	UpdateStatusBar();
}

void
A_Canvas::OnSize(wxSizeEvent& event)
{
#	if 0
	wxSize sz = event.GetSize();
	if ( im_main && (sz.GetWidth() != im_main->GetWidth() ||
	     sz.GetHeight() != im_main->GetHeight()) ) {
		delete im_main;
		im_main = 0;
	}
#	else
	if ( im_main ) {
		delete im_main;
		im_main = 0;
	}
#	endif

	resized = true;
	event.Skip();
}

void
A_Canvas::OnScroll(wxScrollWinEvent& event)
{
	int x, y, ux, uy;
	GetScrollPixelsPerUnit(&ux, &uy);
	GetViewStart(&x, &y);
	if ( hrule && event.GetOrientation() == wxHORIZONTAL ) {
		hrule->OffsRefresh(x * ux);
	} else if ( vrule && event.GetOrientation() == wxVERTICAL ) {
		vrule->OffsRefresh(y * uy);
	}

	#if defined(__WXMSW__) || defined(__WXGTK__) || 1
	resized = true;
	#endif

	event.Skip();
}

void
A_Canvas::OnKeyDown(wxKeyEvent& event)
{
	if ( !D->sel ) {
		switch ( event.m_keyCode ) {
			case WXK_NEXT: case WXK_PRIOR:
			case WXK_HOME: case WXK_END:
			case WXK_LEFT: case WXK_UP:
			case WXK_RIGHT: case WXK_DOWN:
				// Force conditional in OnIdle().
				resized = true;
				break;
			default:
				break;
		}

		event.Skip();
		return;
	}

	switch ( event.m_keyCode ) {
		case WXK_NEXT: case WXK_PRIOR:
		case 'N':      case 'P':
		case WXK_LEFT: case WXK_UP: case WXK_RIGHT: case WXK_DOWN:
		case 'H':      case 'K':    case 'L':       case 'J':
			int x, y;
			switch ( event.m_keyCode ) {
				case WXK_NEXT: case 'N':
					x = event.CmdDown() ? page : 0;
					y = event.CmdDown() ? 0 : page;
					break;
				case WXK_PRIOR: case 'P':
					x = event.CmdDown() ? -page : 0;
					y = event.CmdDown() ? 0 : -page;
					break;
				case WXK_LEFT: case 'H':
					y = 0; x = -arrow; break;
				case WXK_UP: case 'K':
					x = 0; y = -arrow; break;
				case WXK_RIGHT: case 'L':
					y = 0; x = arrow; break;
				case WXK_DOWN: case 'J':
				default: // default keeps egcs-2.91.66 quite.
					x = 0; y = arrow; break;
			}

			if ( D->selpt ) {
				if ( movingpoint == false ) {
					PushUndo();
					movingpoint = true;
				}
				D->selpt->x += x;
				D->selpt->y += y;
				Refresh();
			} else if ( D->sel ) {
				if ( movingsel == false ) {
					PushUndo();
					movingsel = true;
				}
				move_spline(*D->sel, x, y);
				Refresh();
			} else
				event.Skip();
			break;
		case WXK_SPACE:
			if ( !event.m_shiftDown && D->sel ) {
				#if USE_wxBufferedDC
				wxClientDC clidc(this);
				PrepareDC(clidc);
				wxBufferedDC dc(&clidc, clidc.GetSize());
				DrawSelBox(dc, D->sel);
				#else
				wxClientDC clidc(this);
				PrepareDC(clidc);
				DrawSelBox(clidc, D->sel);
				#endif
			} else
				event.Skip();
			break;
		default:
			event.Skip();
	}
}

void
A_Canvas::OnKeyUp(wxKeyEvent& event)
{
	switch ( event.m_keyCode ) {
		case WXK_BACK:
		case WXK_DELETE:
			DelSelPt();
			break;
		default:
			event.Skip();
	}
	movingsel = movingpoint = false;
}

void
A_Canvas::DelSelPt()
{
	if ( D->selpt ) {
		PushUndo();
		if ( D->sel->DelPoint(*D->selpt) ) {
			D->selpt = 0;
			Refresh();
		} else
			PopUndo();
	} else if ( D->sel ) {
		PushUndo();
		DelSel();
	}
}

void
A_Canvas::RefreshObjBox(data_type_ptr o, wxDC* dc)
{
	if ( !o ) o = D->sel;
	if ( !o ) return;

	wxRect r(*o->BBox());
	r.x -= 4; r.y -= 4;
	r.width += 8; r.height += 8;

	if ( dc ) {
		r.width += r.x;
		r.height += r.y;
		r.x = dc->LogicalToDeviceX(r.x);
		r.y = dc->LogicalToDeviceY(r.y);
		r.width = dc->LogicalToDeviceX(r.width);
		r.height = dc->LogicalToDeviceY(r.height);
		r.width -= r.x;
		r.height -= r.y;
	}

	FixupRect(r);
	Refresh(true, &r);

	if ( dc ) {
		GetTransCtrRect(r);
		r.x -= 2; r.y -= 2;
		r.width += 4; r.height += 4;
		r.width += r.x;
		r.height += r.y;
		r.x = dc->LogicalToDeviceX(r.x);
		r.y = dc->LogicalToDeviceY(r.y);
		r.width = dc->LogicalToDeviceX(r.width);
		r.height = dc->LogicalToDeviceY(r.height);
		r.width -= r.x;
		r.height -= r.y;
		FixupRect(r);
		Refresh(true, &r);
		if ( transforming && !scaling )
			DrawTransCtr(*dc, true, true, transctrpt);
	}
}

bool
A_Canvas::PickupGuide(wxDC& dc, wxMouseEvent& ev)
{
	int x, y;
	x = dc.DeviceToLogicalX(ev.m_x);
	y = dc.DeviceToLogicalY(ev.m_y);

	guidestore::iterator i, e;

	i = hguides.begin(), e = hguides.end();
	i = std::find(i, e, y);
	if ( i != e ) {
		hgcur = &*i;
		return true;
	}

	i = vguides.begin(), e = vguides.end();
	i = std::find(i, e, x);
	if ( i != e ) {
		vgcur = &*i;
		return true;
	}

	return false;
}

bool
A_Canvas::GuideSnap(wxRect& r)
{
	if ( !guidesnap ) return false;

	// FIXME:
	// This is broken -- doesn't work when snapping corner
	// into guideline intersection.
	// Head's not into it now; fix later.
	wxPoint pnw, pne, psw, pse;
	wxPoint Pnw, Pne, Psw, Pse;
	bool    bnw, bne, bsw, bse;
	bool    xbnw, xbne, xbsw, xbse;
	bool    ybnw, ybne, ybsw, ybse;

	pnw.x = r.x; pnw.y = r.y;
	pne.x = r.x + r.width; pne.y = r.y;
	psw.x = r.x; psw.y = r.y + r.height;
	pse.x = pne.x; pse.y = psw.y;

	Pnw = pnw;
	Pne = pne;
	Psw = psw;
	Pse = pse;

	bnw = GuideSnap(pnw, xbnw, ybnw);
	bne = GuideSnap(pne, xbne, ybne);
	bsw = GuideSnap(psw, xbsw, ybsw);
	bse = GuideSnap(pse, xbse, ybse);

	if ( bnw ) {
		int d1 = std::abs(pnw.x - Pnw.x) + std::abs(pnw.y - Pnw.y);

		if ( bne ) {
			int d0 = std::abs(pne.x - Pne.x)
				+ std::abs(pne.y - Pne.y);

			if ( d0 < d1 ) {
				if ( (xbne&&ybne) && !(xbnw&&ybnw) )
					goto got_bne;
			}
		}
		if ( bsw ) {
			int d0 = std::abs(psw.x - Psw.x)
				+ std::abs(psw.y - Psw.y);

			if ( d0 < d1 ) {
				if ( (xbsw&&ybsw) && !(xbnw&&ybnw) )
					goto got_bsw;
			}
		}
		if ( bse ) {
			int d0 = std::abs(pse.x - Pse.x)
				+ std::abs(pse.y - Pse.y);

			if ( d0 < d1 ) {
				if ( (xbse&&ybse) && !(xbnw&&ybnw) )
					goto got_bse;
			}
		}
		r.x = pnw.x;
		r.y = pnw.y;
	} else if ( bne ) {
		got_bne:
		int d1 = std::abs(pne.x - Pne.x) + std::abs(pne.y - Pne.y);

		if ( bsw ) {
			int d0 = std::abs(psw.x - Psw.x)
				+ std::abs(psw.y - Psw.y);

			if ( d0 < d1 ) {
				if ( (xbsw&&ybsw) && !(xbne&&ybne) )
					goto got_bsw;
			}
		}
		if ( bse ) {
			int d0 = std::abs(pse.x - Pse.x)
				+ std::abs(pse.y - Pse.y);

			if ( d0 < d1 ) {
				if ( (xbse&&ybse) && !(xbne&&ybne) )
					goto got_bse;
			}
		}
		r.x = pne.x - r.width;
		r.y = pne.y;
	} else if ( bsw ) {
		got_bsw:
		int d1 = std::abs(psw.x - Psw.x) + std::abs(psw.y - Psw.y);

		if ( bse ) {
			int d0 = std::abs(pse.x - Pse.x)
				+ std::abs(pse.y - Pse.y);

			if ( d0 < d1 ) {
				if ( (xbse&&ybse) && !(xbsw&&ybsw) )
					goto got_bse;
			}
		}
		r.x = psw.x;
		r.y = psw.y - r.height;
	} else if ( bse ) {
		got_bse:
		r.x = pse.x - r.width;
		r.y = pse.y - r.height;
	} else {
		return false;
	}

	return true;
}

bool
A_Canvas::GuideSnap(wxPoint& p, bool& bx, bool& by)
{
	if ( !guidesnap ) return false;

	int x = p.x, y = p.y;
	bool r = false;
	int diff = guidetol + 1;

	bx = by = false;

	guidestore::iterator i, e;
	i = hguides.begin(); e = hguides.end();
	while ( i != e ) {
		int n = *i++;
		int d = std::abs(n - p.y);

		if ( d < diff ) {
			y = n;
			diff = d;
			r = true;
			by = true;
		}
	}

	diff = guidetol + 1;
	i = vguides.begin(); e = vguides.end();
	while ( i != e ) {
		int n = *i++;
		int d = std::abs(n - p.x);

		if ( d < diff ) {
			x = n;
			diff = d;
			r = true;
			bx = true;
		}
	}

	if ( r ) {
		p.x = x;
		p.y = y;
	}

	return r;
}

bool
A_Canvas::GuideSnap(wxPoint& p)
{
	bool b0, b1;
	return GuideSnap(p, b0, b1);
}

bool
A_Canvas::CheckGuide(wxDC& dc, wxMouseEvent& e)
{
	if ( !hgcur && !vgcur )
		return false;

	wxRect r;
	int n;
	const int pad = 4;

	if ( hgcur ) {
		n = dc.LogicalToDeviceY(*hgcur);
		r.height = std::abs(n - e.m_y) + pad*2;
		r.y = std::min(n, e.m_y) - pad;
		if ( r.y < 0 ) r.y = 0;
		*hgcur = dc.DeviceToLogicalY(e.m_y);

		GetClientSize(&r.width, &n);
		r.x = 0;
		FixupRect(r);
		Refresh(true, &r);
	}
	if ( vgcur ) {
		n = dc.LogicalToDeviceX(*vgcur);
		r.width = std::abs(n - e.m_x) + pad*2;
		r.x = std::min(n, e.m_x) - pad;
		if ( r.x < 0 ) r.x = 0;
		*vgcur = dc.DeviceToLogicalX(e.m_x);

		GetClientSize(&n, &r.height);
		r.y = 0;
		FixupRect(r);
		Refresh(true, &r);
	}

	SetDirty();
	return true;
}

void
A_Canvas::OnMouseEnter(wxMouseEvent& event)
{
	event.Skip();

	if ( !event.m_shiftDown ) return;

	#if USE_wxBufferedDC
	wxClientDC clidc(this);
	PrepareDC(clidc);
	wxBufferedDC dc(&clidc, clidc.GetSize());
	PrepareDC(dc);
	#else
	wxClientDC dc(this);
	PrepareDC(dc);
	#endif

	if ( in_rulehdlr && hrule && hrule->GetMLDown() ) {
		hrule->ClearMLDown();

		int n = dc.DeviceToLogicalY(event.m_y);
		hguides.push_back(n);
		hgcur = &hguides.back();

		wxRect r;
		r.y = n;
		GetClientSize(&r.width, &n);
		r.width = dc.DeviceToLogicalXRel(r.width);
		r.x = dc.DeviceToLogicalX(0);
		r.height = dc.DeviceToLogicalYRel(1);

		Refresh(true, &r);
	}
	if ( in_rulehdlr && vrule && vrule->GetMLDown() ) {
		vrule->ClearMLDown();

		int n = dc.DeviceToLogicalX(event.m_x);
		vguides.push_back(n);
		vgcur = &vguides.back();

		wxRect r;
		r.x = n;
		GetClientSize(&n, &r.height);
		r.height = dc.DeviceToLogicalYRel(r.height);
		r.y = dc.DeviceToLogicalY(0);
		r.width = dc.DeviceToLogicalXRel(1);

		Refresh(true, &r);
	}
}

void
A_Canvas::OnMouseLeave(wxMouseEvent& event)
{
	event.Skip();

	if ( /*in_rulehdlr &&*/ hgcur ) {
		int w, h;
		GetClientSize(&w, &h);
		wxRect r;
		r.x = 0; r.width = w;
		r.y = *hgcur; r.height = guidepen.GetWidth();
		hguides.erase(std::find(hguides.begin(), hguides.end(), *hgcur));
		hgcur = 0;
		FixupRect(r);
		Refresh(true, &r);
	}
	if ( /*in_rulehdlr &&*/ vgcur ) {
		int w, h;
		GetClientSize(&w, &h);
		wxRect r;
		r.x = *vgcur; r.width = guidepen.GetWidth();
		r.y = 0; r.height = h;
		vguides.erase(std::find(vguides.begin(), vguides.end(), *vgcur));
		vgcur = 0;
		FixupRect(r);
		Refresh(true, &r);
	}

	if ( /*in_rulehdlr &&*/ (hgcur || vgcur) ) {
		hgcur = vgcur = 0;
		if ( hrule ) hrule->ClearMLDown();
		if ( vrule ) vrule->ClearMLDown();
	}
}

void
A_Canvas::OnMouseMove(wxMouseEvent& event)
{
	event.Skip();
	wxPoint pos = event.GetPosition();

	if ( hrule ) hrule->DrawTickLine(pos.x);
	if ( vrule ) vrule->DrawTickLine(pos.y);

	#if USE_wxBufferedDC
	wxClientDC clidc(this);
	PrepareDC(clidc);
	wxBufferedDC dc(&clidc, clidc.GetSize());
	PrepareDC(dc);
	#else
	wxClientDC dc(this);
	PrepareDC(dc);
	#endif

	long x = pos.x = dc.DeviceToLogicalX(pos.x);
	long y = pos.y = dc.DeviceToLogicalY(pos.y);

	wxString str;
	str.Printf(_("x,y: %ld, %ld"), x, y);
	a_frame->SetStatusText(str, 1);
 
	if ( CheckGuide(dc, event) ) {
		return;
	}
	
	bool snap = event.ShiftDown();

	if ( !movingsel && snap )
		GuideSnap(pos);

	if ( event.LeftIsDown() && event.Dragging() ) {
		if ( transforming ) {  // && transtype != or_not
			if ( transtype == or_not ) {
				if ( !scaling &&
					(TestTransCtr(pos)||movingtransctr) ) {
					movingtransctr = true;
					DrawTransCtr(dc, true, true, pos);
				}
				return;
			}

			if ( !movingtrans ) {
				PushUndo();
				if ( event.CmdDown() ) {
					D->sel = CopyObj(D->sel);
					D->lst.push_front(D->sel);
				}
			}
			movingtrans = true;
			transpt = pos;
			wxRect r0(*D->sel->BBox());
			if ( DoTransform(dc, event.ShiftDown()) ) {
				D->sel->CalcBBox();
				wxRect r(*D->sel->BBox());
				int W = std::max(r0.x+r0.width,r.x+r.width);
				int H = std::max(r0.y+r0.height,r.y+r.height);
				r.x = std::min(r0.x,r.x);
				r.y = std::min(r0.y,r.y);
				r.x -= szhandles; r.y -= szhandles;
				r.x = dc.LogicalToDeviceX(r.x);
				r.y = dc.LogicalToDeviceY(r.y);
				W += szhandles*2; H += szhandles*2;
				W = dc.LogicalToDeviceX(W);
				H = dc.LogicalToDeviceY(H);
				r.width = W - r.x;
				r.height = H - r.y;
				FixupRect(r);
				Refresh(true, &r);
			}
			pt_mousedown = transpt;
			return;
		} else if ( D->selpt && D->sel ) {
			if ( !movingpoint ) {
				PushUndo();
			}
			movingpoint = true;

			bool ol = D->sel->SetDotLock(!event.CmdDown());

			D->sel->MovePoint(*D->selpt
				, pos.x - D->selpt->x
				, pos.y - D->selpt->y);
			D->sel->SetDotLock(ol);

			wxRect r(*D->sel->BBox());
			r.x -= 4; r.y -= 4;
			r.width += 8; r.height += 8;
			r.x = dc.LogicalToDeviceX(r.x);
			r.y = dc.LogicalToDeviceY(r.y);
			r.width = dc.LogicalToDeviceXRel(r.width);
			r.height = dc.LogicalToDeviceYRel(r.height);
			FixupRect(r);
			Refresh(true, &r);
		} else if ( D->sel ) {
			if ( !movingsel ) {
				PushUndo();
				if ( event.CmdDown() ) {
					D->sel = CopyObj(D->sel);
					D->lst.push_front(D->sel);
				}
			}
			movingsel = true;

			int dx = pos.x - pt_mousedown.x;
			int dy = pos.y - pt_mousedown.y;
			wxRect r(*D->sel->BBox());
			wxRect r2(r);

			if ( snap && GuideSnap(r2) ) {
				if ( r2.x != r.x ) {
					dx = r2.x - r.x;
					pos.x = pt_mousedown.x + dx;
				}
				if ( r2.y != r.y ) {
					dy = r2.y - r.y;
					pos.y = pt_mousedown.y + dy;
				}
			}

			move_spline(*D->sel, dx, dy);
			r2 = *D->sel->BBox();

			wxRect r3(r.Union(r2));
			r3.Inflate(int(refresh_pad), int(refresh_pad));
			FixupRect(r3);

			r3.width = dc.LogicalToDeviceX(r3.x + r3.width);
			r3.x = dc.LogicalToDeviceX(r3.x);
			r3.width -= r3.x;
			r3.height = dc.LogicalToDeviceY(r3.y + r3.height);
			r3.y = dc.LogicalToDeviceY(r3.y);
			r3.height -= r3.y;

			Refresh(true, &r3);
		}
	}

	pt_mousedown = pos;
}

void
A_Canvas::OnMouseLUp(wxMouseEvent& event)
{
	event.Skip();

	#if USE_wxBufferedDC
	wxClientDC clidc(this);
	PrepareDC(clidc);
	wxBufferedDC dc(&clidc, clidc.GetSize());
	PrepareDC(dc);
	#else
	wxClientDC dc(this);
	PrepareDC(dc);
	#endif

	if ( transforming ) {
		RefreshObjBox(D->sel, &dc);
	}
	if ( !event.m_shiftDown && D->sel ) {
		DrawSelBox(dc, D->sel);
	}

	movingsel = movingpoint = movingtrans = movingtransctr = false;
	transtype = or_not;
	scaleconstrain = shearconstrain = 0;
	rotateconstrain = 0.0;

	if ( true || in_rulehdlr ) {
		hgcur = vgcur = 0;
		if ( hrule ) hrule->ClearMLDown();
		if ( vrule ) vrule->ClearMLDown();
	}
}


/*
 * Mouse left down behavior:
 * -- shift key down:
 *    -- if currently creating a curve, finish (stop creating)
 *    -- not currently creating a curve:
 *       -- if an object is selected, add point to selected curve, else
 *       -- no object selected, then begin creating new curve
 * -- shift not down:
 *    -- an object is already selected:
 *       -- mouse point in object's bbox:
 *          -- point lies near control point, then select ctrl point
 *          -- else deselect selected point; movement will move object.
 *       -- mouse point not in bbox: deselect selected object
 *    -- no object select, then find first object with bbox
 *       containing mouse point; if found, select object
 */
void
A_Canvas::OnMouseLDown(wxMouseEvent& event)
{
	event.Skip();

	#if USE_wxBufferedDC
	wxClientDC clidc(this);
	PrepareDC(clidc);
	wxBufferedDC dc(&clidc, clidc.GetSize());
	PrepareDC(dc);
	#else
	wxClientDC dc(this);
	PrepareDC(dc);
	#endif

	if ( !event.m_shiftDown && !creating && PickupGuide(dc, event) ) {
		 return;
	}

	wxPoint pos = event.GetPosition();
	long x = dc.DeviceToLogicalX( pos.x );
	long y = dc.DeviceToLogicalY( pos.y );
	pos.x = x; pos.y = y;
	pt_mousedown.x = x;
	pt_mousedown.y = y;

	if ( transforming ) {
		transtype = PointOnRect(*D->sel->BBox(), pt_mousedown);
		if ( transtype == or_not )
			transtype = TestTransformHandles(D->sel, pt_mousedown);
		return;
	}

	if ( event.m_shiftDown ) { // start/end curve edit
		if ( creating ) {
			if ( D->cur && !D->cur->Okay() ) {
				PopUndo();
				Refresh();
			} else
				D->cur = 0;
			creating = false;
		} else {
			if ( D->sel ) {
				PushUndo();
				if ( D->sel->AddPoint(pt_mousedown) ) {
					SplineBase::iterator i =
					    D->sel->PointIndexIt(pt_mousedown);
					D->selpt = (i==D->sel->end()) ? 0 : &*i;
					Refresh();
				} else {
					//PopUndo(); // old, wrong, prevents new sub-curve
					D->cur = D->sel;
					D->cur->push_back(pt_mousedown);
					creating = true;
				}
			} else {
				PushUndo();
				D->cur = D->NewObj();
				D->lst.push_front(D->cur);
				D->cur->push_back(pt_mousedown);
				creating = true;
			}
		}
	} else { // ! event.m_shiftDown
		if ( creating && D->cur ) { // editing curve, new point
			D->cur->push_back(pt_mousedown);
		} else if ( D->sel ) { // (de)select
			if ( D->sel->PtInRect(pt_mousedown, refresh_pad) ) {
				// (de)select point
				SplineBase::iterator i =
					D->sel->PointIndexIt(pt_mousedown);
				if ( i != D->sel->end() )
					D->selpt = &*i;
				else
					D->selpt = 0;
				// object should describe itself with wxString
				wxString s = D->sel->GetDescription();
				if ( s != wxT("") ) {
					a_frame->SetStatusText(s, 0);
				}
			} else { // deselect whole curve
				a_frame->SetStatusText(wxT(""), 0);
				D->sel = 0;
				D->selpt = 0;
				Refresh();
			}
		} else { // select curve
			std::list<SplineBase*>::iterator i =
				find_if(D->lst.begin(),
					D->lst.end(),
					PtInBBox(pt_mousedown));
			D->sel = (i==D->lst.end()) ? 0 : *i;
			// if object selected, put description is status bar
			if ( D->sel ) {
				// object should describe itself with wxString
				wxString s = D->sel->GetDescription();
				if ( s != wxT("") ) {
					a_frame->SetStatusText(s, 0);
				}
			}
		}

		if ( D->sel )
			RefreshObjBox(D->sel, &dc);
		if ( D->cur ) {
			D->cur->CalcBBox();
			RefreshObjBox(D->cur, &dc);
		}
	}

	//DrawCur(dc);
	DrawCur();
}

bool
A_Canvas::DoTransform(wxDC& dc, bool constrain)
{
	const wxRect& r = *D->sel->BBox();

	if ( scaling ) {
		int xs = transpt.x - pt_mousedown.x
		  , ys = transpt.y - pt_mousedown.y;

		switch ( transtype ) {
		case or_w:
			if ( constrain ) {
				scaleconstrain += xs;
				if ( scaleconstrain >= scaleconstrainval ) {
					int tx = r.x + r.width;
					D->sel->Flip(transtype);
					move_spline(*D->sel, -r.width, 0);
					transtype = or_e;
					scaleconstrain = 0;
					transpt.x = tx;
					WarpPointer(
						dc.LogicalToDeviceX(transpt.x),
						dc.LogicalToDeviceY(transpt.y));
					return true;
				}
				return false;
			} else if ( xs >= r.width ) {
				D->sel->Flip(transtype);
				transtype = or_e;
				xs -= r.width;
				transpt.x += xs;
				WarpPointer(
					dc.LogicalToDeviceX(transpt.x),
					dc.LogicalToDeviceY(transpt.y));
			}
			break;
		case or_ws:
			if ( constrain ) {
				scaleconstrain += (xs + -ys) / 2;
				if ( scaleconstrain >= scaleconstrainval ) {
					int tx = r.x + r.width;
					int ty = r.y;
					D->sel->Flip(transtype);
					move_spline(*D->sel
						, -r.width, r.height);
					transtype = or_en;
					scaleconstrain = 0;
					transpt.x = tx;
					transpt.y = ty;
					WarpPointer(
						dc.LogicalToDeviceX(transpt.x),
						dc.LogicalToDeviceY(transpt.y));
					return true;
				}
				return false;
			} else if ( xs >= r.width || -ys >= r.height ) {
				if ( xs >= r.width && ys >= r.height ) {
					D->sel->Flip(transtype);
					transtype = or_en;
					xs -= r.width;
					ys += r.height;
					transpt.x += xs;
					transpt.y += ys;
				} else if ( xs >= r.width ) {
					D->sel->Flip(or_w);
					transtype = or_se;
					xs -= r.width;
					transpt.x += xs;
				} else if ( -ys >= r.height ) {
					D->sel->Flip(or_s);
					transtype = or_nw;
					ys += r.height;
					transpt.y += ys;
				}
				WarpPointer(
					dc.LogicalToDeviceX(transpt.x),
					dc.LogicalToDeviceY(transpt.y));
			}
			break;
		case or_s:
			if ( constrain ) {
				scaleconstrain += -ys;
				if ( scaleconstrain >= scaleconstrainval ) {
					int ty = r.y;
					D->sel->Flip(transtype);
					move_spline(*D->sel, 0, r.height);
					transtype = or_n;
					scaleconstrain = 0;
					transpt.y = ty;
					WarpPointer(
						dc.LogicalToDeviceX(transpt.x),
						dc.LogicalToDeviceY(transpt.y));
					return true;
				}
				return false;
			} else if ( -ys >= r.height ) {
				D->sel->Flip(or_s);
				transtype = or_n;
				ys += r.height;
				transpt.y += ys;
				WarpPointer(
					dc.LogicalToDeviceX(transpt.x),
					dc.LogicalToDeviceY(transpt.y));
			}
			break;
		case or_en:
			if ( constrain ) {
				scaleconstrain += (-xs + ys) / 2;
				if ( scaleconstrain >= scaleconstrainval ) {
					int tx = r.x;
					int ty = r.y + r.height;
					D->sel->Flip(transtype);
					move_spline(*D->sel
						, r.width, -r.height);
					transtype = or_ws;
					scaleconstrain = 0;
					transpt.x = tx;
					transpt.y = ty;
					WarpPointer(
						dc.LogicalToDeviceX(transpt.x),
						dc.LogicalToDeviceY(transpt.y));
					return true;
				}
				return false;
			} else if ( -xs >= r.width || ys >= r.height ) {
				if ( -xs >= r.width && ys >= r.height ) {
					D->sel->Flip(transtype);
					transtype = or_ws;
					xs += r.width;
					ys -= r.height;
					transpt.x += xs;
					transpt.y += ys;
				} else if ( -xs >= r.width ) {
					D->sel->Flip(or_e);
					transtype = or_nw;
					xs += r.width;
					transpt.x += xs;
				} else if ( ys >= r.height ) {
					D->sel->Flip(or_n);
					transtype = or_se;
					ys -= r.height;
					transpt.y += ys;
				}
				WarpPointer(
					dc.LogicalToDeviceX(transpt.x),
					dc.LogicalToDeviceY(transpt.y));
			}
			break;
		case or_e:
			if ( constrain ) {
				scaleconstrain += -xs;
				if ( scaleconstrain >= scaleconstrainval ) {
					int tx = r.x;
					D->sel->Flip(transtype);
					move_spline(*D->sel, r.width, 0);
					transtype = or_w;
					scaleconstrain = 0;
					transpt.x = tx;
					WarpPointer(
						dc.LogicalToDeviceX(transpt.x),
						dc.LogicalToDeviceY(transpt.y));
					return true;
				}
				return false;
			} else if ( -xs >= r.width ) {
				D->sel->Flip(transtype);
				transtype = or_w;
				xs += r.width;
				transpt.x += xs;
				WarpPointer(
					dc.LogicalToDeviceX(transpt.x),
					dc.LogicalToDeviceY(transpt.y));
			}
			break;
		case or_se:
			if ( constrain ) {
				scaleconstrain += (-xs + -ys) / 2;
				if ( scaleconstrain >= scaleconstrainval ) {
					int tx = r.x;
					int ty = r.y;
					D->sel->Flip(transtype);
					move_spline(*D->sel, r.width, r.height);
					transtype = or_nw;
					scaleconstrain = 0;
					transpt.x = tx;
					transpt.y = ty;
					WarpPointer(
						dc.LogicalToDeviceX(transpt.x),
						dc.LogicalToDeviceY(transpt.y));
					return true;
				}
				return false;
			} else if ( -xs >= r.width || -ys >= r.height ) {
				if ( -xs >= r.width && -ys >= r.height ) {
					D->sel->Flip(transtype);
					transtype = or_nw;
					xs += r.width;
					ys += r.height;
					transpt.x += xs;
					transpt.y += ys;
				} else if ( -xs >= r.width ) {
					D->sel->Flip(or_e);
					transtype = or_ws;
					xs += r.width;
					transpt.x += xs;
				} else if ( -ys >= r.height ) {
					D->sel->Flip(or_s);
					transtype = or_en;
					ys += r.height;
					transpt.y += ys;
				}
				WarpPointer(
					dc.LogicalToDeviceX(transpt.x),
					dc.LogicalToDeviceY(transpt.y));
			}
			break;
		case or_n:
			if ( constrain ) {
				scaleconstrain += ys;
				if ( scaleconstrain >= scaleconstrainval ) {
					int ty = r.y + r.height;
					D->sel->Flip(transtype);
					move_spline(*D->sel, 0, -r.height);
					transtype = or_s;
					scaleconstrain = 0;
					transpt.y = ty;
					WarpPointer(
						dc.LogicalToDeviceX(transpt.x),
						dc.LogicalToDeviceY(transpt.y));
					return true;
				}
				return false;
			} else if ( ys >= r.height ) {
				D->sel->Flip(or_n);
				transtype = or_s;
				ys -= r.height;
				transpt.y += ys;
				WarpPointer(
					dc.LogicalToDeviceX(transpt.x),
					dc.LogicalToDeviceY(transpt.y));
			}
			break;
		case or_nw:
			if ( constrain ) {
				scaleconstrain += (xs + ys) / 2;
				if ( scaleconstrain >= scaleconstrainval ) {
					int tx = r.x + r.width;
					int ty = r.y + r.height;
					D->sel->Flip(transtype);
					move_spline(*D->sel
						, -r.width, -r.height);
					transtype = or_se;
					scaleconstrain = 0;
					transpt.x = tx;
					transpt.y = ty;
					WarpPointer(
						dc.LogicalToDeviceX(transpt.x),
						dc.LogicalToDeviceY(transpt.y));
					return true;
				}
				return false;
			} else if ( xs >= r.width || ys >= r.height ) {
				if ( xs >= r.width && ys >= r.height ) {
					D->sel->Flip(transtype);
					transtype = or_se;
					xs -= r.width;
					ys -= r.height;
					transpt.x += xs;
					transpt.y += ys;
				} else if ( xs >= r.width ) {
					D->sel->Flip(or_w);
					transtype = or_en;
					xs -= r.width;
					transpt.x += xs;
				} else if ( ys >= r.height ) {
					D->sel->Flip(or_n);
					transtype = or_ws;
					ys -= r.height;
					transpt.y += ys;
				}
				WarpPointer(
					dc.LogicalToDeviceX(transpt.x),
					dc.LogicalToDeviceY(transpt.y));
			}
			break;
		default: break;
		}

		D->sel->Scale(transtype, xs, ys);
		return true;
	} else if ( shearing ) {
		wxPoint ctr = transctrpt;
		if ( constrain ) {
			int xs = transpt.x - pt_mousedown.x
			  , ys = transpt.y - pt_mousedown.y;

			switch ( transtype ) {
				case or_n: case or_en:
				case or_s: case or_ws:
					shearconstrain += xs;
					break;
				case or_w: case or_nw:
				case or_e: case or_se:
					shearconstrain += ys;
					break;
				default:
					break;
			}

			if ( std::abs(shearconstrain) >= shearconstrainval ) {
				switch ( transtype ) {
					case or_n: case or_en:
					case or_s: case or_ws:
						shearconstrain -= xs;
						xs = (xs >= 0) ?
							shearconstrainval :
							-shearconstrainval;
						transpt.x = pt_mousedown.x + xs;
						break;
					case or_w: case or_nw:
					case or_e: case or_se:
						shearconstrain -= ys;
						ys = (ys >= 0) ?
							shearconstrainval :
							-shearconstrainval;
						transpt.y = pt_mousedown.y + ys;
						break;
					default:
						break;
				}
				D->sel->Shear(transtype
					, pt_mousedown, transpt, ctr);
				return true;
			}
			return false;
		}
		D->sel->Shear(transtype, pt_mousedown, transpt, ctr);
		return true;
	} else if ( rotating ) {
		wxPoint ctr = transctrpt;
		if ( constrain && transtype != or_not ) {
			double A = FindAngle(pt_mousedown, transpt, ctr);
			rotateconstrain += A;

			if ( std::abs(rotateconstrain) >= rotateconstrainval ) {
				bool neg = (rotateconstrain < 0.0);
				A = neg ?  -rotateconstrainval :
					rotateconstrainval;
				rotateconstrain -= A;
				wxRealPoint c(ctr.x, ctr.y);
				D->sel->Rotate(A, c);
				return true;
			}

			return false;
		}
		D->sel->Rotate(transtype, pt_mousedown, transpt, ctr);
		return true;
	}

	return false;
}

bool
A_Canvas::TestTransCtr(wxPoint pt)
{
	wxRect r;
	GetTransCtrRect(r);
	return InRect(r, pt);
}

void
A_Canvas::GetTransCtrRect(wxRect& r)
{
	r.x = transctrpt.x - szhandles;
	r.y = transctrpt.y - szhandles;
	r.width = r.height = szhandles*2;
}

void
A_Canvas::DrawTransCtr(wxDC& dc, bool clear, bool draw, wxPoint pt)
{
#	if wxCHECK_VERSION(2, 9, 0)
	wxRasterOperationMode
#	else
	int
#	endif
		of = dc.GetLogicalFunction();
	wxPen op = dc.GetPen();
	wxBrush ob = dc.GetBrush();
	wxBrush br(wxColour(0,0,0), wxTRANSPARENT);

	dc.SetLogicalFunction(wxINVERT);
	dc.SetPen(*wxGREY_PEN);
	dc.SetBrush(br);

	if ( clear ) {
		dc.DrawCircle(transctrpt.x, transctrpt.y, szhandles);
		dc.DrawCircle(transctrpt.x, transctrpt.y, szhandles/2);
	}
	if ( draw ) {
		dc.DrawCircle(pt.x, pt.y, szhandles);
		dc.DrawCircle(pt.x, pt.y, szhandles/2);
		transctrpt = pt;
	}

	dc.SetLogicalFunction(of);
	dc.SetPen(op);
	dc.SetBrush(ob);
}

void
A_Canvas::OnMouseRUp(wxMouseEvent& event)
{
	event.Skip();
}

void
A_Canvas::OnMouseRDown(wxMouseEvent& event)
{
	event.Skip();

	#if USE_wxBufferedDC
	wxClientDC clidc(this);
	PrepareDC(clidc);
	wxBufferedDC dc(&clidc, clidc.GetSize());
	PrepareDC(dc);
	#else
	wxClientDC dc(this);
	PrepareDC(dc);
	#endif

	if ( transforming && !event.m_shiftDown ) {
		transforming = scaling = shearing = rotating = false;
		curcursor = arrowcursor;
		D->sel->toggle_close();
		RefreshObjBox(D->sel, &dc);
	} else if ( event.m_shiftDown ) {
		wxPoint p;
		p.x = dc.DeviceToLogicalX(event.m_x);
		p.y = dc.DeviceToLogicalY(event.m_y);

		if ( D->sel && D->sel->PtInRect(p) ) {
			bool clear, draw;
			clear = draw = false;

			if ( !transforming ) {
				D->sel->toggle_close();
				const wxRect& r = *D->sel->BBox();
				transctrpt.x = r.x + r.width / 2;
				transctrpt.y = r.y + r.height / 2;
				transforming = scaling = true;
				curcursor = scalecursor;
			} else if ( scaling ) {
				scaling = false;
				shearing = true;
				draw = true;
				curcursor = shearcursor;
			} else if ( shearing ) {
				shearing = false;
				rotating = true;
				draw = true;
				clear = true;
				curcursor = rotatecursor;
			} else if ( rotating ) {
				rotating = false;
				scaling = true;
				clear = true;
				curcursor = scalecursor;
			}

			DrawSelBox(dc, D->sel);
			if ( clear || draw )
				DrawTransCtr(dc, clear, draw, transctrpt);
		}
	} else {
		DoRClickMenu(event.m_x, event.m_y);
	}
}

void
A_Canvas::DoRClickMenu(int x, int y)
{
	PopupMenu(D->sel ? m_sel : m_pop, x, y);
}

void
A_Canvas::DataState::GotPopup(wxCommandEvent& event)
{
	switch ( event.GetId() ) {
		case IC_set_linear:
			splinet = linear;
			UpdateObj(cur);
			break;
		case IC_set_quadratic:
			splinet = quadratic;
			UpdateObj(cur);
			break;
		case IC_set_cubic:
			splinet = cubic;
			UpdateObj(cur);
			break;
		case IC_set_bezier:
			splinet = bezier;
			UpdateObj(cur);
			break;
		case IC_set_prism:
			objt = prism;
			UpdateObj(cur);
			break;
		case IC_set_lathe:
			objt = lathe;
			UpdateObj(cur);
			break;
		case IC_set_conicsweep:
			sweept = conic;
			UpdateObj(cur);
			break;
		case IC_set_linearsweep:
			sweept = slinear;
			UpdateObj(cur);
			break;
	}
}

void
A_Canvas::GotPopup(wxCommandEvent& event)
{
	switch ( event.GetId() ) {
		case IC_export:
			Export();
			break;
		case IC_exportas:
			ExportAs();
			break;
		case IC_save:
			Save();
			break;
		case IC_saveas:
			SaveAs();
			break;
		case IC_set_props:
			if ( D->sel ) {
				DataState* t = new DataState(*D);
				if ( SetSplineProps(t->sel, m_parent) ==
					wxID_OK ) {
					ustack.push(D);
					D = t;
					SetDirty();
				} else
					delete t;
				Refresh();
			}
			break;
		case IC_delete:
			if ( D->sel ) {
				PushUndo();
				DelSel();
				Refresh();
			}
			break;
		case IC_move_down:
			MoveDown();
			break;
		case IC_move_up:
			MoveUp();
			break;
		default:
			D->GotPopup(event);
	}
}

void
A_Canvas::PopUndo()
{
	if ( ustack.count() > 0 ) {
		if ( D ) delete D;
		D = ustack.popv();
		if ( ustack.count() == 0 ) {
			UpdateStatusBar();
		}
	}
}

void
A_Canvas::PushUndo()
{
	if ( !D ) return;

	DataState* t = new DataState(*D);
	ustack.push(D);
	D = t;
	SetDirty();  // PushUndo implies a change.
}

void
A_Canvas::Undo()
{
	bool wasdirty = IsDirty();
	DataState* t = ustack.popv();
	if ( t ) {
		rstack.push(D);
		D = t;
		Refresh();
		if ( !wasdirty )
			SetDirty();
		UpdateStatusBar();
	}
}

void
A_Canvas::Redo()
{
	bool wasdirty = IsDirty();
	DataState* t = rstack.popv();
	if ( t ) {
		ustack.push(D);
		D = t;
		Refresh();
		if ( !wasdirty )
			SetDirty();
		UpdateStatusBar();
	}
}

void
A_Canvas::MoveDown()
{
	if ( !D || !D->sel || D->lst.size() < 2 ) return;

	itr_t i, j, k;
	i = D->lst.begin();
	j = D->lst.end();
	k = find(i, j, D->sel);

	if ( k == j ) return;
	PushUndo();

	// Redo, as D now points to new DataState object!
	i = D->lst.begin();
	j = D->lst.end();
	k = find(i, j, D->sel);

	if ( k == j ) return;

	if ( k == --j ) {
		D->lst.pop_back();
		D->lst.push_front(D->sel);
	} else {
		j = k;
		++j;
		*k = *j;
		*j = D->sel;
	}

	Refresh();
}

void
A_Canvas::MoveUp()
{
	if ( !D || !D->sel || D->lst.size() < 2 ) return;

	itr_t i, j, k;
	i = D->lst.begin();
	j = D->lst.end();
	k = find(i, j, D->sel);

	if ( k == j ) return;
	PushUndo();

	// Redo, as D now points to new DataState object!
	i = D->lst.begin();
	j = D->lst.end();
	k = find(i, j, D->sel);

	if ( k == j ) return;

	if ( k == i ) {
		D->lst.pop_front();
		D->lst.push_back(D->sel);
	} else {
		j = k;
		--j;
		*k = *j;
		*j = D->sel;
	}

	Refresh();
}

void
A_Canvas::clipCopy()
{
	if ( !D || !D->sel ) return;

	delete clip;
	clip = CopyObj(D->sel);
}

void
A_Canvas::clipCopyGlobal()
{
	if ( !D || !D->sel ) return;

	delete clip_global;
	clip_global = CopyObj(D->sel);
}

void
A_Canvas::clipCutGlobal()
{
	if ( !D || !D->sel ) return;

	PushUndo();
	clipCopyGlobal();
	DelSel();
}

void
A_Canvas::clipCut()
{
	if ( !D || !D->sel ) return;

	PushUndo();
	clipCopy();
	DelSel();
}

void
A_Canvas::clipPaste()
{
	if ( !D || !clip ) return;

	PushUndo();
	D->selpt = 0;
	D->sel = CopyObj(clip);
	D->lst.push_front(D->sel);
	Refresh();
	UpdateStatusBar();
}

void
A_Canvas::clipPasteGlobal()
{
	if ( !D || !clip_global ) return;

	PushUndo();
	D->selpt = 0;
	D->sel = CopyObj(clip_global);
	D->lst.push_front(D->sel);
	Refresh();
	UpdateStatusBar();
}

void
A_Canvas::DelSel()
{
	if ( !D || !D->sel ) return;
	std::list<SplineBase*>::iterator i =
		find(D->lst.begin(), D->lst.end(), D->sel);
	if ( i != D->lst.end() )
		D->lst.erase(i);
	delete D->sel;
	D->selpt = 0;
	D->sel = 0;
	Refresh();
	UpdateStatusBar();
}

void
A_Canvas::DoSetScale()
{
#if !defined(DEBUG) && defined(__WXGTK__)
	// GTK2 getting X error with anything < 50
	const int rmin = 50;
#else
	const int rmin = 20;
#endif
	const int rmax = 1000;

	A_Zoomdlg zd(a_frame, -1);
	zd.SetRangeX(rmin, rmax);
	zd.SetRangeY(rmin, rmax);
	zd.SetValueX(xscale);
	zd.SetValueY(yscale);
	zd.Centre(wxBOTH);
	int r = zd.ShowModal();
	if ( r == wxID_OK ) {
		xscale = std::max(std::min(zd.GetValueX(), rmax), rmin);
		yscale = std::max(std::min(zd.GetValueY(), rmax), rmin);
		if ( hrule )
			hrule->SetScalePercent(xscale);
		if ( vrule )
			vrule->SetScalePercent(yscale);
		Refresh();
		UpdateStatusBar();
	}
}

void
A_Canvas::DoCycleScale()
{
	// there are two independent variables;
	// work with Y and make X match
	if ( yscale < 100 ) {
		yscale = 100;
	} else if ( yscale == 100 ) {
		yscale = 200;
	} else {
		yscale = 50;
	}
	
	xscale = yscale;

	if ( hrule )
		hrule->SetScalePercent(xscale);
	if ( vrule )
		vrule->SetScalePercent(yscale);
	Refresh();
	UpdateStatusBar();
}

bool
A_Canvas::CloseOpt(bool force)
{
	if ( IsDirty() && force == false ) {
		wxString msg = _("There are unsaved changes.  ");
		msg = msg + _("Do you really want to close this data?");
		wxString ttl = wxGetApp().GetAppTitle() + _(": unsaved changes");
		int rep = ::wxMessageBox(msg, ttl
			, wxCENTRE | wxICON_QUESTION | wxYES_NO
			, m_parent);

		if ( rep != wxYES )
			return false;
	}

	DataState* t = D;
	D = new DataState;
	delete t;
	ustack.flush();
	rstack.flush();
	clear(hguides);
	clear(vguides);
#if wxCHECK_VERSION(2, 8, 0)
	m_parent->SetLabel(wxGetApp().GetAppTitle());
#else
	m_parent->SetTitle(wxGetApp().GetAppTitle());
#endif
	Refresh();
	return true;
}

void
A_Canvas::Close()
{
	CloseOpt();
}

void
A_Canvas::Open()
{
	if ( IsDirty() ) {
		wxString msg = _("There are unsaved changes.  ");
		msg = msg + _("Do you really want to open a new file?");
		wxString ttl = wxGetApp().GetAppTitle() + _(": unsaved changes");
		int rep = ::wxMessageBox(msg, ttl
			, wxCENTRE | wxICON_QUESTION | wxYES_NO
			, m_parent);

		if ( rep != wxYES )
			return;
	}

	if ( D->sSdir == wxT("") ) {
		D->sSdir = wxGetApp().GetLastDlgDir();
	}
	
	wxFileDialog d(a_frame, _("Please select a file")
		, D->sSdir, D->sSname
		, wxT("*.pse"), wxOPEN);

	if ( d.ShowModal() == wxID_OK ) {
		wxString td = d.GetDirectory();
		wxString tn = d.GetFilename();
		if ( td != wxT("") ) {
#if wxCHECK_VERSION(2, 4, 0)
			wxFileName fn(td, tn);
			tn = fn.GetFullPath();
#else
			tn = td + SEPC + tn;
#endif
		}

		Open(tn);
	}
}

void
A_Canvas::Open(wxString filename)
{
	if ( filename != wxT("") ) {
#if wxCHECK_VERSION(2, 4, 0)
		wxFileName fn(filename);
		wxString tn(fn.GetFullName());
		wxString td(fn.GetPath());
#else
		wxString tn(::wxFileNameFromPath(filename));
		wxString td(::wxPathOnly(filename));
#endif

		DataState* tmp = new DataState;
		guidestore th, tv;
		if ( ReadData(filename, tmp->lst, th, tv) > 0 ) {
			DataState* t2 = D;
			D = tmp;
			tmp = t2;
			D->sSdir = td;
			D->sSname = tn;
			wxGetApp().SetLastDlgDir(td);
			ustack.flush();
			rstack.flush();
			clear(hguides); clear(vguides);
			copy(th.begin(), th.end(), back_inserter(hguides));
			copy(tv.begin(), tv.end(), back_inserter(vguides));
			a_frame->SetTitlePrefix(tn);
			Refresh();
		} else {
			wxString msg;

			msg.Printf(_("Failed reading %s"), filename.c_str());
			ErrorBox(msg);
		}

		// If read succeeded, this was swapped with the old D
		delete tmp;
	}
}

bool
A_Canvas::SaveBGImage(const wxImage* p) const
{
	// ensure destruction iff allocated here.
	// NOTE auto_ptr deprecated in C++11
	std::auto_ptr<const wxImage> ap;

	// If im_main is used there are no decorations, e.g.
	// dots at control points -- which was wanted for debugging
	// -- generally the the decorations should be included,
	// so disable use of im_main (unless passed in p)
	if ( false && p == 0 && query_AA() ) {
		p = im_main;
	}

	if ( p == 0 ) {
		int x, y;
		GetClientSize(&x, &y);
		
		wxBitmap bm(x, y);
#		if wxCHECK_VERSION(2, 8, 0)
		wxMemoryDC mdc(bm);
#		else  // wxCHECK_VERSION(2, 8, 0)
		wxMemoryDC mdc;
		mdc.SelectObject(bm);
#		endif // wxCHECK_VERSION(2, 8, 0)

		// this proc is marked const as none of this class'
		// data is changed; whether parent(s) are changed
		// by this dc use is uncertain (probably not in a
		// way to be concerned with)
		wxClientDC cdc(const_cast<A_Canvas*>(this));
		mdc.Blit(0, 0, x, y, &cdc, 0, 0, wxCOPY, false);

#		if 1 // possibly temporary; it's mostly for working on docs
		const wxPen& pnsave = mdc.GetPen();
		wxPen pn(*wxBLACK, 6);
		mdc.SetPen(pn);
		mdc.DrawLine(0, 0, x, 0);
		mdc.DrawLine(x, 0, x, y);
		mdc.DrawLine(x, y, 0, y);
		mdc.DrawLine(0, y, 0, 0);
		mdc.SetPen(pnsave);
#		endif

#		if wxCHECK_VERSION(2, 8, 0)
		mdc.SelectObjectAsSource(wxNullBitmap);
#		else  // wxCHECK_VERSION(2, 8, 0)
		mdc.SelectObject(wxNullBitmap);
#		endif // wxCHECK_VERSION(2, 8, 0)

		// image is allocated here, and we want it gone at return,
		// so hand it up-scope to auto_ptr declared for this purpose
		ap = std::auto_ptr<const wxImage>(
			p = new wxImage(bm.ConvertToImage()) );
	}

	if ( D->sSdir == wxT("") )
		D->sSdir = wxGetApp().GetLastDlgDir();

	wxString seed;
	wxString f = wxFileSelector(_("Save As File . . .")
		, D->sSdir, seed, wxT("")
		, seed
#		if 1
		+ p->GetImageExtWildcard()
#		else
		+ _("PNG files (*.png)|*.png")
		+ _("|PNM (PPM) files (*.pnm)|*.pnm")
		+ _("|BMP files (*.bmp)|*.bmp")
		+ _("|JPEG files (*.jpg)|*.jpg")
		+ _("|TIFF files (*.tif)|*.tif")
		+ _("|XPM files (*.xpm)|*.xpm")
		+ _("|PCX files (*.pcx)|*.pcx")
#		endif
		, wxSAVE | wxOVERWRITE_PROMPT,
		const_cast<A_Frame*>(a_frame));

	if ( ! f.empty() ) {
		bool r = p->SaveFile(f);
		if ( ! r ) {
			wxString m(wxT("Failed to save the image"));
			ErrorBox(m);
		}
		return r;
	}

	return false;
}

void
A_Canvas::SaveAs()
{
	if ( D->sSdir == wxT("") )
		D->sSdir = wxGetApp().GetLastDlgDir();

	wxFileDialog d(a_frame, _("Please provide a name")
		, D->sSdir, D->sSname
		, wxT("*.pse")
		, wxSAVE | wxOVERWRITE_PROMPT);

	if ( d.ShowModal() == wxID_OK ) {
		wxString tn = d.GetFilename();
		if ( tn != wxT("") ) {
			if ( tn.AfterLast(wxT('.')) != wxT("pse") ) {
				tn += wxT(".pse");
			}
			wxString on(D->sSname), od(D->sSdir);
			D->sSdir = d.GetDirectory();
			D->sSname = tn;
			wxGetApp().SetLastDlgDir(D->sSdir);
			Save();
			if ( D->sSname == wxT("") ) {
				D->sSname = on;
				D->sSdir = od;
				return;
			}
			a_frame->SetTitlePrefix(tn);
			A_Tabwnd* t =
			#if defined(__SC__) && !defined(_CPPRTTI)
				(A_Tabwnd*)a_tabpg->GetParent();
			#else
				dynamic_cast<A_Tabwnd*>(a_tabpg->GetParent());
			#endif
			if ( t )
				t->SetCurPageText(tn);
		}
	}
}

void
A_Canvas::Save()
{
	if ( !D->lst.size() ) return;
	if ( D->sSname == wxT("") ) {
		SaveAs();
		return;
	}

	wxString t = GetCurFullpath();
	errno = 0;
	if ( !WriteData(t, D->lst, hguides, vguides) ) {
		int e = errno;
		wxString msg(_("Failed saving to \""));
		
		msg += t + _("\" : ") + ch2wxs(strerror(e));

		ErrorBox(msg);
		D->sSname = wxT("");
	} else {
		SetClean();
	}
}

wxString
A_Canvas::GetCurFullpath() const
{
	wxString f(GetCurFileName());
	wxString d(GetCurDirName());

	if ( f == wxT("") )
		return wxT("");
#if wxCHECK_VERSION(2, 4, 0)
	if ( d == wxT("") )
		return f;
	return wxFileName(d, f, wxPATH_NATIVE).GetFullPath();
#else
	return d == wxT("") ? f : d + SEPS + f;
#endif
}

void
A_Canvas::ExportAs()
{
	if ( D->sFdir == wxT("") )
		D->sFdir = wxGetApp().GetLastDlgDir();

	wxFileDialog d(a_frame, _("Please provide a name"), D->sFdir
        , D->sFname, wxT("*.*"), wxSAVE | wxOVERWRITE_PROMPT);

	if ( d.ShowModal() == wxID_OK ) {
		D->sFdir = d.GetDirectory();
		D->sFname = d.GetFilename();
		if ( D->sFname != wxT("") )
			Export();
		wxGetApp().SetLastDlgDir(D->sFdir);
	}
}

void
A_Canvas::Export()
{
	if ( !D->lst.size() ) return;
	if ( D->sFname == wxT("") ) {
		ExportAs();
		return;
	}

	wxString& sFname = D->sFname;
	wxString& sFdir = D->sFdir;
	// std::list<SplineBase*>& lst = D->lst;

	// The following code should be moved elsewhere.
	// A Canvas doesn't need to know this stuff.

	// Using C rather than C++ streams.
	wxString t =
#if wxCHECK_VERSION(2, 4, 0)
		wxFileName(sFdir, sFname, wxPATH_NATIVE).GetFullPath();
#else
		(sFdir == wxT("")) ? sFname : (sFdir + SEPC + sFname);
#endif

	FILE* f = fopen(wxs2ch(t), "wt");
	if ( !f ) {
		int e = errno;
		wxString msg;

		msg.Printf(_("Failed to open %s for exporting : %s"), t.c_str()
			, strerror(e));
		ErrorBox(msg);
		sFname = wxT("");
		return;
	}

	ExportFILE(f, sFname);

	int e0 = ferror(f);
	int e1 = fclose(f);
	if ( e0 || e1 ) {
		int e = errno;
		wxString msg;

		msg.Printf(_("Failed exporting to %s : %s"), t.c_str()
			, strerror(e));
		ErrorBox(msg);
	}
}

void
A_Canvas::ExportFILE(FILE* f, const wxString& fname, bool indemo)
{
	cnumtmp c_tmp;
	fprintf(f, "// POV include file\n// Generated by epspline\n\n");
	wxString sentry(wxT("INCLUDED_"));
	sentry += fname;
	sanitise_for_pov(sentry);
	fprintf(f, "\n#ifndef ( %s )\n#declare %s = 1;\n\n"
		, wxs2ch(sentry), wxs2ch(sentry));

	int n = 0;
	double lx = DBL_MAX, ly = DBL_MAX;
	double mx = DBL_MIN, my = DBL_MIN;
	std::list<SplineBase*>::iterator it = D->lst.begin();

	while ( it != D->lst.end() ) {
		SplineBase* p = *it++;
		p->Export(f, n++, lx, ly, mx, my, indemo);
	}

	wxString t(wxT("I_"));
	t += fname;
	sanitise_for_pov(t);
	fprintf(f, "#declare %s_width = %f;\n#declare %s_height = %f;\n"
		, wxs2ch(t), double(mx-lx), wxs2ch(t), double(my-ly));
	fprintf(f, "#declare %s_left = %f;\n#declare %s_right = %f;\n"
		, wxs2ch(t), double(lx), wxs2ch(t), double(mx));
	fprintf(f,"#declare %s_top = %f;\n#declare %s_bottom = %f;\n"
		, wxs2ch(t), double(ly), wxs2ch(t), double(my));

	fprintf(f, "\n#end // #ifdef ( %s )\n\n", wxs2ch(sentry));
}

void
A_Canvas::Demo()
{
	if ( D && D->lst.size() )
		DoPovDemo(*this, D->lst);
}

void
A_Canvas::DrawCur(wxDC& dc)
{
	if ( D->cur ) {
		D->cur->Draw(dc);
		D->cur->DrawDots(dc, wxBrush(wxColour(255,0,0), wxSOLID));
	}
}

void
A_Canvas::DrawCur()
{
	if ( D->cur ) {
		#if USE_wxBufferedDC
		wxClientDC clidc(this);
		PrepareDC(clidc);
		wxBufferedDC dc(&clidc, clidc.GetSize());
		PrepareDC(dc);
		#else
		wxClientDC dc(this);
		PrepareDC(dc);
		#endif

		wxPen o = dc.GetPen();
		dc.SetPen(*wxBLACK_PEN);

		DrawCur(dc);
		dc.SetPen(o);
	}
}

void
A_Canvas::DrawSelPt(wxDC& dc)
{
	const unsigned selsize = 7;
	if ( D->selpt == 0 )
		return;
	SplineBase* sb = 0;
	if ( D->sel )
		sb = D->sel;
	else {
		DataState::itr_t c = D->lst.begin(), e = D->lst.end();
		while ( c != e ) {
			SplineBase* t = *c++;
			if ( t->PointIndex(*D->selpt, 0) >= 0 ) {
				sb = t;
				break;
			}
		}
	}
	if ( sb == 0 )
		return;
	sb->DrawSelectedDot(dc, wxBrush(wxColour(0,255,255), wxSOLID)
		, *D->selpt, selsize);
}

void
A_Canvas::DrawSelPt()
{
	if ( !D->selpt )
		return;

	#if USE_wxBufferedDC
	wxClientDC clidc(this);
	PrepareDC(clidc);
	wxBufferedDC dc(&clidc, clidc.GetSize());
	PrepareDC(dc);
	#else
	wxClientDC dc(this);
	PrepareDC(dc);
	#endif

	DrawSelPt(dc);
}

void
A_Canvas::DrawTransformHandles(wxDC& dc, datatype s)
{
	const int sz = szhandles;
	const int hsz = sz / 2;
	const wxRect& r = *s->BBox();
	wxCoord x, y, w, h;
	wxBrush ob = dc.GetBrush();
	dc.SetBrush(wxBrush(wxColour(192,192,192), wxSOLID));

	x = r.x; y = r.y; w = sz; h = sz;
	dc.DrawRectangle(x, y, w, h);
	x = r.x + (r.width / 2) - hsz; y = r.y;
	w = sz; h = sz;
	dc.DrawRectangle(x, y, w, h);
	x = r.x + r.width - sz + 1; y = r.y;
	w = sz; h = sz;
	dc.DrawRectangle(x, y, w, h);
	x = r.x; y = r.y + (r.height / 2) - hsz;
	w = sz; h = sz;
	dc.DrawRectangle(x, y, w, h);
	x = r.x + r.width - sz + 1; y = r.y + (r.height / 2) - hsz;
	w = sz; h = sz;
	dc.DrawRectangle(x, y, w, h);
	x = r.x; y = r.y + r.height - sz + 1;
	w = sz; h = sz;
	dc.DrawRectangle(x, y, w, h);
	x = r.x + (r.width / 2) - hsz; y = r.y + r.height - sz + 1;
	w = sz; h = sz;
	dc.DrawRectangle(x, y, w, h);
	x = r.x + r.width - sz + 1; y = r.y + r.height - sz + 1;
	w = sz; h = sz;
	dc.DrawRectangle(x, y, w, h);

	dc.SetBrush(ob);
}

or_enum
A_Canvas::TestTransformHandles(datatype s, const wxPoint& pt)
{
	const int sz = szhandles;
	const int hsz = sz / 2;
	const wxRect& r = *s->BBox();
	wxRect R;
	R.width = R.height = sz;

	R.x = r.x; R.y = r.y;
	if ( InRect(R, pt) ) return or_nw;
	R.x = r.x + (r.width / 2) - hsz; R.y = r.y;
	if ( InRect(R, pt) ) return or_n;
	R.x = r.x + r.width - sz + 1; R.y = r.y;
	if ( InRect(R, pt) ) return or_en;
	R.x = r.x; R.y = r.y + (r.height / 2) - hsz;
	if ( InRect(R, pt) ) return or_w;
	R.x = r.x + r.width - sz + 1; R.y = r.y + (r.height / 2) - hsz;
	if ( InRect(R, pt) ) return or_e;
	R.x = r.x; R.y = r.y + r.height - sz + 1;
	if ( InRect(R, pt) ) return or_ws;
	R.x = r.x + (r.width / 2) - hsz; R.y = r.y + r.height - sz + 1;
	if ( InRect(R, pt) ) return or_s;
	R.x = r.x + r.width - sz + 1; R.y = r.y + r.height - sz + 1;
	if ( InRect(R, pt) ) return or_se;

	return or_not;
}

void
A_Canvas::DrawSelBox(SplineBase* s)
{
	#if USE_wxBufferedDC
	wxClientDC clidc(this);
	PrepareDC(clidc);
	wxBufferedDC dc(&clidc, clidc.GetSize());
	PrepareDC(dc);
	#else
	wxClientDC dc(this);
	PrepareDC(dc);
	#endif

	DrawSelBox(dc, s);
}

void
A_Canvas::DrawSelBox(wxDC& dc, SplineBase* s)
{
	wxPen op = dc.GetPen();
	wxPen np(*wxGREY_PEN);
	np.SetStyle(wxSHORT_DASH);
	dc.SetPen(np);
#if 0
	wxBrush ob = dc.GetBrush();
	wxBrush nb(wxNullBrush);
	nb.SetStyle(wxTRANSPARENT);
	dc.SetBrush(nb);
	wxRect r(*s->BBox());
	dc.DrawRectangle(r.x, r.y, r.width, r.height);
	dc.SetBrush(ob);
#else
	wxPoint p [5];
	s->BBox(p[0], p[2]);
	p[1].x = p[2].x;
	p[1].y = p[0].y;
	p[3].x = p[0].x;
	p[3].y = p[2].y;
	p[4] = p[0];
	dc.DrawLines(A_SIZE(p), p);
#endif
	dc.SetPen(op);

	s->DrawDots(dc, wxBrush(wxColour(255,0,0), wxSOLID));
	DrawCur(dc);
	DrawSelPt(dc);

	if ( transforming ) {
		DrawTransformHandles(dc, s);
		if ( false && !scaling ) {
			DrawTransCtr(dc, false, true, transctrpt);
		}
	}
}

// draw bg grid: takes rectangle in device coordinates
// 'wid' is pixels between grid-lines -- 0 or less means no lines
// 'pad' is to inflate the rect.; generally should be 0
void
A_Canvas::DrawGrid(wxDC& dc, const wxRect& R, int wid, int pad)
{
	wxRect r(R);

	r.width = dc.DeviceToLogicalX(r.width + r.x + pad * 2);
	r.x = dc.DeviceToLogicalX(r.x - pad);
	r.width -= r.x;
	r.height = dc.DeviceToLogicalY(r.height + r.y + pad * 2);
	r.y = dc.DeviceToLogicalY(r.y - pad);
	r.height -= r.y;

	DrawGridLogical(dc, r, wid);
}

// draw bg grid: takes rectangle in 'logical' coordinates
// 'wid' is pixels between grid-lines -- 0 or less means no lines
void
A_Canvas::DrawGridLogical(wxDC& dc, const wxRect& r, int wid)
{
	wxPen op = dc.GetPen();
	dc.SetPen(wxNullPen);
	dc.DrawRectangle(r.x, r.y, r.width, r.height);

	wxColour clr(0xE0,0xE0,0xFF);
	#if defined(__WXGTK__) && 0
	wxPen    np(clr, 1, wxSHORT_DASH);
	#else
	wxPen    np(clr, 1, wxSOLID);
	#endif
	dc.SetPen(np);

	if ( wid > 0 ) {
		int xs = wid;
		int ys = wid;
		int offs = r.x - (r.x % xs); // - 1;
		int cnt = ((r.width + r.x) - offs) / xs;

		while ( cnt-- > 0 ) {
			offs += xs;
			dc.DrawLine(offs, r.y, offs, r.y + r.height);
		}
	
		offs = r.y - (r.y % ys); // - 1;
		cnt = ((r.height + r.y) - offs) / ys;

		while ( cnt-- > 0 ) {
			offs += ys;
			dc.DrawLine(r.x, offs, r.x + r.width, offs);
		}
	}

	dc.SetPen(guidepen);
	guidestore::iterator i, e;
	int offs;

	i = hguides.begin(); e = hguides.end();
	while ( i != e ) {
		offs = *i++;
		if ( offs >= r.y && offs < (r.y + r.height) )
			dc.DrawLine(r.x, offs, r.x + r.width, offs);
	}

	i = vguides.begin(); e = vguides.end();
	while ( i != e ) {
		offs = *i++;
		if ( offs >= r.x && offs < (r.x + r.width) )
			dc.DrawLine(offs, r.y, offs, r.y + r.height);
	}

	dc.SetPen(op);
}

void
A_Canvas::OnEraseBG(wxEraseEvent& event)
{
	if ( true && false )
		event.Skip();
}

// paint visible client area to a wxImage --
// arg is ** because this method will create or
// recreate as necessary and assign to caller's
// variable -- arg logrc recieves logical coords
// that were drawn to image, and
// arg thickness is line thickness
void
A_Canvas::ImagePaint(wxImage** ipp, wxRect& logrc, double thickness)
{
	wxImage* ip = *ipp;

	double sx = xscale / 100.0, sy = yscale / 100.0;
	int vw, vh;
	GetClientSize(&vw, &vh);
	int vws = std::max(vw, irint(std::ceil(vw / sx))) + bgpad2;
	int vhs = std::max(vh, irint(std::ceil(vh / sy))) + bgpad2;
	
	// make sure image exists, at sufficient dimensions
	if ( ip && (ip->GetWidth() != vws || ip->GetHeight() != vhs) ) {
		delete ip;
		ip = 0;
		*ipp = ip;
	}
	if ( ip == 0 ) {
		ip = alloc_image(vws, vhs, false);
		*ipp = ip;
	}

	int vx, vy, sux, suy;
	GetScrollPixelsPerUnit(&sux, &suy);
	GetViewStart(&vx, &vy);
	wxRect rr(
		irint(vx * sux / sx),
		irint(vy * suy / sy),
		irint(vws /* * sx */),
		irint(vhs /* * sy */));

	logrc = rr;

	DrawGridOnRast(*ip, rr, def_grid_space);

	aap::draw_device dr(*ip);
	aap::pxl px(0, 0, 0);
	int ox = rr.x - bgpad;
	int oy = rr.y - bgpad;

	itr_t it = D->lst.begin(), e = D->lst.end();
	while ( it != e ) {
		data_type_ptr p = *it++;
		wxRect r;

		p->CBox(r);
		if ( IntersectRect(r, rr) ) {
			const SplineBase::curve_cache& C = p->GetCachedPoints();

			for ( unsigned i = 1; i < C.size(); i++ ) {
				const CurvePt& p0 = C[i-1];
				const CurvePt& p1 = C[i];
				
				if ( p0.isflag() || p1.isflag() )
					continue;
				
				aap::drawline_wu(
			         thickness,
			         p0.getX() - ox, p0.getY() - oy,
			         p1.getX() - ox, p1.getY() - oy,
			         dr,
			         long(dr.Width()), long(dr.Height()),
			         px,
			         true);
			 }
		}
	}
}

// handler for wx paint event -- generally not to be called directly
void
A_Canvas::OnPaint(wxPaintEvent& /*event*/)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

	dc.SetPen(*wxBLACK_PEN);
	dc.SetBackground(*wxWHITE_BRUSH);

	if ( ! aa_draw ) {
		wxRegion re = GetUpdateRegion();
		wxRect rr(re.GetBox());
	
		DCPaint(dc, rr);
	} else {
		wxRect rr;
	
		ImagePaint(&im_main, rr);
		wxBitmap tbm(*im_main);

		// bg image is drawn with padding to get around a bug
		// that leaves some garbage in 1st row/column (I think
		// the bug is in wxDC::DrawBitmap, but it might be mine),
		// so target coords are offset here
		dc.DrawBitmap(tbm, rr.x - bgpad, rr.y - bgpad, false);
	}

	if ( D->sel ) {
		DrawSelBox(dc, D->sel);
		if ( transforming && !scaling ) {
			DrawTransCtr(dc, false, true, transctrpt);
		}
	}
	if ( D->cur ) {
		DrawCur(dc);
		DrawSelPt(dc);
	}
}

// take a device context and paint to rect, which is
// modified by DeviceToLogical[XY]
// -- intended as core of OnPaint handler --
// may be useful elsewhere
// this does *not* draw selected box or highlights,
// or transform decorations
void
A_Canvas::DCPaint(wxDC& dc, wxRect& rr)
{
	int pad = 1 + std::max(xscale, yscale) / 101;
	rr.Inflate(pad, pad);

	int x1 = dc.DeviceToLogicalX(rr.x + rr.width);
	int y1 = dc.DeviceToLogicalY(rr.y + rr.height);
	rr.x = dc.DeviceToLogicalX(rr.x);
	rr.y = dc.DeviceToLogicalY(rr.y);
	rr.width = x1 - rr.x;
	rr.height = y1 - rr.y;

	DrawGridLogical(dc, rr);

	itr_t it = D->lst.begin(), e = D->lst.end();
	while ( it != e ) {
		data_type_ptr p = *it++;
		wxRect r(*p->BBox());

		if ( r.x == INT_MAX || r.y == INT_MAX ) { // 1st time draw
			p->Draw(dc);
			continue;
		}
		
		p->CBox(r);
		if ( IntersectRect(r, rr) ) {
			r = rr;
			r.Inflate(16, 16);
			p->Draw(dc, &r);
		}
	}
}

// take a device context and paint to region(s) passed
// in wxRegion& re -- intended as core of OnPaint handler --
// may be useful elsewhere
// this does *not* draw selected box or highlights,
// or transform decorations
// NOTE: this usually [always] leaves artifacts: it seems the iterated
// sub-regions are not accurate(?).
void
A_Canvas::DCPaint(wxDC& dc, wxRegion& re)
{
	wxRegionIterator ri(re);

	while ( ri ) {
		wxRect rr(ri.GetRect());
		DCPaint(dc, rr);
		ri++;
	}
}

void
A_Canvas::PrepareDC(wxDC& dc)
{
	wxScrolledWindow::PrepareDC(dc);
	//a_frame->PrepareDC(dc);

#if ! wxCHECK_VERSION(2, 6, 0)
	dc.SetOptimization(true);
#endif
	dc.SetUserScale(double(xscale)/100.0, double(yscale)/100.0);
}

// draw bg grid: this draws onto wxImage because as of wx 2.9
// there is no portably working way to draw into wxBitmap raw data
// (don't even mention wxPixelData!')
void
A_Canvas::DrawGridOnRast(wxImage& im, const wxRect& r, int wid,
	wxColour bg,  // backround color
	wxColour rlc, // rules color
	wxColour grc  // grid color
	)
{
	// wxImage does not include alpha in 
	// 4-component pixels as I had assumed, but in separate
	// memory
	//bool alph = im.HasAlpha();
	int imw = im.GetWidth();
	int imh = im.GetHeight();
	// no, never 4, see above
	//int pixlen = alph ? 4 : 3;
	int pixlen = 3;
	int rowlen = imw * pixlen;
	unsigned char* pd = im.GetData();
	// vars for colors to be assigned
	const unsigned char opaq = 255;
	unsigned char cR, cG, cB, cA = opaq;
	// pixel component indices
	// no, alpha is not the 4th indice, but the 1st indice
	// in separate memory
	const int iR = 0, iG = 1, iB = 2; //, iA = 3;
	
	// device x, y
	int X, Y, W, H;
	X = r.x;
	Y = r.y;
	W = r.width;
	H = r.height;

	// image is drawn with padding to work around a bug that leaves
	// garbage in the 1st row or column -- I think the bug is in wx
	X -= bgpad;
	W += bgpad2;
	Y -= bgpad;
	H += bgpad2;

	// write bg color
	cR = bg.Red();
	cG = bg.Green();
	cB = bg.Blue();
#	if wxCHECK_VERSION(2, 8, 0)
	cA = bg.Alpha();
#	endif
	unsigned char* rp = pd;
	for ( int j = 0; j < imh; j++ ) {
		unsigned char* p = rp;

		for ( int i = 0; i < imw; i++ ) {
			p[iR] = cR; p[iG] = cG; p[iB] = cB;
			p += pixlen;
		}

		rp += rowlen;
	}

	// draw grid lines
	if ( wid > 0 ) {
		cR = grc.Red();
		cG = grc.Green();
		cB = grc.Blue();
#		if wxCHECK_VERSION(2, 8, 0)
		cA = grc.Alpha();
#		endif
		int xs = wid;
		int ys = wid;
		int xsm = X % xs;
		int ysm = Y % ys;

		for ( int offs = -xsm; offs < imw; offs += xs ) {
			if ( offs < 0 )
				continue;
			//orig. dc code: im.DrawLine(offs, r.y, offs, r.y + H);
			int i = 0;
			unsigned char* p = pd + pixlen * offs;
			for ( p += (rowlen * i); i < imh; i++ ) {
				p[iR] = cR;
				p[iG] = cG;
				p[iB] = cB;
				p += rowlen;
			}
		}
	
		for ( int offs = -ysm; offs < imh; offs += ys ) {
			if ( offs < 0 )
				continue;
			//orig. dc code: im.DrawLine(r.x, offs, r.x + W, offs);
			int i = 0;
			unsigned char* p = pd + rowlen * offs;
			for ( p += (pixlen * i); i < imw; i++) {
				p[iR] = cR;
				p[iG] = cG;
				p[iB] = cB;
				p += pixlen;
			}
		}
	}

	// draw guide lines
	cR = rlc.Red();
	cG = rlc.Green();
	cB = rlc.Blue();
#	if wxCHECK_VERSION(2, 8, 0)
	cA = rlc.Alpha();
#	endif
	guidestore::iterator b, e;
	int offs;

	b = hguides.begin(); e = hguides.end();
	while ( b != e ) {
		offs = *b++ - Y;
		if ( offs >= 0 && offs < imh ) {
			//orig. dc code: im.DrawLine(r.x, offs, r.x + W, offs);
			int i = 0;
			unsigned char* p = pd + rowlen * offs;
			for ( p += (pixlen * i); i < imw; i++) {
				p[iR] = cR;
				p[iG] = cG;
				p[iB] = cB;
				p += pixlen;
			}
		}
	}

	b = vguides.begin(); e = vguides.end();
	while ( b != e ) {
		offs = *b++ - X;
		if ( offs >= 0 && offs < imw ) {
			//orig. dc code: im.DrawLine(offs, r.y, offs, r.y + H);
			int i = 0;
			unsigned char* p = pd + pixlen * offs;
			for ( p += (rowlen * i); i < imh; i++ ) {
				p[iR] = cR;
				p[iG] = cG;
				p[iB] = cB;
				p += rowlen;
			}
		}
	}
}

void
A_BUFDCCanvas::OnScroll(wxScrollWinEvent& event)
{
// place holder
	A_Canvas::OnScroll(event);
}

void
A_BUFDCCanvas::OnSize(wxSizeEvent& event)
{
	A_Canvas::OnSize(event);

#	if 0
	wxSize sz = event.GetSize();
	if ( bm_main && (sz.GetWidth() != bm_main->GetWidth() ||
	     sz.GetHeight() != bm_main->GetHeight()) ) {
		delete bm_main;
		bm_main = 0;
	}
#	else
	if ( bm_main ) {
		delete bm_main;
		bm_main = 0;
	}
#	endif
}

void
A_BUFDCCanvas::OnEraseBG(wxEraseEvent& event)
{
// place holder
	if ( false && (xscale != 100 || yscale != 100) ) {
		A_Canvas::OnEraseBG(event);
		return;
	}
}

void
A_BUFDCCanvas::OnPaint(wxPaintEvent& /*event*/)
{
	// if size changes image(s) are deleted in OnSize(), but
	// not reallocated until point of use, in case multiple
	// resize events are delivered rapidly, to many windows, etc.
	if ( bm_main == 0 ) {
		int vw, vh;
		GetClientSize(&vw, &vh);
		bm_main = alloc_bitmap(vw, vh);
	}

#	if wxCHECK_VERSION(2, 8, 0)
	wxMemoryDC dc(*bm_main);
#	else  // wxCHECK_VERSION(2, 8, 0)
	wxMemoryDC dc;
	dc.SelectObject(*bm_main);
#	endif // wxCHECK_VERSION(2, 8, 0)

	PrepareDC(dc);

	dc.SetPen(*wxBLACK_PEN);
	dc.SetBackground(*wxWHITE_BRUSH);

	wxRect rr(0, 0, bm_main->GetWidth(), bm_main->GetHeight());
	
	if ( ! aa_draw ) {
		DCPaint(dc, rr);
	} else {
		ImagePaint(&im_main, rr);
		wxBitmap tbm(*im_main);

		// bg image is drawn with padding to get around a bug
		// that leaves some garbage in 1st row/column (I think
		// the bug is in wxDC::DrawBitmap, but it might be mine),
		// so target coords are offset here
		dc.DrawBitmap(tbm, rr.x - bgpad, rr.y - bgpad, false);
	}
	
	if ( D->sel ) {
		DrawSelBox(dc, D->sel);
		if ( transforming && !scaling ) {
			DrawTransCtr(dc, false, true, transctrpt);
		}
	}
	if ( D->cur ) {
		DrawCur(dc);
		DrawSelPt(dc);
	}

#	if wxCHECK_VERSION(2, 8, 0)
	dc.SelectObjectAsSource(wxNullBitmap);
#	else  // wxCHECK_VERSION(2, 8, 0)
	dc.SelectObject(wxNullBitmap);
#	endif // wxCHECK_VERSION(2, 8, 0)

	wxPaintDC pdc(this);
	pdc.DrawBitmap(*bm_main, 0, 0, false);
}

void
A_BUFDCCanvas::PrepareDC(wxDC& dc)
{
	// TODO: remove this override if no further use is found
	A_Canvas::PrepareDC(dc);
}

// The Data

A_Canvas::DataState::DataState()
	: sFname(wxT("")), sFdir(wxT("")), sSname(wxT("")), sSdir(wxT(""))
	, cur(0), sel(0), selpt(0)
	, objt(prism), splinet(cubic), sweept(slinear)
	, dirty(false)
{
}

A_Canvas::DataState::DataState(const A_Canvas::DataState& o)
{
	*this = o;
}

A_Canvas::DataState::~DataState()
{
	delete_contained(lst);
}

A_Canvas::DataState&
A_Canvas::DataState::operator = (const A_Canvas::DataState& o)
{
	sFname = o.sFname;
	sFdir = o.sFdir;
	sSname = o.sSname;
	sSdir = o.sSdir;

	objt = o.objt;
	splinet = o.splinet;
	sweept = o.sweept;

	selpt = 0;
	sel = 0;
	cur = 0;

	dirty = o.dirty;

	delete_clear_contained(lst);
	std::list<SplineBase*>::const_iterator i = o.lst.begin();
	while ( i != o.lst.end() ) {
		const SplineBase* p = *i++;
		if ( !p ) continue;
		SplineBase* t = CopyObj(p);
		if ( !t ) {
			std::fprintf(stderr, "\nspline editor: internal error:\n"
				"  t == 0 on %d, %s\n", __LINE__, __FILE__);
			continue;
		}
		lst.push_back(t);

		if ( p == o.cur )
			cur = t;
		if ( p == o.sel ) {
			sel = t;
			if ( o.selpt ) {
				int i = p->PointIndex(*o.selpt, 0);
				if ( i >= 0 )
					selpt = &((*t)[i]);
			}
		}
	}

	return *this;
}

void
A_Canvas::DataState::UpdateObj(SplineBase*& o)
{
	if ( !o ) return;

	SplineBase::spline_t spt;

	switch ( splinet ) {
		case linear: spt = SplineBase::linear; break;
		case quadratic: spt = SplineBase::quadratic; break;
		case bezier: spt = SplineBase::bezier; break;
		default: spt = SplineBase::cubic; break;
	}

	if ( o->Getsplinet() != spt ) {
		SplineBase* t = NewObj();
		replace(lst.begin(), lst.end(), o, t);
		copy(o->begin(), o->end(), back_inserter(*t));
		delete o;
		o = t;
	} else {
		if ( sweept == conic )
			o->Getsweept() = SplineBase::conic;
		else
			o->Getsweept() = SplineBase::slinear;

		if ( objt == lathe )
			o->SetType(SplineBase::lathe);
		else
			o->SetType(SplineBase::prism);
	}
}

SplineBase*
A_Canvas::DataState::NewObj()
{
	SplineBase* t;
	switch ( splinet ) {
		case linear:  t = new LinearSpline; break;
		case quadratic: t = new QuadraticSpline; break;
		case bezier:  t = new BezierSpline; break;
		default:      t = new CubicSpline; break;
	}

	if ( sweept == conic )
		t->Getsweept() = SplineBase::conic;
	else
		t->Getsweept() = SplineBase::slinear;

	if ( objt == lathe )
		t->SetType(SplineBase::lathe);
	else if ( objt == prism )
		t->SetType(SplineBase::prism);
	else
		t->SetType(SplineBase::undef);

	return t;
}

// Simple functions that could not be inlined in the
// header due to mutual include problem between
// a_frame.h and a_canvas.h if the header were to
// have the A_Frame protos.

void
A_Canvas::enableExportCurves(bool b)
{a_frame->enableExportCurves(b); m_pop->Enable(IC_export, b);}
void
A_Canvas::enableExportAsCurves(bool b)
{a_frame->enableExportAsCurves(b); m_pop->Enable(IC_exportas, b);}
void
A_Canvas::enableSaveCurves(bool b)
{a_frame->enableSaveCurves(b); m_pop->Enable(IC_save, b);}
void
A_Canvas::enableSaveAsCurves(bool b)
{a_frame->enableSaveAsCurves(b); m_pop->Enable(IC_saveas, b);}
void
A_Canvas::enableOpenCurves(bool b)
{a_frame->enableOpenCurves(b);}
void
A_Canvas::enableCloseCurves(bool b)
{a_frame->enableCloseCurves(b);}
void
A_Canvas::enableEdUndo(bool b)
{a_frame->enableEdUndo(b);}
void
A_Canvas::enableEdRedo(bool b)
{a_frame->enableEdRedo(b);}
void
A_Canvas::enableEdMoveDown(bool b)
{a_frame->enableEdMoveDown(b);}
void
A_Canvas::enableEdMoveUp(bool b)
{a_frame->enableEdMoveUp(b);}
void
A_Canvas::enableEdCopy(bool b)
{a_frame->enableEdCopy(b);}
void
A_Canvas::enableEdCopyAll(bool b)
{a_frame->enableEdCopyAll(b);}
void
A_Canvas::enableEdCut(bool b)
{a_frame->enableEdCut(b);}
void
A_Canvas::enableEdDel(bool b)
{a_frame->enableEdDel(b);}
void
A_Canvas::enableEdPaste(bool b)
{a_frame->enableEdPaste(b);}
void
A_Canvas::enableEdCopyGlobal(bool b)
{a_frame->enableEdCopyGlobal(b);}
void
A_Canvas::enableEdCopyAllGlobal(bool b)
{a_frame->enableEdCopyAllGlobal(b);}
void
A_Canvas::enableEdCutGlobal(bool b)
{a_frame->enableEdCutGlobal(b);}
void
A_Canvas::enableEdPasteGlobal(bool b)
{a_frame->enableEdPasteGlobal(b);}
void
A_Canvas::enableHelpDemo(bool b)
{a_frame->enableHelpDemo(b);}

void
A_Canvas::ErrorBox(const wxString& msg, const wxString& titletail)
const
{
	a_frame->ErrorBox(msg, titletail);
}

