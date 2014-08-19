#ifndef _STRUCTS_H_
#define _STRUCTS_H_

struct header_stream_packets 
{
	unsigned int 	payload_size;
	unsigned short 	payload_type;
	short 			something;
	double 			timestamp;
} __attribute__ ((__packed__));

struct codec_data
{
	unsigned int 	version: 8;
	unsigned int 	profile: 8;
	unsigned int 	compatibility: 8;
	unsigned int 	level: 8;
	unsigned short 	reserved_1 :6;
	unsigned short 	nal :2;
	unsigned short 	reserved_2 :3;
	unsigned short 	number_of_sps :5;
	unsigned short 	length_of_sps;
	unsigned int 	sequence_parameter_set[4];
	unsigned char 	number_of_pps;
	unsigned short 	length_of_pps;
	unsigned int 	picture_parameter_set;
}  __attribute__ ((__packed__));

#endif // !_STRUCTS_H_
