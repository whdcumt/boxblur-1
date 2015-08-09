#include <stdio.h>
#include <malloc.h>
#include <assert.h>
typedef unsigned char UINT8;
typedef long long LL;
void boxfilter(int* const srcImg, int* const desImg, const int mask, const int nrows, const int ncols)
{
	LL  *buffer,  *rowImg, *rowPtr;
	int *srcPtr, *desPtr;
	LL pre, sum;
	int row, col, i, len, r,j;
	const int MAX_ROW_COL = nrows > ncols ? nrows : ncols, MASK_SIZE = mask*mask;

	r = mask / 2;

	rowImg = (LL *)malloc(sizeof(LL)*nrows*ncols);
	buffer = (LL *)malloc(sizeof(LL)*(MAX_ROW_COL + mask));

	len = ncols + 2 * r;
	srcPtr = srcImg;
	rowPtr = rowImg;
	LL *headL, *tailL;
	int *head, *tail;
	for (row = 0; row < nrows; ++row)
	{
		head = srcPtr + r;
		tail = srcPtr + (ncols - 2 - r);
		for (i = 0; i < r; ++i)
		{
			buffer[i] = *head;//*(srcPtr + row*ncols + r + i);
			buffer[len - 1 - i] = *tail;
			++head;
			--tail;
		}

		for (i = r; i < len - r; ++i)
		{
			buffer[i] = *srcPtr;
			++srcPtr;
		}

		sum = buffer[0];
		for (i = 1; i < mask; ++i)
		{
			sum += buffer[i];
		}
		*rowPtr = sum;
		++rowPtr;
		pre = sum;

		headL = buffer;
		tailL = buffer + mask;
		for (i = mask; i < len; ++i, ++headL, ++tailL)
		{
			*rowPtr = pre;
			++rowPtr;
			pre = pre - *headL + *tailL;

		}


	}
	len = nrows + 2 * r;

	rowPtr = rowImg;
	desPtr = desImg;
	for (col = 0; col < ncols; ++col)
	{
		rowPtr = rowImg + col;

		headL = rowPtr + r*ncols;
		tailL = rowPtr + (nrows - 2 - r)*ncols;
		for (i = 0; i < r; ++i)
		{
			buffer[i] = *headL;//*(rowPtr + (r + i)*ncols + col);
			buffer[len - 1 - i] = *tailL;//*(rowPtr + (nrows - 2 - i - r)*ncols + col);
			headL += ncols;
			tailL -= ncols;
		}

		for (i = r; i < len - r; ++i)
		{
			buffer[i] = *rowPtr;//*(rowPtr + (i - r)*ncols + col);
			rowPtr += ncols;
		}

		sum = buffer[0];
		for (i = 1; i < mask; ++i)
		{
			sum += buffer[i];
		}
		desPtr = desImg + col;
		*desPtr = sum / MASK_SIZE;
		pre = sum;
		headL = buffer;
		tailL = buffer + mask;
		for (i = mask; i < len; ++i, ++headL, ++tailL)
		{
			//*(desPtr + (i - mask + 1)*ncols + col) = pre / MASK_SIZE;
			*desPtr = pre / MASK_SIZE;
			desPtr += ncols;
			pre = pre - *headL + *tailL;
		}
	}

	free(rowImg);
	free(buffer);
	return;
}

int main()
{
    //load an 1024*768 image
    FILE *srcFile=fopen("./forest1_gray.raw","rb"),*desFile=fopen("boxlur.raw","wb");
    int Height=768,Width=1024,i,j;
    UINT8 *img=(UINT8*)malloc(sizeof(UINT8)*Height*Width),*desImg=(UINT8*)malloc(sizeof(UINT8)*Height*Width);
    fread(img,sizeof(UINT8),Height*Width,srcFile);

    int *srcImg=(int *)malloc(sizeof(int)*Height*Width);
    assert(srcFile!=NULL);
    fclose(srcFile);
    for (i=0; i<Height; ++i)
        for (j=0; j<Width; ++j)
        {
            srcImg[i*Width+j]=img[i*Width+j];

        }
    //for (i=0;i<50;++i)
    boxfilter(srcImg,srcImg,7,Height,Width);
    for (i=0; i<Height; ++i)
        for (j=0; j<Width; ++j)
        {
            desImg[i*Width+j]=(UINT8)srcImg[i*Width+j];
        }
    fwrite(desImg,sizeof(UINT8),Height*Width,desFile);
    fclose(desFile);
    free(img);
    free(srcImg);
    free(desImg);

    return 0;
}
