<?xml version="1.0"?>
<!DOCTYPE module SYSTEM "../../../tools/rbuild/project.dtd">
<module name="gdi32_winetest" type="win32cui" installbase="bin" installname="gdi32_winetest.exe" allowwarnings="true">
	<include base="gdi32_winetest">.</include>
	<define name="__ROS_LONG64__" />
	<library>ntdll</library>
	<library>gdi32</library>
	<library>user32</library>
	<library>advapi32</library>
	<file>bitmap.c</file>
	<file>brush.c</file>
	<file>clipping.c</file>
	<file>dc.c</file>
	<file>dib.c</file>
	<file>font.c</file>
	<file>gdiobj.c</file>
	<file>generated.c</file>
	<file>icm.c</file>
	<file>mapping.c</file>
	<file>metafile.c</file>
	<file>palette.c</file>
	<file>path.c</file>
	<file>pen.c</file>
	<file>testlist.c</file>
	<file>resource.rc</file>
</module>
