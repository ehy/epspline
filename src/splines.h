
/*
 * Classes for splines.
 */

#ifndef _SPLINES_H_
#define _SPLINES_H_

#include "cfg.h"
#include "util.h"
#include <vector>

// Here's the little class structure:
// The point is creating POV's various spline-based objects,
// so the first class is SplineBase, derived from a standard
// C++ container for storage and ease of use.
//
// From this POV spline type are derived:
// {Linear,Quadratic,Cubic,Bezier}Spline.  These use math from
// the POV source code[1] to draw -- on a wxDC -- spline curves
// just like the outlines of object POV will generate from
// the same set of Points.
//
// Conversion among spline types requires allocing
// a whole new spline type object and copying points before
// deleting the old.
//
// [1] POVRay source code is not licensed with permission
// for reuse.  Please read the POVRay license.
// The very small amount of code (24 lines) used in this
// program was merely arithmetic (not even algorithmic)
// and is used only after contacting the POV Team
// and obtaining permission.  See POV_permission.txt for
// details.


class PovSplineObject;

#include "spline_props.h"

// forward declare the cached curve point object
class CurvePt;

// Maintain splines with reals, but let interface
// be in terms of ints.
typedef wxRealPoint     SplinePoint;

#if 0
typedef std::vector<SplinePoint> SplineBaseBase;
#else
typedef std::vector<SplinePoint> SplinePointDataStruct;
struct SplineBaseBase : public SplinePointDataStruct {
	SplineBaseBase() {}
	SplineBaseBase(unsigned size) : SplinePointDataStruct(size) {}
	virtual ~SplineBaseBase() {}
};
#endif

#ifndef SPLINEPOINT_TOLERANCE
#define SPLINEPOINT_TOLERANCE 4
#endif
const int sptol = SPLINEPOINT_TOLERANCE;

#ifndef SPLINEPOINT_DOTRADIUS
#define SPLINEPOINT_DOTRADIUS 3
#endif
const unsigned dotsz = SPLINEPOINT_DOTRADIUS;

#ifndef SPLINEPOINT_SELDOTSIZE
#define SPLINEPOINT_SELDOTSIZE 7
#endif
const unsigned sdotsz = SPLINEPOINT_SELDOTSIZE;

// declare namespace to that isolates some funcs & functors
namespace eps_funcs {
    // forward declarations
    class sp_offset;
    class cp_offset;
};

#ifdef __GNUG__
    #pragma interface
#endif

class SplineBase : public SplineBaseBase {
	friend class PtInBBox;
	friend class DataState;
	friend class eps_funcs::sp_offset;
	friend class eps_funcs::cp_offset;

public:
	virtual void Export(FILE* f, int n,
                     double& lx, double& ly,
                     double& mx, double& my,
                     bool indemo = false);
	virtual void CalcCurveCache() = 0;
	virtual void Draw(wxDC*, const wxRect* bound = 0) = 0;
	inline  void Draw(wxDC& dc, const wxRect* bound = 0) {
		Draw(&dc, bound);
	}
	inline  void CalcBBox() {
		bool t=doclose; doclose=false; CalcCurveCache(); doclose=t;
	}
	inline  void CloseSpline() { SetDirty(); Draw(0); }
	virtual void DrawDots(wxDC& dc,wxBrush br,unsigned size = dotsz);
	virtual void DrawDots(wxDC& dc,unsigned size = dotsz);
	virtual void DrawSelectedDot(wxDC& dc, wxBrush br,
                    const SplinePoint& pt, unsigned size = sdotsz);
	virtual void DrawSelectedDot(wxDC& dc,const SplinePoint& pt
		, unsigned size = sdotsz);
	virtual void DrawDotsSelectedDot(wxDC& dc, const SplinePoint& pt
		, wxBrush selectedbr
		, unsigned size = dotsz, unsigned selsize = sdotsz);
	virtual void AddPts(int n, int pts[]);
	virtual void AddPts(int n, SplinePoint pts[]);
	virtual void SetPts(int n, int pts[]);
	virtual void SetPts(int n, SplinePoint pts[]);

