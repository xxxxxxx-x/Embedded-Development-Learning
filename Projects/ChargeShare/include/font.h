#ifndef __font_h__
#define __font_h__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#define color u32
#define getColor(a, b, c, d) (a | b << 8 | c << 16 | d << 24)
/***************************************数据类型***************************************/
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

// 定义一个结构体存储字体文件信息
typedef struct stbtt_fontinfo
{
	void *userdata;		 // 用户数据
	unsigned char *data; //.ttf 文件指针
	int fontstart;		 // 字体开始的偏移量

	int numGlyphs; // 符号的数量，需要范围检查

	int loca, head, glyf, hhea, hmtx, kern; // 表位置从.ttf开始的偏移量
	int index_map;							// 所选字符编码的cmap映射
	int indexToLocFormat;					// 从字形索引映射到字形所需的格式

} stbtt_fontinfo;
// 定义一个结构体存储位图信息
typedef struct
{
	u32 height;		  // 画布高度
	u32 width;		  // 画布宽度
	u32 byteperpixel; // 每像素的字节数
	u8 *map;		  // 位图数据指针
} bitmap;
// 定义一个结构体存储字体信息
typedef struct
{
	stbtt_fontinfo *info; // 字体信息
	u8 *buffer;			  // 字体文件缓冲区
	float scale;		  // 字体缩放比例
} font;

// lcd设备结构体
struct LcdDevice
{
	int fd;			  // 文件描述符
	unsigned int *mp; // 保存映射首地址
};
/***************************************END******************************************/

/*******************************初始化画布和字体相关信息*******************************/

/**
 * @name      fontLoad
 * @brief     1.加载TrueType字体文件  初始化字库  打开开发板路径下TrueType字库
 * @param     fontPath 指向字体文件路径的字符串 例如 "./font/HYZhengYuan55W.ttf"
 * @return
 *      @retval    成功则返回字体信息结构体的指针，失败则返回NULL
 * @note      在程序结束之前，需要手动释放申请的堆内存地址
 * 使用:      font *f = fontLoad("./font/HYZhengYuan55W.ttf");
 */
font *fontLoad(char *fontPath);

/**
 * @name      fontSetSize
 * @brief     2.设置字体大小
 * @param     f 指向字体信息结构体的指针
 * @param     pixels 字体大小（像素） 例如72像素
 * @note      调用此函数来设置字体的显示大小
 * 			  若字体大小 > 画布(输出框) 则从左到右,从上到下,超出的部分会无法显示
 * 			  可以理解为视窗, 所以字体要<=画布大小
 * 使用:       fontSetSize(f, 100);
 */
void fontSetSize(font *f, s32 pixels);

/**
 * @name      createBitmap
 * @brief     创建一个位图画布 设置字体输出框的大小
 * @param     width 画布的宽度
 * @param     height 画布的高度
 * @param     byteperpixel 每像素的字节数 必须为4
 * @return
 *      @retval    成功则返回位图画布结构体的指针，失败则返回NULL
 * @note      在程序结束之前，需要手动释放申请的堆内存地址
 * 注意:      创建画布大小, 每个像素字节数为4
 * 			  画布大小即视窗大小
 * 			  不能调整颜色, 默认白色! 建议用下面的
 * 使用:bitmap *bm = createBitmap(100, 100, 4);
 */
bitmap *createBitmap(u32 width, u32 height, u32 byteperpixel);

/**
 * @name      createBitmapWithInit
 * @brief     创建一个位图画布并初始化颜色 可以指定输出框的颜色
 * @param     width 画布的宽度
 * @param     height 画布的高度
 * @param     byteperpixel 每像素的字节数
 * @param     c 初始化颜色
 * @return
 *      @retval    成功则返回位图结构体的指针，失败则返回NULL
 * @note      在程序结束之前，需要手动释放申请的堆内存地址
 * 			  和上面一样, 建议用这个,因为可以调颜色
 * 			  当宽80 长200时就是典型的垂直视窗
 * 使用:    bitmap *bm = createBitmapWithInit(80, 200, 4, getColor(0, 255, 255, 255));
 */
bitmap *createBitmapWithInit(u32 width, u32 height, u32 byteperpixel, color c);
/***************************************END******************************************/
/***************************************显示生成*************************************/
// 文本--->画布--->栅格化以画布为输出
/**
 * @name      fontPrint
 * @brief     将文本输出到位图画布中
 * @param     f 指向字体信息结构体的指针
 * @param     screen 指向位图结构体的指针
 * @param     x 文本在画布内的相对起始x坐标 可以理解为画布就是小屏幕 在画布内移动
 * @param     y 文本在画布内的相对起始y坐标
 * @param     text 要输出的文本字符串
 * @param     c 文本颜色
 * @param     maxWidth 文本的最大宽度 即一行最大显示多大字符, 当和文本一样宽都是72时,则表示一行一个字, 垂直显示
 * @note      调用此函数在位图画布上绘制文本
 * 使用:  fontPrint(f, bm, 0, 20, buf, getColor(0, 255, 255, 0), 72);
 */
void fontPrint(font *f, bitmap *screen, s32 x, s32 y, char *text, color c, s32 maxWidth);

/**
 * @name      show_font_to_lcd
 * @brief     将位图画布显示到LCD屏幕上
 * @param     p 指向LCD屏幕映射内存的指针
 * @param     px 画布在LCD屏幕上的起始x坐标
 * @param     py 画布在LCD屏幕上的起始y坐标
 * @param     bm 指向位图结构体的指针
 * @note      调用此函数将位图画布内容显示到LCD屏幕上
 * 使用:  show_font_to_lcd(lcd->mp, 400, 200, bm);
 */
void show_font_to_lcd(unsigned int *p, int px, int py, bitmap *bm);

/**
 * @name      fontUnload
 * @brief     释放字体信息结构体  关闭字体库
 * @param     f 指向字体信息结构体的指针
 * @note      调用此函数释放字体信息结构体占用的内存
 */
void fontUnload(font *f);

/**
 * @name      destroyBitmap
 * @brief     释放位图结构体 关闭bitmap
 * @param     bm 指向位图结构体的指针
 * @note      调用此函数释放位图结构体占用的内存
 */
void destroyBitmap(bitmap *bm);

#endif