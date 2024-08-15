#include <setjmp.h>
#include <stdio.h>
#include "jpeglib.h"

// 声明外部变量，在其他文件中定义
extern int (*lcd)[800];

struct my_error_mgr {
  struct jpeg_error_mgr pub; /* "public" fields */

  jmp_buf setjmp_buffer; /* for return to caller */
};

typedef struct my_error_mgr* my_error_ptr;

// 错误退出处理函数
METHODDEF(void)
my_error_exit(j_common_ptr cinfo) {
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr)cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message)(cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

// jpeg解压例子
GLOBAL(int)
read_JPEG_file(char* filename) {
  // 定义一个jpeg解压参数结构体
  struct jpeg_decompress_struct cinfo;

  // 定义一个错误处理结构体
  struct my_error_mgr jerr;

  /* More stuff */
  FILE* infile;      /* 源文件*/
  JSAMPARRAY buffer; /* 输出行缓存 */
  int row_stride;    /* 输出行缓存的大小 */

  // 打开一个源文件
  if ((infile = fopen(filename, "rb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return 0;
  }

  /* Step 1:分配和初始化jpeg解压对象*/
  /* 注册一个错误退出函数 */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;  // 初始化错误处理函数

  // 判断是否出错
  if (setjmp(jerr.setjmp_buffer)) {
    // 释放资源
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return 0;
  }

  /*初始化jpeg解压结构体*/
  jpeg_create_decompress(&cinfo);

  /*关系解码文件和解码结构体*/
  jpeg_stdio_src(&cinfo, infile);

  /*读取jpeg图片的头数据*/
  (void)jpeg_read_header(&cinfo, TRUE);

  /*开始jpeg解码*/
  (void)jpeg_start_decompress(&cinfo);

  /*计算一行jpeg图片的字节大小*/
  row_stride = cinfo.output_width * cinfo.output_components;

  // cinfo.output_width   JPEG 图片的宽度
  // cinfo.output_components JPEG 图片的色深

  // 输出jpeg 图片的信息
  printf("%s 宽度:%d 高度:%d 色深:%d\n", filename, cinfo.output_width,
         cinfo.output_height, cinfo.output_components);

  /*分配一行解码数据的缓存区*/
  buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE,
                                      row_stride, 1);

  // buffer 存储jpeg 图片解码后的数据

  // cinfo.output_scanline 当前的解码行号
  // cinfo.output_height   jpeg图片的高度

  // 定义像素点的缓存区
  int color[cinfo.output_height][cinfo.output_width];
  int y = 0;

  while (cinfo.output_scanline < cinfo.output_height)  // 一行一行的进行解码
  {
    // 把1行jpeg图片的数据转换为1行rgb数据
    // 存储到 buffer 缓存中
    (void)jpeg_read_scanlines(&cinfo, buffer, 1);

    char* p = buffer[0];  // buffer[0] 把二级地址转换为一级

    // 转换一行rgb数据
    for (int x = 0; x < cinfo.output_width; x++) {
      char r = *p++;
      char g = *p++;
      char b = *p++;

      int c = r << 16 | b << 8 | g;

      color[y][x] = c;  // 存储转换后的像素数据
    }

    /* Assume put_scanline_someplace wants a pointer and sample count. */
    // put_scanline_someplace(buffer[0], row_stride); //
    // 把一行数据输出到某个地方
    y++;  // 增加下一行
  }

  // 把color 的像素数据放入映射地址中
  for (int y = 0; y < 480; y++) {
    for (int x = 0; x < 800; x++) {
      lcd[y][x] = color[y][x];
    }
  }

  /* Step 7: 结束解码 */
  (void)jpeg_finish_decompress(&cinfo);

  /* 销毁解码对象*/
  jpeg_destroy_decompress(&cinfo);

  // 关闭文件
  fclose(infile);

  return 1;
}