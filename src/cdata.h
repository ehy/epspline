/**********************************************************************\
 * cdata.h -- common data for application Epspline                 
 * Generated using cdata.m4 from cdata.h.in                    
 *                                                                    
 * Copyright (C) 2012 Ed Hynan                                        
 *                                                                    
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *                                                                    
\**********************************************************************/

#ifndef	_CDATA_H_
#define	_CDATA_H_		327680


#ifndef	APPNAME_IN_ASCII
#define	APPNAME_IN_ASCII		"epspline"	/* name, lowercase, e.g. executable name */
#endif	/* #ifndef APPNAME_IN_ASCII */

#ifndef	APPCLASS_IN_ASCII
#define	APPCLASS_IN_ASCII		"Epspline"	/* name, Initial Cap, of e.g. executable name */
#endif	/* #ifndef APPCLASS_IN_ASCII */

#ifndef	VENDORNAME
#define	VENDORNAME		"GPLFreeSoftwareApplications"	/* Vendor, e.g. used in MSW registry */
#endif	/* #ifndef VENDORNAME */

#ifndef	APPVERSIONSTR
#define	APPVERSIONSTR		"0.5.0.0"	/* application version as string */
#endif	/* #ifndef APPVERSIONSTR */

#ifndef	APPVERSION
#define	APPVERSION		327680	/* numeric version: major<<24|minor<<16|revisionmajor<<8|revisionminor */
#endif	/* #ifndef APPVERSION */

#if defined(__WXMSW__)
#define	HELP_DOC_SUBDIR		"Help Documents"
#else
#define	HELP_DOC_SUBDIR		"doc"
#endif

/* end cdata.h */
#endif	/* _CDATA_H_ */

