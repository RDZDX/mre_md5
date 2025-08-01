"C:\Program Files\MRE_SDK\tools\DllPackage.exe" "D:\MyGitHub\mre_md5\mre_md5.vcproj"
if %errorlevel% == 0 (
 echo postbuild OK.
  copy mre_md5.vpp ..\..\..\MoDIS_VC9\WIN32FS\DRIVE_E\mre_md5.vpp /y
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)

