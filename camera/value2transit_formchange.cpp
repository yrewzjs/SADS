#include "stdafx.h"
#include "value2transit_formchange.h"

void  MatToCImage(cv::Mat& mat, CImage* cImage)
{
	if (0 == mat.total())
	{
		return;
	}

	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = mat.cols;
	int nHeight = mat.rows;

	//�ؽ�cimage
	cImage->Destroy();
	cImage->Create(nWidth, nHeight, 8 * nChannels);


	//��������
	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)cImage->GetBits();		//ָ����������ָ��
	int nStep = cImage->GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�


	if (1 == nChannels)								//���ڵ�ͨ����ͼ����Ҫ��ʼ����ɫ��
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cImage->GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cImage->SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
}


/***
  @return: X,X,X

**/
CString output_errnum(bool arr[])
{
	CString output;
	char out[256] = "";
	char temp[16] = "";
	for (int i = 0; i < n_qianban; i++)
	{
		if (arr[i] == 1)
		{
			_itoa_s(i, temp, 10);
			strcat_s(temp, ",");
			strcat_s(out, temp);
		}
	}
	output = out;
	return output;
}
