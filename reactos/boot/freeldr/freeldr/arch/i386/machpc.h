/* $Id$
 *
 *  FreeLoader
 *
 *  Copyright (C) 2003  Eric Kohl
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __I386_MACHPC_H_
#define __I386_MACHPC_H_

#ifndef __MEMORY_H
#include "mm.h"
#endif

VOID PcMachInit(const char *CmdLine);

VOID PcConsPutChar(int Ch);
BOOL PcConsKbHit();
int PcConsGetCh();

VOID PcVideoClearScreen(UCHAR Attr);
VIDEODISPLAYMODE PcVideoSetDisplayMode(char *DisplayMode, BOOL Init);
VOID PcVideoGetDisplaySize(PULONG Width, PULONG Height, PULONG Depth);
ULONG PcVideoGetBufferSize(VOID);
VOID PcVideoSetTextCursorPosition(ULONG X, ULONG Y);
VOID PcVideoHideShowTextCursor(BOOL Show);
VOID PcVideoPutChar(int Ch, UCHAR Attr, unsigned X, unsigned Y);
VOID PcVideoCopyOffScreenBufferToVRAM(PVOID Buffer);
BOOL PcVideoIsPaletteFixed(VOID);
VOID PcVideoSetPaletteColor(UCHAR Color, UCHAR Red, UCHAR Green, UCHAR Blue);
VOID PcVideoGetPaletteColor(UCHAR Color, UCHAR* Red, UCHAR* Green, UCHAR* Blue);
VOID PcVideoSync(VOID);
VOID PcVideoPrepareForReactOS(VOID);

ULONG PcMemGetMemoryMap(PBIOS_MEMORY_MAP BiosMemoryMap, ULONG MaxMemoryMapSize);

BOOL PcDiskReadLogicalSectors(ULONG DriveNumber, ULONGLONG SectorNumber, ULONG SectorCount, PVOID Buffer);
BOOL PcDiskGetPartitionEntry(ULONG DriveNumber, ULONG PartitionNumber, PPARTITION_TABLE_ENTRY PartitionTableEntry);
BOOL PcDiskGetDriveGeometry(ULONG DriveNumber, PGEOMETRY DriveGeometry);
ULONG PcDiskGetCacheableBlockCount(ULONG DriveNumber);

VOID PcRTCGetCurrentDateTime(PULONG Year, PULONG Month, PULONG Day, PULONG Hour, PULONG Minute, PULONG Second);

VOID PcHwDetect(VOID);

#endif /* __I386_MACHPC_H_ */

/* EOF */
