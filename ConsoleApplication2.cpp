// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <time.h>  
#include <afxwin.h>  



/*
* hwnd:要截图的窗口的句柄
* fileName:要比较的图片的路径
* offsets:有4个成员的int型数组，用于设置比较图片时，上、下、左、右的偏移量
* offsets[0]：左
* offsets[1]：上
* offsets[2]：右
* offsets[3]：下
*/

/*
* hwnd:要截图的窗口的句柄
* fileName:要比较的图片的路径
* left:比较图片时的左偏移量
* top:比较图片时的上偏移量
* right:比较图片时的右偏移量
* bottom:比较图片时的下偏移量
*/


void Screen(char* filename)
{
	
	CDC *pDC;//屏幕DC
	pDC = CDC::FromHandle(GetDC(NULL));//获取当前整个屏幕DC
	int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
	int Width = pDC->GetDeviceCaps(HORZRES);
	int Height = pDC->GetDeviceCaps(VERTRES);
	
	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDC, Width, Height);
	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC
	//以下代码保存memDC中的位图到文件
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//获得位图信息
	FILE *fp = fopen(filename, "w+b");
	BITMAPINFOHEADER bih = { 0 };//位图信息头
	bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
	bih.biWidth = bmp.bmWidth;//宽度
	BITMAPFILEHEADER bfh = { 0 };//位图文件头
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
	bfh.bfType = (WORD)0x4d42;
	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头
	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头
	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据
	GetDIBits(memDC.m_hDC, (HBITMAP)memBitmap.m_hObject, 0, Height, p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);//获取位图数据
	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据
	delete[] p;
	fclose(fp);
	memDC.SelectObject(oldmemBitmap);
}

int main(int argc, char * argv[])
{
	
	
	char* filename = argv[1];
	Screen(filename);
	return 0;
}

