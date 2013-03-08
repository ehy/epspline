/*
 * For user management of spline object properties
 */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "cfg.h"
#include "stdcc.h"
#include "splines.h"
#include "spline_props.h"


int
SetSplineProps(SplineBase* s, wxWindow* parent)
{
	spline_properties pd(parent);
	s->InitPropsDialog(pd);
	pd.Centre(wxBOTH);

	int r = pd.ShowModal();
	if ( r == wxID_OK ) {
		s->SetProps(pd);
	}
	
	return r;
}

