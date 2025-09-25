#ifndef CONFIG_MEMMAP_H
#define CONFIG_MEMMAP_H

/**
 * ------------------------------------------
 * Address  | Size  | Usage
 * ------------------------------------------
 * 080E8414 | 785A8 | Kernel .text
 * 089875AC | 0F000 | Kernel .rodata (secure)
 * 08B2A604 | D59FC | user configurable .rodata (none-secure)
 * 08EFB2E0 | E4D20 | Font .rodata
 * 09000000 | ----  | _unused_
 * ------------------------------------------
 */
#define KernelSpace_Text 0x0E8414


/**
 * Originally 0x785A8 
 * JESTER - This should be roughly enough if you zero out all text strings that belong
 * to the vanilla game, I have extended it because I'm making a rewrite of the vanilla game
 * Do be aware that by default, if you use too much space for text, you will overwrite the limits 
 * of FreeSpace_Common and corrupt battle animations. Which is why I have used FreeSpace_DEMO instead,
 * but for most games the default location is enough
 */
#define KernelSpace_TextSize 0xC85A8 

#define KernelSpace_Data 0x9875AC
#define KernelSpace_DataSize 0xF000

#define FreeSpaceCommon 0xB2A604
#define FreeSpaceCommonSize 0xD59FC

#define FreeSpaceFont 0xEFB2E0
#define FreeSpaceFontSize 0xE4D20

#define FreeSpaceDEMO 0x1000000
// #define FreeSpace FreeSpaceDEMO

/**
 * Secure rodata memmap
 *
 * ------------------------------
 * Name      | Address | Size
 * ------------------------------
 * Magic     | 9875AC  | 00010
 * Reloc     | 9875BC  | 00400
 * Main      | 9879BC  | 0EBF0
 * ------------------------------
 */
#define KernelMagicSize 0x10
#define KernelRelocSize 0x400
#define KernelKDataSize 0xEBF0

#define KernelSpace_Magic (KernelSpace_Data + 0x0)
#define KernelSpace_Reloc (KernelSpace_Data + KernelMagicSize)
#define KernelSpace_KData (KernelSpace_Data + KernelMagicSize + KernelRelocSize)

#define KernelSpace_MagicEnd (KernelSpace_Data + KernelMagicSize)
#define KernelSpace_RelocEnd (KernelSpace_Data + KernelMagicSize + KernelRelocSize)
#define KernelSpace_KDataEnd (KernelSpace_Data + KernelMagicSize + KernelRelocSize + KernelKDataSize)

/**
 * None-secure rodata memmap
 *
 * ------------------------------
 * Name      | Address | Size
 * ------------------------------
 * Magic     | B2A604  | 00010
 * Reloc     | B2A614  | 00400
 * Main      | B2AA14  | CD5E0
 * MSG table | BF7FF4  | 0800C
 * ------------------------------
 */
#define UsrMagicSize 0x10
#define UsrRelocSize 0x400
#define UsrDataSize  0x2FD5E0 // Originally 0xCD5E0
#define MsgTableSize 0x800C

#define FreeSpace_Magic    (FreeSpaceCommon + 0x0)
#define FreeSpace_Reloc    (FreeSpaceCommon + UsrMagicSize)
#define FreeSpace_UsrData  (FreeSpaceDEMO + UsrMagicSize + UsrRelocSize)
#define FreeSpace_MsgTable (FreeSpaceCommon + UsrMagicSize + UsrRelocSize + UsrDataSize)

#define FreeSpace_MagicEnd    (FreeSpaceCommon + UsrMagicSize)
#define FreeSpace_RelocEnd    (FreeSpaceCommon + UsrMagicSize + UsrRelocSize)
// I was originally using FreeSpaceCommon here, but the text extends beyond 0xB2A604 and overwrites battle animations
#define FreeSpace_UsrDataEnd  (FreeSpaceDEMO + UsrMagicSize + UsrRelocSize + UsrDataSize)
#define FreeSpace_MsgTableEnd (FreeSpaceCommon + UsrMagicSize + UsrRelocSize + UsrDataSize + MsgTableSize)

#endif
