/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --font assets/fonts/NotoSansSC-Meltdown.ttf --symbols 今日崩溃次本周较昨长按切换时一下月返回未归档记入保持分开正在校请打小程序蓝牙配网写失败确认回拨期声二三四五六 --font assets/fonts/Rajdhani-Bold.ttf -r 0x20-0x7E --size 12 --bpp 1 --no-compress --format lvgl --lv-font-name meltdown_font_12 --lv-include lvgl.h -o assets/fonts/meltdown_font_12.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef MELTDOWN_FONT_12
#define MELTDOWN_FONT_12 1
#endif

#if MELTDOWN_FONT_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xcf,

    /* U+0022 "\"" */
    0x55,

    /* U+0023 "#" */
    0x36, 0x6f, 0xfb, 0x66, 0xdf, 0xd2, 0x6c,

    /* U+0024 "$" */
    0x23, 0xf3, 0x8f, 0x3c, 0x73, 0xf1, 0x0,

    /* U+0025 "%" */
    0xe8, 0xa8, 0xb8, 0xf0, 0x1f, 0x29, 0x29, 0x2f,

    /* U+0026 "&" */
    0x39, 0xe7, 0x88, 0xdf, 0x4c, 0xbf,

    /* U+0027 "'" */
    0x50,

    /* U+0028 "(" */
    0x6a, 0xaa, 0x40,

    /* U+0029 ")" */
    0xc4, 0x92, 0x49, 0xc0,

    /* U+002A "*" */
    0x4f, 0x64,

    /* U+002B "+" */
    0x31, 0xbe, 0x63, 0x0,

    /* U+002C "," */
    0xf8,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x10, 0x8c, 0x42, 0x31, 0x18,

    /* U+0030 "0" */
    0x76, 0xf7, 0xbd, 0xef, 0x6e,

    /* U+0031 "1" */
    0xfd, 0xb6, 0xdb,

    /* U+0032 "2" */
    0x76, 0xf6, 0x77, 0x73, 0x1f,

    /* U+0033 "3" */
    0xf6, 0xc6, 0x37, 0xf, 0x7f,

    /* U+0034 "4" */
    0x38, 0xe5, 0x96, 0xdb, 0xf1, 0x86,

    /* U+0035 "5" */
    0xfe, 0x31, 0xf9, 0x8f, 0x7e,

    /* U+0036 "6" */
    0x7e, 0x31, 0xfd, 0xef, 0x6e,

    /* U+0037 "7" */
    0xf8, 0xc6, 0x63, 0x11, 0x8c,

    /* U+0038 "8" */
    0x76, 0xf7, 0xb7, 0x6f, 0x7f,

    /* U+0039 "9" */
    0x76, 0xf7, 0xbf, 0x8c, 0x7e,

    /* U+003A ":" */
    0xf0, 0xf0,

    /* U+003B ";" */
    0xf0, 0xf8,

    /* U+003C "<" */
    0x1f, 0xb8, 0x70,

    /* U+003D "=" */
    0xf8, 0x3e,

    /* U+003E ">" */
    0xc3, 0xcf, 0xc0,

    /* U+003F "?" */
    0xf6, 0xc6, 0xe6, 0x1, 0x8c,

    /* U+0040 "@" */
    0x7e, 0xc1, 0xbd, 0xe5, 0xe5, 0xbf, 0xc0, 0x7c,

    /* U+0041 "A" */
    0x38, 0xe2, 0x9a, 0x6d, 0xf4, 0x71,

    /* U+0042 "B" */
    0xf6, 0xf7, 0xbf, 0x6f, 0x7f,

    /* U+0043 "C" */
    0x7e, 0xf7, 0x8c, 0x6f, 0x6f,

    /* U+0044 "D" */
    0xf6, 0xf7, 0xbd, 0xef, 0x7e,

    /* U+0045 "E" */
    0xfe, 0x31, 0x8f, 0x63, 0x1f,

    /* U+0046 "F" */
    0xfe, 0x31, 0x8f, 0x63, 0x18,

    /* U+0047 "G" */
    0x7e, 0xf1, 0x8f, 0xef, 0x6f,

    /* U+0048 "H" */
    0xcf, 0x3c, 0xf3, 0xff, 0x3c, 0xf3,

    /* U+0049 "I" */
    0xff, 0xff,

    /* U+004A "J" */
    0x18, 0xc6, 0x31, 0xef, 0x7e,

    /* U+004B "K" */
    0xcf, 0x6d, 0x3c, 0xf3, 0x6c, 0xf3,

    /* U+004C "L" */
    0xc6, 0x31, 0x8c, 0x63, 0x1f,

    /* U+004D "M" */
    0xc7, 0xdf, 0xbf, 0x7f, 0x7a, 0xf1, 0xe3,

    /* U+004E "N" */
    0xcf, 0x3e, 0xfb, 0xdf, 0x7c, 0xf3,

    /* U+004F "O" */
    0x7b, 0x3c, 0xf3, 0xcf, 0x3c, 0xde,

    /* U+0050 "P" */
    0xfe, 0xf7, 0xbf, 0xe3, 0x18,

    /* U+0051 "Q" */
    0x7b, 0x3c, 0xf3, 0xcf, 0x3c, 0xde, 0x10, 0x60,

    /* U+0052 "R" */
    0xf6, 0xf7, 0xbf, 0x6b, 0x5b,

    /* U+0053 "S" */
    0x7e, 0x71, 0xe7, 0x8e, 0x7e,

    /* U+0054 "T" */
    0xf9, 0x8c, 0x63, 0x18, 0xc6,

    /* U+0055 "U" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x3c, 0xde,

    /* U+0056 "V" */
    0xcf, 0x34, 0xda, 0x69, 0xe3, 0x8c,

    /* U+0057 "W" */
    0xcc, 0xf3, 0x34, 0xc9, 0x7a, 0x73, 0x9c, 0xe7,
    0x39, 0xce,

    /* U+0058 "X" */
    0xcd, 0xb7, 0x8e, 0x39, 0xe6, 0xf3,

    /* U+0059 "Y" */
    0xcf, 0x37, 0x9e, 0x30, 0xc3, 0xc,

    /* U+005A "Z" */
    0xf8, 0xce, 0x66, 0x33, 0x1f,

    /* U+005B "[" */
    0xea, 0xaa, 0xc0,

    /* U+005C "\\" */
    0xc2, 0x18, 0x42, 0x18, 0x42,

    /* U+005D "]" */
    0xd5, 0x55, 0xc0,

    /* U+005E "^" */
    0x10, 0xc6, 0x92,

    /* U+005F "_" */
    0xf0,

    /* U+0060 "`" */
    0xc0,

    /* U+0061 "a" */
    0x7e, 0xf7, 0xbd, 0xfc,

    /* U+0062 "b" */
    0xc6, 0x3f, 0xbd, 0xef, 0x7e,

    /* U+0063 "c" */
    0x7e, 0x31, 0x8c, 0x3c,

    /* U+0064 "d" */
    0x18, 0xff, 0xbd, 0xef, 0x7f,

    /* U+0065 "e" */
    0x76, 0xf7, 0xfc, 0x3c,

    /* U+0066 "f" */
    0x76, 0xf6, 0x66, 0x66,

    /* U+0067 "g" */
    0x7e, 0xf7, 0xbd, 0xfc, 0x7e,

    /* U+0068 "h" */
    0xc6, 0x3f, 0xbd, 0xef, 0x7b,

    /* U+0069 "i" */
    0xcf, 0xff,

    /* U+006A "j" */
    0x30, 0x33, 0x33, 0x33, 0x3e,

    /* U+006B "k" */
    0xc6, 0x37, 0xbf, 0x7b, 0x79,

    /* U+006C "l" */
    0xff, 0xff,

    /* U+006D "m" */
    0xff, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,

    /* U+006E "n" */
    0xfe, 0xf7, 0xbd, 0xec,

    /* U+006F "o" */
    0x76, 0xf7, 0xbd, 0xb8,

    /* U+0070 "p" */
    0xfe, 0xf7, 0xbd, 0xff, 0x18,

    /* U+0071 "q" */
    0x7e, 0xf7, 0xbd, 0xfc, 0x63,

    /* U+0072 "r" */
    0xfc, 0xcc, 0xcc,

    /* U+0073 "s" */
    0xf6, 0x38, 0xe1, 0xf8,

    /* U+0074 "t" */
    0x66, 0xf6, 0x66, 0x63,

    /* U+0075 "u" */
    0xde, 0xf7, 0xbd, 0xfc,

    /* U+0076 "v" */
    0xca, 0x56, 0xb7, 0x18,

    /* U+0077 "w" */
    0xdb, 0x5b, 0x5b, 0x56, 0x76, 0x66,

    /* U+0078 "x" */
    0xdb, 0xcc, 0xe7, 0xec,

    /* U+0079 "y" */
    0x9c, 0xb5, 0xa7, 0x30, 0x9c,

    /* U+007A "z" */
    0xf8, 0xcc, 0xce, 0x7c,

    /* U+007B "{" */
    0x69, 0x28, 0x92, 0x60,

    /* U+007C "|" */
    0xff,

    /* U+007D "}" */
    0xc9, 0x22, 0x92, 0xc0,

    /* U+007E "~" */
    0xf8,

    /* U+4E00 "一" */
    0xff, 0xe0,

    /* U+4E09 "三" */
    0x7f, 0x80, 0x0, 0x0, 0x0, 0x7f, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xc0,

    /* U+4E0B "下" */
    0xff, 0xff, 0xfc, 0x10, 0x2, 0x0, 0x60, 0xb,
    0x1, 0x10, 0x20, 0x4, 0x0, 0x80, 0x10, 0x0,

    /* U+4E8C "二" */
    0x7f, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x7, 0xff,

    /* U+4E94 "五" */
    0x7f, 0xc1, 0x0, 0x20, 0x8, 0x7, 0xf0, 0x22,
    0x4, 0x40, 0x88, 0xff, 0xe0,

    /* U+4ECA "今" */
    0x4, 0x0, 0xe0, 0x19, 0x83, 0xc, 0xc6, 0x30,
    0x20, 0x3f, 0xe0, 0xc, 0x1, 0x80, 0x30, 0x2,
    0x0,

    /* U+4FDD "保" */
    0x0, 0x1, 0xfe, 0x28, 0x22, 0x82, 0x6f, 0xee,
    0xfe, 0x21, 0x3, 0xfe, 0x23, 0x82, 0x54, 0x29,
    0x22, 0x10,

    /* U+5165 "入" */
    0x8, 0x0, 0xc0, 0x4, 0x0, 0x60, 0x6, 0x0,
    0x50, 0x9, 0x0, 0x88, 0x10, 0x42, 0x6, 0x40,
    0x0,

    /* U+516D "六" */
    0xc, 0x0, 0x80, 0x10, 0x7f, 0xf0, 0x0, 0x2,
    0x4, 0x61, 0x4, 0x60, 0x58, 0xe, 0x0, 0x80,

    /* U+5199 "写" */
    0xff, 0xe8, 0x1a, 0x4, 0xfc, 0x20, 0xf, 0xe0,
    0xb, 0xfc, 0x1, 0x0, 0x40, 0x70,

    /* U+5206 "分" */
    0x0, 0x2, 0x20, 0x82, 0x30, 0x6c, 0x7, 0xff,
    0xc2, 0x30, 0x86, 0x10, 0x84, 0x13, 0x1e, 0x0,

    /* U+5207 "切" */
    0x20, 0x4, 0xfc, 0x84, 0x9c, 0x96, 0x12, 0x42,
    0x48, 0x49, 0xd1, 0x62, 0x20, 0x84, 0x23, 0x80,

    /* U+5468 "周" */
    0x7f, 0xec, 0x5, 0xfe, 0xb1, 0x17, 0xfa, 0xc0,
    0x5b, 0xeb, 0x45, 0x68, 0xa9, 0xf6, 0x23, 0x0,

    /* U+56DB "四" */
    0xff, 0xff, 0xe5, 0x32, 0x99, 0x4c, 0xa6, 0x9f,
    0x81, 0x80, 0xff, 0xe0, 0x20,

    /* U+56DE "回" */
    0xff, 0xf8, 0x7, 0x0, 0xef, 0x9d, 0x13, 0xa2,
    0x74, 0x4e, 0xf9, 0xc0, 0x3f, 0xff, 0x0, 0x80,

    /* U+5728 "在" */
    0xc, 0x1, 0x3, 0xff, 0x88, 0x82, 0x10, 0xdf,
    0xe8, 0x41, 0x8, 0x21, 0x4, 0x20, 0xbf, 0x80,

    /* U+58F0 "声" */
    0x4, 0x1f, 0xfc, 0x10, 0x3f, 0xe0, 0x0, 0xff,
    0xd9, 0x1b, 0xff, 0x60, 0x68, 0x3, 0x0, 0x0,

    /* U+5931 "失" */
    0x24, 0x4, 0x81, 0xff, 0x22, 0x0, 0x41, 0xff,
    0xc1, 0x80, 0x50, 0x19, 0xc, 0x1a, 0x0, 0x80,

    /* U+5C0F "小" */
    0x4, 0x0, 0x80, 0x10, 0x12, 0x44, 0x44, 0x88,
    0x91, 0xc, 0x21, 0x4, 0x0, 0x80, 0x70, 0x0,

    /* U+5D29 "崩" */
    0x44, 0x28, 0x85, 0xff, 0xbd, 0xf4, 0xa2, 0xf7,
    0xd2, 0x8b, 0xdf, 0x4e, 0x29, 0xc6, 0x73, 0x80,

    /* U+5E8F "序" */
    0x2, 0x3, 0xff, 0x20, 0x3, 0xfe, 0x20, 0xc2,
    0x70, 0x7f, 0xf6, 0x12, 0x61, 0x44, 0x10, 0x4f,
    0x0,

    /* U+5F00 "开" */
    0x7f, 0xc2, 0x20, 0x44, 0x8, 0x8f, 0xfe, 0x22,
    0x4, 0x41, 0x8, 0x61, 0x18, 0x20,

    /* U+5F52 "归" */
    0x20, 0x29, 0xfa, 0x6, 0x81, 0xa0, 0x69, 0xfa,
    0x4, 0x81, 0x20, 0x53, 0xfc, 0x4,

    /* U+6253 "打" */
    0x20, 0x4, 0xff, 0xe2, 0x10, 0x42, 0x8, 0x71,
    0x38, 0x21, 0x4, 0x20, 0x84, 0x11, 0x8e, 0x0,

    /* U+62E8 "拨" */
    0x20, 0x82, 0x52, 0x25, 0x7, 0x7f, 0x21, 0x2,
    0x3e, 0x72, 0x22, 0x54, 0x28, 0xc2, 0x16, 0x66,
    0x30,

    /* U+6301 "持" */
    0x21, 0x2, 0xfe, 0xf1, 0x2, 0xff, 0x2f, 0xf2,
    0x4, 0xff, 0xf2, 0x44, 0x24, 0x42, 0x4, 0x61,
    0xc0,

    /* U+6309 "按" */
    0x21, 0x4, 0xff, 0xe0, 0x91, 0x12, 0xfe, 0x5f,
    0xcd, 0x17, 0x34, 0x21, 0x84, 0x79, 0xb0, 0x80,

    /* U+6362 "换" */
    0x22, 0x2, 0x7e, 0x7f, 0xe2, 0x7e, 0x25, 0x23,
    0x52, 0xef, 0xe2, 0xfe, 0x22, 0x82, 0x44, 0x68,
    0x20,

    /* U+65E5 "日" */
    0xff, 0xe0, 0xf0, 0x78, 0x3f, 0xfe, 0xf, 0x7,
    0x83, 0xff, 0xe0, 0xc0,

    /* U+65F6 "时" */
    0x0, 0xbc, 0x2b, 0xfe, 0xc2, 0xf4, 0xad, 0x2b,
    0x2a, 0xc2, 0xf0, 0xa0, 0x20, 0x38,

    /* U+6628 "昨" */
    0x8, 0x1d, 0x2, 0xbf, 0xd7, 0xe, 0x7d, 0x4c,
    0x29, 0x85, 0x3e, 0xe6, 0x10, 0xc0, 0x18, 0x0,

    /* U+6708 "月" */
    0x3f, 0xe6, 0xc, 0xc1, 0x9f, 0xf3, 0x6, 0x60,
    0xcf, 0xf9, 0x83, 0x20, 0x6c, 0xf, 0x7, 0x0,

    /* U+671F "期" */
    0x25, 0xef, 0xe4, 0x94, 0x9e, 0xf2, 0x52, 0x7a,
    0x49, 0x7f, 0xf9, 0x1, 0x24, 0xa7, 0x9, 0x80,

    /* U+672A "未" */
    0x4, 0x0, 0x81, 0xff, 0x2, 0x0, 0x41, 0xff,
    0xc3, 0x80, 0xa8, 0x24, 0x98, 0x8c, 0x10, 0x0,

    /* U+672C "本" */
    0x4, 0x0, 0x83, 0xff, 0x87, 0x1, 0x50, 0x2a,
    0x9, 0x22, 0x22, 0xff, 0xe0, 0x80, 0x10, 0x0,

    /* U+6821 "校" */
    0x21, 0x5, 0xff, 0xc0, 0x12, 0x26, 0x86, 0xf9,
    0x18, 0xa5, 0x18, 0xa3, 0x4, 0xd8, 0xa0, 0x80,

    /* U+6863 "档" */
    0x21, 0x2, 0x92, 0xfd, 0x62, 0x14, 0x2f, 0xf7,
    0x3, 0x60, 0x3a, 0xff, 0xa0, 0x32, 0xff, 0x20,
    0x30,

    /* U+6B21 "次" */
    0xc, 0x19, 0x1, 0xbf, 0x84, 0x21, 0x2c, 0x25,
    0x8, 0x82, 0x28, 0x85, 0x91, 0x18, 0xc1, 0x80,

    /* U+6B63 "正" */
    0x7f, 0xe0, 0x20, 0x2, 0x2, 0x20, 0x23, 0xe2,
    0x20, 0x22, 0x2, 0x20, 0xff, 0xf0,

    /* U+6E83 "溃" */
    0x41, 0x5, 0xf8, 0x3f, 0x0, 0x87, 0xfe, 0x1f,
    0x82, 0x11, 0x4a, 0x29, 0x48, 0x51, 0x31, 0x80,

    /* U+7259 "牙" */
    0x7f, 0xcf, 0xf8, 0x4, 0x10, 0x84, 0x10, 0xff,
    0xc1, 0xc0, 0x48, 0x31, 0x18, 0x20, 0x1c, 0x0,

    /* U+786E "确" */
    0x2, 0x3d, 0x4, 0x79, 0x24, 0x4f, 0xfe, 0x59,
    0xfe, 0x65, 0x99, 0x66, 0xff, 0x96, 0x47,

    /* U+7A0B "程" */
    0x37, 0xdc, 0x88, 0x91, 0x7f, 0xe2, 0x0, 0xff,
    0xd8, 0x45, 0x3e, 0x21, 0x4, 0x20, 0xbf, 0x80,

    /* U+7F51 "网" */
    0xff, 0xe0, 0x18, 0x96, 0xd5, 0xb3, 0x64, 0x9a,
    0xb6, 0xb5, 0xc4, 0x60, 0x18, 0x1c,

    /* U+84DD "蓝" */
    0x11, 0x1f, 0xfc, 0x44, 0x29, 0xf5, 0x30, 0xa9,
    0x4, 0x3, 0xfe, 0x4a, 0x4f, 0xfd, 0xff, 0x80,

    /* U+8BA4 "认" */
    0x41, 0x4, 0x20, 0x44, 0x0, 0x8e, 0x10, 0x42,
    0x8, 0x41, 0x14, 0x2a, 0x86, 0xc8, 0x10, 0x80,

    /* U+8BB0 "记" */
    0x0, 0x5, 0xf8, 0x41, 0x0, 0x2e, 0x4, 0x4f,
    0x89, 0x11, 0x20, 0x24, 0x26, 0x84, 0x1f, 0x80,

    /* U+8BF7 "请" */
    0x41, 0x4, 0xf8, 0x1f, 0x80, 0x8e, 0xfe, 0x5f,
    0xcb, 0x19, 0x7f, 0x3f, 0xe5, 0x8c, 0x33, 0x80,

    /* U+8D25 "败" */
    0x7d, 0x4, 0x50, 0x55, 0xf5, 0x62, 0x57, 0x25,
    0x54, 0x54, 0xc1, 0xc, 0x28, 0xc4, 0x52, 0x0,
    0x10,

    /* U+8F83 "较" */
    0x40, 0x8, 0x23, 0xff, 0x32, 0x4a, 0x45, 0xf9,
    0x8, 0xa1, 0x18, 0xe1, 0x4, 0x50, 0xb1, 0x0,

    /* U+8FD4 "返" */
    0x0, 0x24, 0x7c, 0x24, 0x0, 0xfe, 0xee, 0x42,
    0xdc, 0x28, 0x83, 0x96, 0x22, 0x45, 0x0, 0x8f,
    0xe0,

    /* U+914D "配" */
    0xfb, 0xcc, 0x9, 0x81, 0x4c, 0x2e, 0xbd, 0xd4,
    0xbe, 0x84, 0x50, 0xfa, 0x3f, 0x46, 0x2f, 0x0,

    /* U+957F "长" */
    0x20, 0x84, 0x20, 0x88, 0x16, 0x2, 0x1, 0xff,
    0x89, 0x1, 0x10, 0x23, 0x4, 0xb0, 0xe1, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 42, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 46, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 68, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 4, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 11, .adv_w = 101, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 18, .adv_w = 142, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 26, .adv_w = 119, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 32, .adv_w = 36, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 33, .adv_w = 59, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 36, .adv_w = 59, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 40, .adv_w = 76, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 42, .adv_w = 108, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 46, .adv_w = 39, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 47, .adv_w = 60, .box_w = 3, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 48, .adv_w = 39, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 49, .adv_w = 79, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 54, .adv_w = 103, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 59, .adv_w = 64, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 95, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 67, .adv_w = 98, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 72, .adv_w = 104, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 78, .adv_w = 97, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 83, .adv_w = 101, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 83, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 93, .adv_w = 104, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 101, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 103, .adv_w = 39, .box_w = 2, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 39, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 107, .adv_w = 108, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 110, .adv_w = 109, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 112, .adv_w = 109, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 115, .adv_w = 89, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 143, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 128, .adv_w = 107, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 107, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 103, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 144, .adv_w = 106, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 92, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 86, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 159, .adv_w = 105, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 111, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 170, .adv_w = 51, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 100, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 107, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 183, .adv_w = 86, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 188, .adv_w = 139, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 106, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 102, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 212, .adv_w = 109, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 220, .adv_w = 105, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 103, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 85, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 235, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 241, .adv_w = 100, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 155, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 104, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 99, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 100, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 274, .adv_w = 62, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 277, .adv_w = 79, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 282, .adv_w = 62, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 285, .adv_w = 98, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 288, .adv_w = 71, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 289, .adv_w = 57, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 290, .adv_w = 99, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 294, .adv_w = 101, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 79, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 303, .adv_w = 101, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 95, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 312, .adv_w = 68, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 100, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 321, .adv_w = 100, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 47, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 328, .adv_w = 47, .box_w = 4, .box_h = 10, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 333, .adv_w = 91, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 338, .adv_w = 47, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 340, .adv_w = 153, .box_w = 8, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 346, .adv_w = 100, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 350, .adv_w = 98, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 354, .adv_w = 101, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 359, .adv_w = 99, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 364, .adv_w = 67, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 84, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 371, .adv_w = 68, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 375, .adv_w = 100, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 379, .adv_w = 90, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 383, .adv_w = 134, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 389, .adv_w = 89, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 90, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 398, .adv_w = 86, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 402, .adv_w = 60, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 406, .adv_w = 44, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 60, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 411, .adv_w = 108, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 412, .adv_w = 192, .box_w = 11, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 414, .adv_w = 192, .box_w = 10, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 426, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 442, .adv_w = 192, .box_w = 11, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 453, .adv_w = 192, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 466, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 483, .adv_w = 192, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 501, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 518, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 534, .adv_w = 192, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 548, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 564, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 580, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 596, .adv_w = 192, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 609, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 625, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 641, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 657, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 673, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 689, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 705, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 722, .adv_w = 192, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 736, .adv_w = 192, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 750, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 766, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 783, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 800, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 816, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 833, .adv_w = 192, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 845, .adv_w = 192, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 859, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 875, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 891, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 907, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 923, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 939, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 955, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 972, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 988, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1002, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1018, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1034, .adv_w = 192, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1049, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1065, .adv_w = 192, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1079, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1095, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1111, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1127, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1143, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1160, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1176, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1193, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 1209, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x9, 0xb, 0x8c, 0x94, 0xca, 0x1dd, 0x365,
    0x36d, 0x399, 0x406, 0x407, 0x668, 0x8db, 0x8de, 0x928,
    0xaf0, 0xb31, 0xe0f, 0xf29, 0x108f, 0x1100, 0x1152, 0x1453,
    0x14e8, 0x1501, 0x1509, 0x1562, 0x17e5, 0x17f6, 0x1828, 0x1908,
    0x191f, 0x192a, 0x192c, 0x1a21, 0x1a63, 0x1d21, 0x1d63, 0x2083,
    0x2459, 0x2a6e, 0x2c0b, 0x3151, 0x36dd, 0x3da4, 0x3db0, 0x3df7,
    0x3f25, 0x4183, 0x41d4, 0x434d, 0x477f
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 19968, .range_length = 18304, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 53, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t meltdown_font_12 = {
#else
lv_font_t meltdown_font_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 13,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if MELTDOWN_FONT_12*/

