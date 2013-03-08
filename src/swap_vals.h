#if 0 // here C++ is helpful . . .
#	define swap_vals(v1, v2) \
	do { \
		v1 ^= v2; v2 ^= v1; v1 ^= v2; \
	} while ( 0 )
#else // . . . with specialization
template<typename T> inline void swap_vals(T& v1, T& v2)
{
	T t = v1; v1 = v2; v2 = t;
}
template<typename T> inline void swap_vals_int(T& v1, T& v2)
{
	v1 ^= v2; v2 ^= v1; v1 ^= v2;
}
template<> inline void swap_vals(signed char& v1, signed char& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned char& v1, unsigned char& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(signed short& v1, signed short& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned short& v1, unsigned short& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(signed int& v1, signed int& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned int& v1, unsigned int& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(signed long& v1, signed long& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned long& v1, unsigned long& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(signed long long& v1, signed long long& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(unsigned long long& v1, unsigned long long& v2)
{ swap_vals_int(v1, v2); }
template<> inline void swap_vals(bool& v1, bool& v2)
{ swap_vals_int(v1, v2); }
#endif