	virtual bool AddPoint(const SplinePoint& pt) = 0;
	virtual bool DelPoint(const SplinePoint& pt) = 0;
	virtual bool MovePoint(SplinePoint& pt,
                        double xoff, double yoff);
	inline  bool AddPoint(const wxPoint& pt) {
		return AddPoint(SplinePoint(pt.x, pt.y));
	}
	inline  bool DelPoint(const wxPoint& pt) {
		return DelPoint(SplinePoint(pt.x, pt.y));
	}
	inline  bool MovePoint(SplinePoint& pt, int xoff, int yoff) {
		return MovePoint(pt, double(xoff), double(yoff));
	}
	virtual bool MoveSpline(double xoff, double yoff);
	inline  bool MoveSpline(int xoff, int yoff) {
        return MoveSpline(double(xoff), double(yoff));
	}

	inline void push_back(const SplinePoint& p) {
		SplineBaseBase::push_back(p);
		SetDirty();
	}
	inline void push_back(const wxPoint& p) {
		const SplinePoint P(p.x, p.y);
		SplineBaseBase::push_back(P);
		SetDirty();
	}

	// param unsigned type must be an or_enum, found in wxutil.h
	virtual void Scale(unsigned type, int xs, int ys);
	virtual void Flip(unsigned type);
	virtual void Shear(unsigned type,
                    wxPoint p0, wxPoint p1, wxPoint c);
	virtual void Rotate(unsigned type,
                     wxPoint p0, wxPoint p1, wxPoint c);
	virtual void Rotate(double angle, const wxRealPoint& ctr);

	// Bounding box, including contol point loci
	virtual void BBox(wxPoint& least, wxPoint& most);
	const wxRect* BBox() const {return &bbox;}
	// Bounding box, only calculated loci
	virtual void
	CBox(double& mnx, double& mny, double& mxx, double& mxy);
	void CBox(wxRect& r) {
		double mnx, mny, mxx, mxy;
		CBox(mnx, mny, mxx, mxy);
		r.x = int(std::floor(mnx));
		r.y = int(std::floor(mny));
		r.width = int(std::ceil(mxx)) - r.x;
		r.height = int(std::ceil(mxy)) - r.y;
	}

	virtual int
	PointIndex(const SplinePoint& pt, int tolerance = sptol) const;
	virtual SplineBaseBase::iterator
	PointIndexIt(const SplinePoint& pt, int tolerance = sptol);
	inline int
        PointIndex(const wxPoint& pt, int tolerance = sptol) const {
		return PointIndex(SplinePoint(pt.x, pt.y), tolerance);
	}
	virtual SplineBaseBase::iterator
	PointIndexIt(const wxPoint& pt, int tolerance = sptol) {
		return PointIndexIt(SplinePoint(pt.x, pt.y), tolerance);
	}

	virtual bool PtInRect(const wxPoint& p) const;
	virtual bool PtInRect(const wxPoint& p, unsigned pad) const;

	virtual bool Okay() const;
	virtual SplineBase* CopySelf() const = 0;

	enum obj_t { undef, prism, lathe };
	enum spline_t { linear, quadratic, cubic, bezier };
	enum sweep_t { slinear, conic };
	virtual void SetType(obj_t type = undef);

	virtual void InitPropsDialog(spline_properties& dlg);
	virtual void SetProps(spline_properties& dlg);

	inline  void toggle_tol() {tol^=tolbk;tolbk^=tol;tol^=tolbk;}
	inline  void toggle_close() {doclose = !doclose;}

	virtual wxString GetDescription() const;

