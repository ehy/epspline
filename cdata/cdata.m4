dnl Epspline application common data m4 macros
dnl Copyright (C) 2012 Ed Hynan
dnl
dnl  This program is free software: you can redistribute it and/or modify
dnl  it under the terms of the GNU General Public License as published by
dnl  the Free Software Foundation, either version 2 of the License, or
dnl  (at your option) any later version.
dnl
dnl  This program is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl  GNU General Public License for more details.
dnl
dnl  You should have received a copy of the GNU General Public License
dnl  along with this program.  If not, see <http://www.gnu.org/licenses/>.
dnl
dnl
dnl
dnl general utility defs
dnl
define(cdataName, cdata)dnl
define(cdataNameS, `"cdataName"')dnl
define(cppdefine, `#define	'$1`		'$2`'$3)dnl
define(cppifndefine, `#ifndef	'`$1'
cppdefine($1, $2, $3)
`#endif	/* #ifndef '$1` */')dnl 				end cppifndefine
define(cppheaderguard, `#ifndef	'`$1'
cppdefine($1, $2, $3)
)dnl 											end cppheaderguard
dnl
dnl
dnl application attributes defs
dnl
define(appname, `epspline')dnl
define(appnameS, `"appname"')dnl				e.g. executable name
define(appName, `Epspline')dnl
define(appNameS, `"appName"')dnl				name w/ initial cap
define(appclass, `Epspline')dnl
define(appclassS, `"appclass"')dnl				appname with initial cap
define(licenseFile, `LICENSE.gpl,v3')dnl		name of file w/ GPLv3
define(vendorname,
	`GPLFreeSoftwareApplications')dnl			used in MSW registry
define(vendornameS, `"vendorname"')dnl
define(mswinstallername,
	appName`-'appversion`-msw32-install.exe')dnl	name of MSW install prog
define(mswinstallernameS,
	"mswinstallername")dnl
define(mswinstallername64,
	appName`-'appversion`-msw64-install.exe')dnl	name of MSW64bit install prog
define(mswinstallername64S,
	"mswinstallername64")dnl
define(mswinstalldir, `appclass appversion')dnl	top MSW install dir
dnl define(mswinstalldir, `appclass')dnl			top MSW install dir
define(mswinstalldirS, "mswinstalldir")dnl
define(progprettyname,
	`appclass Prism and Lathe Editor')dnl		e.g. startfolder on MSW
define(progprettynameS, "progprettyname")dnl
define(mswhelpdir, `Help Documents')dnl			help docs dir on MSW
define(mswhelpdirS, "mswhelpdir")dnl
define(genhelpdir, `doc')dnl					doc dir on e.g. Unix
define(genhelpdirS, "genhelpdir")dnl
define(mswsrc_dir, `Source Code')dnl			src zip dir on MSW
define(mswsrc_dirS, "mswsrc_dir")dnl
define(mswxmpldir, `Examples')dnl				examples dir on MSW
define(mswxmpldirS, "mswxmpldir")dnl
define(genFileExtND, `pse')dnl					prog's data file extens.
define(genFileExt, `.genFileExtND')dnl			prog's data file extens.
define(genFileExtNDS, "genFileExtND")dnl					prog's data file extens.
define(genFileExtS, "genFileExt")dnl
define(genFileDesc, `progprettyname')dnl		prog's data file descr.
define(genFileDescS, "genFileDesc")dnl
dnl
dnl app version piece by piece
define(appVMajor, `0')dnl
define(appVMinor, `0')dnl
define(appRMajor, `4')dnl
define(appRMinor, `5')dnl
dnl comment appPatchLvl macro for version string w/o "pN" appended
dnl define(appPatchLvl, `1')dnl
define(appPatchLvl, `1')dnl
define(appversionI,
	eval(appVMajor<<24|appVMinor<<16|appRMajor<<8|appRMinor))dnl
define(appversion,
appVMajor.appVMinor.appRMajor.appRMinor`ifdef(`appPatchLvl', `p'appPatchLvl)')dnl
define(appversionS, `"appversion"')dnl
