/**********************************************************************\
  *     stack (auto) object to temporarily set LC_ALL to "C" for     *
  *     suspension of localisation in some block of code             *
\**********************************************************************/

#ifndef __C_LOCAL_ALL_TEMP__
#define __C_LOCAL_ALL_TEMP__

#include <clocale>
#include <string>

template<int C> class C_loc_Temp {
protected:
	std::string orig;
public:
	C_loc_Temp() {
		const char* p = std::setlocale(C, NULL);
		orig = p ? p : "";
		std::setlocale(C, "C");
	}
	~C_loc_Temp() {
		std::setlocale(C, orig.c_str());
	}

	// possibly useful sometime:
	const char* get_orig() const { return orig.c_str(); }
};

typedef C_loc_Temp<LC_ALL> C_All_loc_Temp;

// For brevity where desired:
typedef C_All_loc_Temp c_alltmp;

#endif // __C_LOCAL_ALL_TEMP__