	SplineBase(int close_tolerance = sptol)
		: ObNam(SplineBase::DefaultName)
		, Transform(EmptyFieldString()), Texture(EmptyFieldString())
		, Interior(EmptyFieldString()), UserStr(EmptyFieldString())
		, objt(undef), splinet(cubic), sweept(slinear)
		, sweep_min(-1.0), sweep_max(1.0)
		, use_sturm(false), use_open(false), dotlock(false)
		, tol(close_tolerance)
		, tolbk(0), doclose(true), dirty(true)
		{ InitBbox(); InitCbox(); }
	SplineBase(const wxString& name, int close_tolerance = sptol)
		: ObNam(name)
		, Transform(EmptyFieldString()), Texture(EmptyFieldString())
		, Interior(EmptyFieldString()), UserStr(EmptyFieldString())
		, objt(undef), splinet(cubic), sweept(slinear)
		, sweep_min(-1.0), sweep_max(1.0)
		, use_sturm(false), use_open(false), dotlock(false)
		, tol(close_tolerance)
		, tolbk(0), doclose(true), dirty(true)
		{ InitBbox(); InitCbox(); }
	SplineBase(const SplineBase& o)
		: SplineBaseBase()
		, ObNam(o.ObNam), Transform(o.Transform)
		, Texture(o.Texture), Interior(o.Interior), UserStr(o.UserStr)
		, objt(o.objt), splinet(o.splinet), sweept(o.sweept)
		, sweep_min(o.sweep_min), sweep_max(o.sweep_max)
		, use_sturm(o.use_sturm), use_open(o.use_open), dotlock(false), 
		bbox(o.bbox), cboxL(o.cboxL), cboxM(o.cboxM)
		, tol(o.tol), tolbk(o.tolbk), doclose(o.doclose), dirty(o.dirty)
	{
		copy(o.begin(), o.end(), back_inserter(*this));
		ccache.reserve(o.ccache.capacity());
		copy(o.ccache.begin(), o.ccache.end(), back_inserter(ccache));
	}
	virtual ~SplineBase();

	// cache storage for calculated curve points
	typedef		std::vector<CurvePt> curve_cache;
protected:
	wxString ObNam, Transform, Texture, Interior, UserStr;
	obj_t			objt;
	spline_t		splinet;
	sweep_t		sweept;
	double			sweep_min, sweep_max;
	bool			use_sturm, use_open;
	bool			dotlock;
	// cache storage for calculated curve points
	//typedef		std::vector<CurvePt> curve_cache; made public
	curve_cache	ccache;

public:
	static const t_ch		DefaultName [];

	static const t_ch*		PropNames [];
	static const size_t	PropCount;
	enum properties {
		name = 0,
		transform,
		texture,
		interior,
		userstr,
		end_properties
	};

	enum { X, Y };

	// empty(-ish) test for the several string fields, to avoid
	// so many in-line if() tests
	static wxString	EmptyFieldString(); // translated, like '_none_'
	static bool StringFieldEmpty(const wxString& s);
	static inline bool NotStringFieldEmpty(const wxString& s) {
		return ! StringFieldEmpty(s);
	}

	// For using code to do something with cached points
	const curve_cache& GetCachedPoints() const { return ccache; }

	wxString  GetObNam() const { return ObNam; }
	wxString  GetTransform() const { return Transform; }
	wxString  GetTexture() const { return Texture; }
	wxString  GetInterior() const { return Interior; }
	wxString  GetUserStr() const { return UserStr; }

	obj_t		Getobjt() const { return objt; }
	spline_t	Getsplinet() const { return splinet; }
	sweep_t	Getsweept() const { return sweept; }
	double		Getsweepmin() const { return sweep_min; }
	double		Getsweepmax() const { return sweep_max; }
	bool		Getusesturm() const { return use_sturm; }
	bool		Getuseopen() const { return use_open; }

	// c++ can't understand friend class A_Canvas::DataState
	// (can't understand nested class) -- so these are public
	wxString& GetObNam() { return ObNam; }
	wxString& GetTransform() { return Transform; }
	wxString& GetTexture() { return Texture; }
	wxString& GetInterior() { return Interior; }
	wxString& GetUserStr() { return UserStr; }

	obj_t&		Getobjt() { return objt; }
	spline_t&	Getsplinet() { return splinet; }
	sweep_t&	Getsweept() { return sweept; }
	double&	Getsweepmin() { return sweep_min; }
	double&	Getsweepmax() { return sweep_max; }
	bool&		Getusesturm() { return use_sturm; }
	bool&		Getuseopen() { return use_open; }

	bool      GetDotLock() { return dotlock; }
	bool      SetDotLock(bool lock = true)
	{
		bool o = dotlock;
		dotlock = lock;
		return o;
	}

