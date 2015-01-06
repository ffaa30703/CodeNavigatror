/*
 * Des.c
 *
 *  Created on: 2012-11-8
 *      Author: root
 */

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>
#include "Des.h"

char* pcDesKey = "123456789012345678901234";
char acDesIv[8]={0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8};
int iPadMode = PAD_PKCS_7;

char* ch64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char IP_Table[64]=
{
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};
const char IPR_Table[64] =
{
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41,  9, 49, 17, 57, 25
};

const char E_Table[48] =
{
	32,  1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

const char P_Table[32] =
{
	16, 7, 20, 21,
	29, 12, 28, 17,
	1,  15, 23, 26,
	5,  18, 31, 10,
	2,  8, 24, 14,
	32, 27, 3,  9,
	19, 13, 30, 6,
	22, 11, 4,  25
};

const char PC1_Table[56] =
{
	57, 49, 41, 33, 25, 17,  9,
	1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

const char PC2_Table[48] =
{
	14, 17, 11, 24,  1,  5,
	3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

const char LOOP_Table[16] =
{
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

const char S_Box[8][4][16] =
{
	{
		{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
		{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
		{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
		{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
	},

	{
		{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
		{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
		{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
		{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
	},

	{
		{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
		{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
		{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
		{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
	},

	{
		{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
		{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
		{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
		{ 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
	},

	{
		{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
		{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
		{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
		{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
	},

	{
		{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
		{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
		{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
		{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
	},

	{
		{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
		{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
		{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
		{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
	},

	{
		{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
		{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
		{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
		{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
	}
};

void F_func(char In[32], const char Ki[48]);
void S_func(char Out[32], const char In[48]);
void Transform(char *Out, const char *In, const char *Table, int len);
void Xor(char *InA, const char *InB, int len);
void RotateL(char *In, int len, int loop);
void BitToByte(char *Out, const char *In, int bits);
void ByteToBit(char *Out, const char *In, int bits);
void SetSubKey(PSubKey pSubKey, const char Key[8]);

char* Base64Decode(char *src)
{
	int m,n,i,j,len;
	char *p;
	char *dst;
	char strbuf[256];

	if (src == 0 || src[0] == 0)
		return NULL;
	len = strlen(src);
	if (len % 4)
		return NULL;

	for (i = 0; i < len-2; i++)
		if (src[i] == '=')
			return NULL;

		memset(strbuf,0,sizeof(strbuf));
		strcpy(strbuf,src);
		n=strlen(src);
		for (i=0; i<n; i++) /* map base64 ASCII character to 6 bit value */
		{
			p=strchr(ch64,src[i]);
			if (!p)
				break;
			src[i]=p-ch64;
		}
		dst=(char*)malloc(n*3/4+1);
		memset(dst,0,n*3/4+1);
		for (i=0,j=0; i<n; i+=4,j+=3)
		{
			dst[j]=(src[i]<<2) + ((src[i+1]&0x30)>>4);
			dst[j+1]=((src[i+1]&0x0F)<<4) + ((src[i+2]&0x3C)>>2);
			dst[j+2]=((src[i+2]&0x03)<<6) + src[i+3];
		}
		m=strcspn(strbuf,"=");
		for (i=0; i<n-m; i++)
			dst[j-i-1]=0x00;

	return dst;
}


void SetSubKey(PSubKey pSubKey, const char Key[])
{
	int i;
	char K[64] = {0}, *KL=&K[0], *KR=&K[28];

	ByteToBit(K, Key, 64);

	Transform(K, K, PC1_Table, 56);

	for ( i=0; i<16; ++i)
	{
		RotateL(KL, 28, LOOP_Table[i]);
		RotateL(KR, 28, LOOP_Table[i]);
		Transform((*pSubKey)[i], K, PC2_Table, 48);
	}
}

void ByteToBit(char *Out, const char *In, int bits)
{
	int i;
	for (i=0; i<bits; ++i)
		Out[i] = (In[i>>3]>>(7 - i&7)) & 1;
}

void BitToByte(char *Out, const char *In, int bits)
{
	int i;
	memset(Out, 0, bits>>3);
	for (i=0; i<bits; ++i) Out[i>>3] |= In[i]<<(7 - i&7);
}

void RotateL(char *In, int len, int loop)
{
	char szTmp[256] = {0};

	if (len >= 256) return;
	if (loop==0 || loop>=256) return;

	memset(szTmp, 0x00, sizeof(szTmp));

	memcpy(szTmp, In, loop);
	memmove(In, In+loop, len-loop);
	memcpy(In+len-loop, szTmp, loop);
}

void Xor(char *InA, const char *InB, int len)
{
	int i;
	for (i=0; i<len; ++i) InA[i] ^= InB[i];
}

void Transform(char *Out, const char *In, const char *Table, int len)
{
	char szTmp[256] = {0};
	int i;

	if (!Out || !In || !Table) return;
	if (len >= 256) return;

	memset(szTmp, 0x00, sizeof(szTmp));
	for (i=0; i<len; ++i) szTmp[i] = In[Table[i]-1];

	memcpy(Out, szTmp, len);
}

void S_func(char Out[], const char In[])
{
	int i,j,k,l;
	for (i=0,j=0,k=0; i<8; ++i,In+=6,Out+=4)
	{
		j = (In[0]<<1) + In[5];
		k = (In[1]<<3) + (In[2]<<2) + (In[3]<<1) + In[4];

		for ( l=0; l<4; ++l)
			Out[l] = (S_Box[i][j][k]>>(3 - l)) & 1;
	}
}

void F_func(char In[], const char Ki[])
{
	char MR[48] = {0};
	memset(MR, 0x00, sizeof(MR));

	Transform(MR, In, E_Table, 48);
	Xor(MR, Ki, 48);
	S_func(In, MR);
	Transform(In, In, P_Table, 32);
}


char* Base64Encode(char *src, int srclen)
{
	int n,buflen,i,j;
	int pading=0;
	char *buf;
	static char *dst;

	buf=src;
	buflen=n=srclen;
	if (n%3!=0) /* pad with 0x00 by using a temp buffer */
	{
		pading=1;
		buflen=n+3-n%3;
		buf=(char*)malloc(buflen+1);
		memset(buf,0,buflen+1);
		memcpy(buf,src,n);
		for (i=0; i<3-n%3; i++)
			buf[n+i]=0x00;
	}

	dst=(char*)malloc(buflen*4/3+1);
	memset(dst,0,buflen*4/3+1);
	for (i=0,j=0; i<buflen; i+=3,j+=4)
	{
		dst[j]=(buf[i]&0xFC)>>2;
		dst[j+1]=((buf[i]&0x03)<<4) + ((buf[i+1]&0xF0)>>4);
		dst[j+2]=((buf[i+1]&0x0F)<<2) + ((buf[i+2]&0xC0)>>6);
		dst[j+3]=buf[i+2]&0x3F;
	}

	for (i=0; i<buflen*4/3; i++) /* map 6 bit value to base64 ASCII character */
	{
		dst[i]=ch64[dst[i]];
	}
	for (i=0; i<3-n%3; i++)/*补'='*/
		dst[j-i-1]='=';

	if (pading)
		free(buf);
	return dst;
}

void DES(char Out[], const char In[], const PSubKey pSubKey, int Type)
{
	int i;
	char M[64] = {0}, *ML=&M[0], *MR=&M[32], szTmp[32] = {0};

	ByteToBit(M, In, 64);
	Transform(M, M, IP_Table, 64);

	if (Type == ENCRYPT)
	{
		for (i=0; i<16; ++i)
		{
			memcpy(szTmp, MR, 32);
			F_func(MR, (*pSubKey)[i]);
			Xor(MR, ML, 32);
			memcpy(ML, szTmp, 32);
		}
	}
	else
	{
		for (i=15; i>=0; --i)
		{
			memcpy(szTmp, MR, 32);
			F_func(MR, (*pSubKey)[i]);
			Xor(MR, ML, 32);
			memcpy(ML, szTmp, 32);
		}
	}
	RotateL(M, 64, 32);
	Transform(M, M, IPR_Table, 64);
	BitToByte(Out, M, 64);
}
/*******************************************************************
函 数 名 称:	XP1Des
功 能 描 述：	执行单DES算法对文本加解密
参 数 说 明：	bType	:类型：加密ENCRYPT，解密DECRYPT
				bMode	:模式：ECB,CBC
				In		:待加密串指针
				in_len	:待加密串的长度，同时Out的缓冲区大小应大于或者等于in_len
				Key		:密钥(可为8位,16位,24位)支持3密钥
				key_len	:密钥长度，多出24位部分将被自动裁减
				Out		:待输出串指针
				out_len :输出缓存大小
				cvecstr :8字节随即字符串
				padMode:设置pad模式
作       者:	rocross---在huangjf代码上改编的。本来主要是用来解密C#des加密的一个程序，然后这里没有修改类名
				rocross@yeah.net
更 新 日 期：	2010.6.18

返回值 说明：	int     :是否加密成功，1：成功，0：失败
*******************************************************************/
int XP1Des(int bType, int bMode, const char *In, unsigned int in_len, const char *Key, unsigned int key_len, char *Out, unsigned int out_len, const char cvecstr[],int padMode)
{
	int i,j,k;
	char m_SubKey[16][48] = {0};
	char xpkey[8];

	memset(xpkey,0x0,8);
	CovertKey((char*)Key,xpkey);
	//strncpy(xpkey,Key,key_len>8?8:key_len);
	/*参数不合法*/
	if (!In || !Key || !Out) return 0;

	/*密钥长度，单DES只支持8字节（64位的密钥，每个第8位作为奇偶校验位），多于8字节的自动裁剪*/
	//if (key_len & 0x00000007) return 0;
	//if(in_len>MAXBLOCK) return 0;//如果输入的大于了最大块长度
	/*被加密数据长度，必需为8字节的倍数，如果非8的倍数，调用XPPad()函数补位*/
	char* instr;
	int o_len=((in_len+7)/8)*8;
	instr = (char*)malloc(o_len);
	memset(instr,0x0,o_len);

	if (in_len & 0x00000007)
	{
		XPPad(padMode,In,in_len,instr,&o_len);
	}
	else
	{
		strcpy(instr,In);//拷贝
	}
	/*输出缓存大小判断*/
	if (out_len < o_len) return 0;

	/*生成16个子密钥*/
	memset(m_SubKey, 0x00, sizeof(m_SubKey));
	SetSubKey(&m_SubKey, xpkey);

	if (bMode == ECB)
	{
		/*每8字节加密*/
		for (i=0,j=o_len>>3; i<j; ++i,Out+=8,instr+=8)
		{
			DES(Out, instr, &m_SubKey, bType);
		}
	}
	else if (bMode == CBC)
	{
		if (cvecstr == NULL) return 0;

		char cvec[8] = {0};
		char cvin[8] = {0};

		memcpy(cvec, cvecstr, 8);

		for (i=0,j=o_len>>3; i<j; ++i,Out+=8,instr+=8)
		{
			if (bType == ENCRYPT)
			{
				for ( k=0; k<8; ++k)
				{
					cvin[k] = instr[k] ^ cvec[k];
				}
			}
			else
			{
				memcpy(cvin, instr, 8);
			}

			DES(Out, cvin, &m_SubKey, bType);

			if (bType == ENCRYPT)
			{
				memcpy(cvec, Out, 8);
			}
			else
			{
				for (k=0; k<8; ++k)
				{
					Out[k] = Out[k] ^ cvec[k];
				}
				memcpy(cvec, cvin, 8);
			}
		}
	}
	else
	{
		return 0;
	}

	return 1;
}
/*******************************************************************
  函 数 名 称:	XP3Des
  功 能 描 述：	执行3DES算法对文本加解密
  参 数 说 明：	bType	:类型：加密ENCRYPT，解密DECRYPT
				bMode	:模式：ECB,CBC
				In		:待加密串指针
				in_len	:待加密串的长度，同时Out的缓冲区大小应大于或者等于in_len
				Key		:密钥(可为8位,16位,24位)支持3密钥
				key_len	:密钥长度，多出24位部分将被自动裁减
				Out		:待输出串指针
				out_len :输出缓存大小
				cvecstr :8字节随即字符串
				padMode:设置pad模式
  返回值 说明：	int     :是否加密成功，1：成功，0：失败
  作       者:	rocross---在huangjf代码上改编的。本来主要是用来解密C#des加密的一个程序，然后这里没有修改类名
				rocross@yeah.net
  更 新 日 期：	2010.6.18

  3DES(加密) = DES(key1, 加密) DES(key2, 解密) DES(key3, 加密)
  3DES(解密) = DES(key3, 解密) DES(key2, 加密) DES(key1, 解密)
  每个KEY为64位，总共可以有192位的KEY, 但一般都只使用128位的key
  如果只用128位密钥，则key3 = key1

*******************************************************************/
int XP3Des(int bType, int bMode, const char *In, unsigned int in_len, const char *Key, unsigned int key_len, char *Out, unsigned int out_len, const char cvecstr[],int padMode)
{
	int i,j,k;
	char m_SubKey[3][16][48] = {0};
	unsigned char nKey;


	/*参数不合法*/
	if (!In || !Key || !Out) return 0;
	//if(in_len>MAXBLOCK) return 0;//如果输入的大于了最大块长度
	/*被加密数据长度，必需为8字节的倍数，如果非8的倍数，调用XPPad()函数补位*/
	int o_len=((in_len+7)/8)*8;

	char* instr = (char*)malloc(o_len);
	memset(instr,0x0,o_len);
	/*被加密数据长度，必需为8字节的倍数，如果非8的倍数，调用RunPad()函数补位*/
	if (in_len & 0x00000007)
	{
		XPPad(padMode,In,in_len,instr,&o_len);
	}
	else
	{
		strcpy(instr,In);//拷贝
	}
	if(key_len>24)
	{
		key_len = 24;
	}
	int xpkey_len = ((key_len+7)/8)*8;//key的长度
	char* xpkey = (char*)malloc(xpkey_len);
	memset(xpkey,0x0,xpkey_len);//置零
	/*密钥长度，3DES只支持8、16、24字节（192位的密钥，实际使用128位，每个第8位作为奇偶校验位），多于24字节的自动裁剪*/

		strncpy(xpkey,Key,key_len);
		xpkey[key_len] = '\0';

	/*输出缓存大小判断*/
	if (out_len < o_len) return 0;

	/*生成16个子密钥*/
	nKey = (xpkey_len>>3)>3 ? 3 : (xpkey_len>>3);
	memset(m_SubKey, 0x00, sizeof(m_SubKey));
	for ( i=0; i<nKey; i++)
	{
		SetSubKey(&m_SubKey[i], &xpkey[i<<3]);
	}

	if (bMode == ECB)
	{
		if (nKey ==	1)
		{
			/*每8字节加密*/
			for (i=0,j=o_len>>3; i<j; ++i,Out+=8,instr+=8)
			{
				DES(Out, instr, &m_SubKey[0], bType);
			}
		}
		else if (nKey == 2)
		{
			for (i=0,j=o_len>>3; i<j; ++i,Out+=8,instr+=8)
			{
				DES(Out, instr,  &m_SubKey[0], bType);
				DES(Out, Out, &m_SubKey[1], bType==ENCRYPT?DECRYPT:ENCRYPT);
				DES(Out, Out, &m_SubKey[0], bType);
			}
		}
		else if (nKey == 3)
		{
			for (i=0,j=o_len>>3; i<j; ++i,Out+=8,instr+=8)
			{
				DES(Out, instr,  &m_SubKey[bType?2:0], bType);
				DES(Out, Out, &m_SubKey[1],         bType==ENCRYPT?DECRYPT:ENCRYPT);
				DES(Out, Out, &m_SubKey[bType?0:2], bType);
			}
		}
		else
		{
			/*密钥长度不对*/
			return 0;
		}
	}
	else if (bMode == CBC)
	{
		if (cvecstr == NULL) return 0;

		char cvec[8] = {0};
		char cvin[8] = {0};

		memcpy(cvec, cvecstr, 8);

		if (nKey == 1)
		{
			for (i=0,j=o_len>>3; i<j; ++i,Out+=8,instr+=8)
			{
				if (bType == ENCRYPT)
				{
					for (k=0; k<8; ++k)
					{
						cvin[k]	= instr[k] ^ cvec[k];
					}
				}
				else
				{
					memcpy(cvin, instr, 8);
				}

				DES(Out, cvin, &m_SubKey[0], bType);

				if (bType == ENCRYPT)
				{
					memcpy(cvec, Out, 8);
				}
				else
				{
					for (k=0; k<8; ++k)
					{
						Out[k] = Out[k] ^ cvec[k];
					}
					memcpy(cvec, cvin, 8);
				}
			}
		}
		else if (nKey == 2)
		{
			for (i=0,j=o_len>>3; i<j; ++i,Out+=8,instr+=8)
			{
				if (bType == ENCRYPT)
				{
					for ( k=0; k<8; ++k)
					{
						cvin[k] = instr[k] ^ cvec[k];
					}
				}
				else
				{
					memcpy(cvin, instr, 8);
				}

				DES(Out, cvin, &m_SubKey[0], bType);
				DES(Out, Out,  &m_SubKey[1], bType==ENCRYPT?DECRYPT:ENCRYPT);
				DES(Out, Out,  &m_SubKey[0], bType);

				if (bType == ENCRYPT)
				{
					memcpy(cvec, Out, 8);
				}
				else
				{
					for (k=0; k<8; ++k)
					{
						Out[k] = Out[k] ^ cvec[k];
					}
					memcpy(cvec, cvin, 8);
				}
			}

		}
		else if (nKey == 3)
		{

			for (i=0,j=o_len>>3; i<j; ++i,Out+=8,instr+=8)
			{
				if (bType == ENCRYPT)
				{
					for (k=0; k<8; ++k)
					{
						cvin[k]	= instr[k] ^ cvec[k];
					}
				}
				else
				{
					memcpy(cvin, instr, 8);
				}

				DES(Out, cvin, &m_SubKey[bType?2:0], bType);
				DES(Out, Out,  &m_SubKey[1],         bType==ENCRYPT?DECRYPT:ENCRYPT);
				DES(Out, Out,  &m_SubKey[bType?0:2], bType);

				if (bType == ENCRYPT)
				{
					memcpy(cvec, Out, 8);
				}
				else
				{
					for (k=0; k<8; ++k)
					{
						Out[k] = Out[k] ^ cvec[k];
					}
					memcpy(cvec, cvin, 8);
				}
			}

		}
		else
		{
			/*密钥长度不对*/
			return 0;
		}
		//XPRsm(Out);//去掉pad
	}
	else
	{
		return 0;
	}
	if(PAD_PKCS_7==padMode)
	{
		XPRsm(Out);
	}
	return 1;
}

int XPPad(int nType, const char *In, unsigned int in_len, char *Out, int *padlen)
{
	int res = (in_len & 0x00000007);

	/*
		if(*padlen< ((int)in_len+8-res))
		{
			return 0;
		}
		else
		{
			*padlen	=	((int)in_len+8-res);
			memcpy(Out,In,in_len);
		}
	*/
	*padlen	=	((int)in_len+8-res);
	memcpy(Out,In,in_len);

	if (nType	==	PAD_ISO_1)
	{
		memset(Out+in_len,0x00,8-res);
	}
	else if (nType	==	PAD_ISO_2)
	{
		memset(Out+in_len,0x80,1);
		memset(Out+in_len,0x00,7-res);
	}
	else if (nType	==	PAD_PKCS_7)
	{
		memset(Out+in_len,8-res,8-res);
		//memset(Out+in_len,0x00,8-res);
	}
	else
	{
		return 0;
	}

	return 1;
}

void XPRsm(char *Text)
{
	int len,tmpint;

	len=strlen(Text);
	tmpint=*(Text+len-1);
	*(Text+len-tmpint)=0x00;
}

int CovertKey(char *iKey, char *oKey)
{
	char in[64],out[64];
	int	 inlen,i,j;
	unsigned char p,q,t,m,n;

	memset(in,0,sizeof(in));
	memset(out,0,sizeof(out));

	inlen=strlen(iKey);
	/*检查长度合法性*/
	if (inlen!=48)
		return 0;
	strcpy(in,iKey);
	/*检查字符合法性*/
	for (i=0; i<inlen; i++)
	{
		if (!isxdigit(in[i]))
			return 0;
	}
	for (i=0,j=0; i<inlen; i+=2,j++)
	{
		p=toupper(in[i]);
		q=toupper(in[i+1]);

		if (isdigit(p))
			m=p-48;
		else
			m=p-55;

		if (isdigit(q))
			n=q-48;
		else
			n=q-55;

		p=(char)((m<<4)&0xf0);
		q=n&0x0f;
		t=p|q;
		out[j]=t;
	}
	memcpy(oKey,out,j+1);

	return 1;
}

char *Base64FromHex(char *pcBase64,int iLen)
{
	int i=0;
	int iBase64Len = iLen/2;
	char *vplainText = (char*)malloc(iBase64Len+1);
	memset(vplainText,0,iBase64Len+1);
	for(i=0;i<2*iBase64Len;i++)
	{
		char c1 = pcBase64[i] - 48  < 17  ? pcBase64[i] - 48 : pcBase64[i] - 55;
		i++;
		char c2 = pcBase64[i] - 48  < 17  ? pcBase64[i] - 48 : pcBase64[i] - 55;
		vplainText[i/2]= c1*16 + c2;
	}
	vplainText[iBase64Len] = 0x0;

	return vplainText;
}

