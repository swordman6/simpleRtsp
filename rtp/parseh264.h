#ifndef __PARSEH264_H__
#define __PARSEH264_H_

/*****************************************************************************************************
 * H.264/AVC 视频编码标准: 
 * NAL(network abstraction layer) = VCL(video coding layer) + header
 * 
 * H264 码流数据由 NALU 序列组成，相邻的 NALU 由起始码 StartCode 隔开
 * 起始码 StartCode 的两种形式：3 字节的 0x000001 和 4 字节的 0x00000001。
 * 
 * NALU Header 由 3 部分组成，共占用 1 个字节
 * forbidden_bit(1bit)      禁止位,默认为0，为1时被丢弃
 * nal_reference_bit(2bit)  重要性指示位，0-3，3最高
 * nal_unit_type(5bit)      类型位，0-12为H264定义,12-23保留,24-31未使用
 * 
 * nal_unit_type: 
 * 0  未使用
 * 1  不分区，非IDR的slice
 * 2  使用Data Partitioning,且为Slice A
 * 3  使用Data Partitioning,且为Slice B
 * 4  使用Data Partitioning,且为Slice C
 * 5  IDR图像中的slice
 * 6  补充增强信息单元(SEI)
 * 7  序列参数集(Sequence Parameter Set, SPS)
 * 8  图像参数集(Picture  Parametet Set, PPS)
 * 9  分界符
 * 10 序列结束
 * 11 码流结束
 * 12 填充
 * 
 * NALU 的主体部分: NALU = NALU Header + EBSP
 * SODB: String Of Data Bits 原始数据比特流，编码后的原始数据，即VCL数据。
 * RBSP: 原始字节序列载荷。由于 SODB 长度不一定是 8 的倍数，添加8字节对齐
 * EBSP: 扩展字节序列载荷。RBSP 数据中每遇到两个字节连续为 0，就插入一个字节的 0x03，这样就得到了 EBSP。
 *
 ********************************************************************************************/
 
typedef enum {
	NALU_TYPE_SLICE     = 1,
	NALU_TYPE_DPA       = 2,
	NALU_TYPE_DPB       = 3,
	NALU_TYPE_DPC       = 4,
	NALU_TYPE_IDR       = 5,
	NALU_TYPE_SEI       = 6,
	NALU_TYPE_SPS       = 7,
	NALU_TYPE_PPS       = 8,
	NALU_TYPE_AUD       = 9,
	NALU_TYPE_EOFSEQ    = 10,
	NALU_TYPE_EOFSTREAM = 11,
	NALU_TYPE_FILL      = 12,
} NaluType;
 
typedef enum {
	NALU_PRIORITY_DISPOSABLE = 0, //B帧
	NALU_PRIORITY_LOW        = 1, //P帧
	NALU_PRIORITY_HIGH       = 2, //I帧
	NALU_PRIORITY_HIGHEST    = 3  //IDR帧,SPS,PPS
} NaluPriority;

typedef struct {
	long len;                      //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
	long max_size;                 //! Nal Unit Buffer size
	char forbidden_bit;            //! should be always FALSE
	char nal_reference_idc;        //! NALU_PRIORITY_xxxx
	char nal_unit_type;            //! NALU_TYPE_xxxx    
	char *buf;                     //! contains the first byte followed by the EBSP
} NALU_t;

int GetAnnexbNALU(FILE *fp, NALU_t *nalu_st);

#endif