    void         SetDirty(bool isdirty = true) { dirty = isdirty; }

protected:
	wxRect			bbox;
	wxRealPoint		cboxL, cboxM;
	int				tol, tolbk;
	bool			doclose;
	// changes; remake cache
	bool			dirty;

	void         InitBbox();
    void         InitCbox();
	void         UpdateBbox(const wxPoint& p);
	void         UpdateBbox(int x, int y)
	{
        wxPoint p(x, y); UpdateBbox(p);
	}
	void         UpdateBbox(const wxRealPoint& p);
	void         UpdateBbox(double x, double y)
	{
        wxRealPoint p(x, y); UpdateBbox(p);
	}
    void         UpdateCbox(double x, double y);
    inline void UpdateCbox(const wxRealPoint& P)
    {
        UpdateCbox(P.x, P.y);
    }

    void         DrawCache(
					wxDC* dc,
					const curve_cache& L,
					const wxRect* bound = 0);
};

class LinearSpline : public SplineBase {
public:
	virtual void CalcCurveCache();
	virtual void Draw(wxDC* dc, const wxRect* bound = 0);
	//virtual void DrawDots(wxDC& dc,wxBrush br,unsigned size = dotsz
	virtual bool Okay() const;
	virtual bool AddPoint(const SplinePoint& pt);
	virtual bool DelPoint(const SplinePoint& pt);
	virtual SplineBase* CopySelf() const;

	LinearSpline(int close_tolerance = sptol)
		: SplineBase(close_tolerance) {SetType(objt); splinet = linear;}
	LinearSpline(const wxString& name, int close_tolerance = sptol)
		: SplineBase(name, close_tolerance) {SetType(objt); splinet = linear;}
	LinearSpline(const LinearSpline& o)
		: SplineBase(o) {SetType(objt);}
protected:
};

class QuadraticSpline : public SplineBase {
public:
	virtual void CalcCurveCache();
	virtual void Draw(wxDC* dc, const wxRect* bound = 0);
	//virtual void DrawDots(wxDC& dc,wxBrush br,unsigned size = dotsz
	virtual bool Okay() const;
	virtual bool AddPoint(const SplinePoint& pt);
	virtual bool DelPoint(const SplinePoint& pt);
	virtual SplineBase* CopySelf() const;

	QuadraticSpline(int close_tolerance = sptol)
		: SplineBase(close_tolerance) {SetType(objt); splinet = quadratic;}
	QuadraticSpline(const wxString& name, int close_tolerance = sptol)
		: SplineBase(name, close_tolerance) {SetType(objt); splinet = quadratic;}
	QuadraticSpline(const QuadraticSpline& o)
		: SplineBase(o) {SetType(objt);}
protected:
};

class CubicSpline : public SplineBase {
public:
	virtual void CalcCurveCache();
	virtual void Draw(wxDC* dc, const wxRect* bound = 0);
	//virtual void DrawDots(wxDC& dc,wxBrush br,unsigned size = dotsz
	virtual bool Okay() const;
	virtual bool AddPoint(const SplinePoint& pt);
	virtual bool DelPoint(const SplinePoint& pt);
	virtual SplineBase* CopySelf() const;

	CubicSpline(int close_tolerance = sptol)
		: SplineBase(close_tolerance) {SetType(objt); splinet = cubic;}
	CubicSpline(const wxString& name, int close_tolerance = sptol)
		: SplineBase(name, close_tolerance) {SetType(objt); splinet = cubic;}
	CubicSpline(const CubicSpline& o)
		: SplineBase(o) {SetType(objt);}
protected:
};

class BezierSpline : public SplineBase {
public:
	virtual void CalcCurveCache();
	virtual void Draw(wxDC* dc, const wxRect* bound = 0);
	virtual void DrawDots(wxDC& dc,wxBrush br,unsigned size = dotsz);
	virtual void DrawDots(wxDC& dc,unsigned size = dotsz);
	virtual void DrawSelectedDot(wxDC& dc,wxBrush br,const SplinePoint& pt
		,unsigned size = sdotsz);
	virtual void DrawSelectedDot(wxDC& dc,const SplinePoint& pt
		,unsigned size = sdotsz);
	virtual void DrawDotsSelectedDot(wxDC& dc,const SplinePoint& pt
		,wxBrush selectedbr,unsigned size = dotsz,unsigned selsize = sdotsz);
	virtual bool Okay() const;
	virtual bool AddPoint(const SplinePoint& pt);
	virtual bool DelPoint(const SplinePoint& pt);
	virtual bool MovePoint(SplinePoint& pt, double xoff, double yoff);
	virtual SplineBase* CopySelf() const;

