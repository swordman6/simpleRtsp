#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseh264.h"

static int FindStartCode2 (unsigned char *Buf){
	if(Buf[0]!=0 || Buf[1]!=0 || Buf[2] !=1) //0x000001?
        return 0; 
	else 
        return 1;
}
 
static int FindStartCode3 (unsigned char *Buf){
	if(Buf[0]!=0 || Buf[1]!=0 || Buf[2] !=0 || Buf[3] !=1) //0x00000001?
        return 0;
	else 
        return 1;
}

long GetH264Header(FILE *fp, int *codeprefix_len, NALU_t *nalu_st)
{
    unsigned char header[4];
    long offset;

    if(3 != fread(header, 1, 3, fp))
        return -1;

    while(!feof(fp))
    {
        if(!FindStartCode2(header))
        {
            if(1 != fread(header+3, 1, 1, fp))
                return -1;

            if(!FindStartCode3(header))
            {              
                header[0] = header[1];
                header[1] = header[2];
                header[2] = header[3];
            }
            else
            {
                *codeprefix_len = 4;
                break;
            }
        }
        else
        {
            *codeprefix_len = 3;
            break;
        }

    }

    offset = ftell(fp);

    return offset;
}

int GetAnnexbNALU(FILE *fp, NALU_t *nalu_st)
{
    if(fp == NULL || nalu_st == NULL)
        return -1;

    long startoffset,endoffset,data_offset;
    int  startcodeprefix_len,endcodeprefix_len;

    startoffset = GetH264Header(fp, &startcodeprefix_len, nalu_st);
    if(startoffset < 0)
        return -1;

    endoffset   = GetH264Header(fp, &endcodeprefix_len, nalu_st);
    if(endoffset < 0)
        return -1;

    data_offset = endoffset - endcodeprefix_len;
    nalu_st->len = data_offset - startoffset - 1;

    fseek(fp, startoffset, SEEK_SET);

    if(1 != fread(nalu_st->buf, 1, 1, fp))
        return -1;

    nalu_st->forbidden_bit = nalu_st->buf[0] & 0x80; //1 bit
	nalu_st->nal_reference_idc = nalu_st->buf[0] & 0x60; // 2 bit
	nalu_st->nal_unit_type = (nalu_st->buf[0]) & 0x1f;// 5 bit

    if(nalu_st->len != fread(nalu_st->buf, 1, nalu_st->len, fp))
        return -1;

    printf("nalu_st->len = %ld\n", nalu_st->len);

    return data_offset;
}