	BezierSpline(int close_tolerance = sptol)
		: SplineBase(close_tolerance) {SetType(objt); splinet = bezier;}
	BezierSpline(const wxString& name, int close_tolerance = sptol)
		: SplineBase(name, close_tolerance) {SetType(objt); splinet = bezier;}
	BezierSpline(const BezierSpline& o)
		: SplineBase(o) {SetType(objt);}
protected:
};

// the cached curve point object; keeps reals
class CurvePt {
    double x, y;

    // it might be necessary to set this as a flag rather than
    // a point; using code may interpret the flag any way, but
    // in splines.cpp it is used to indicate a discontinuity
    // in a curve, e.g. a curve with holes
    bool   f; // this is a flag

public:
    CurvePt(bool flag = false) : x(0), y(0), f(flag) {}
    CurvePt(const CurvePt& o)
        : x(o.x), y(o.y), f(o.f) {}
    CurvePt(const CurvePt* o)
        : x(o->x), y(o->y), f(o->f) {}
    CurvePt(double X, double Y, bool flag = false)
        : x(X), y(Y), f(flag) {}
    CurvePt(int X, int Y, bool flag = false)
        : x(double(X)), y(double(Y)), f(flag) {}

    CurvePt& operator = (const CurvePt& o) {
        x = o.x; y = o.y; f = o.f; return *this;
    }
    CurvePt& operator = (const wxRealPoint& o) {
        x = o.x; y = o.y;
        return *this;
    }
    CurvePt& operator = (const wxPoint& o) {
        x = double(o.x); y = double(o.y);
        return *this;
    }
    CurvePt operator + (const wxRealPoint& o) const {
        return CurvePt(x + o.x, y + o.y);
    }
    CurvePt& operator += (const wxRealPoint& o) {
        *this = *this + o;
        return *this;
    }
    CurvePt operator - (const wxRealPoint& o) const {
        return CurvePt(x - o.x, y - o.y);
    }
    CurvePt& operator -= (const wxRealPoint& o) {
        *this = *this - o;
        return *this;
    }
    // defining == and <, include <utility> and use rel_ops for the rest
    bool operator == (const CurvePt& o) const {
        return x == o.x && y == o.y;
    }
    bool operator < (const CurvePt& o) const {
        return
            (x < o.x && y <= o.y) ||
            (x <= o.x && y < o.y) ||
            // this makes little sense, but these ops are really FPO
            ((x*x+y*y) < (o.x*o.x+o.y*o.y))
        ;
    }

    //casts
    operator wxRealPoint () const { return wxRealPoint(x, y); }
    operator wxPoint () const { return wxPoint(irint(x), irint(y)); }

    // simple ops
    void setX(double v) {
        x = v;
    }
    void addX(double v) { setX(x + v); }
    void subX(double v) { setX(x - v); }
    void mulX(double v) { setX(x * v); }
    void divX(double v) { setX(x / v); }
    void setY(double v) {
        y = v;
    }
    void addY(double v) { setY(y + v); }
    void subY(double v) { setY(y - v); }
    void mulY(double v) { setY(y * v); }
    void divY(double v) { setY(y / v); }

    void negate() { setX(-x); setY(-y); }

    double getX() const { return x; }
    double getY() const { return y; }
    int    getXi() const { return irint(x); }
    int    getYi() const { return irint(y); }

    // flag state
    bool isflag() const { return f; }
    void setflag(bool flag = true) { f = flag; }
    void clearflag() { setflag(false); }
};

// Copy SplineBase subclass
inline SplineBase* CopyObj(const SplineBase* o)
{ return o->CopySelf(); }

#endif  // _SPLINES_H_

