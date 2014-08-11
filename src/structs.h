#ifndef _STRUCTS_H_
#define _STRUCTS_H_

struct header_stream_packets 
{
	unsigned long 	payload_size;
	unsigned short 	payload_type;
	short 			something;
	double 			timestamp;
} __attribute__ ((__packed__));

struct codec_data
{
	unsigned char 	version;
	unsigned char 	profile;
	unsigned char 	compatibility;
	unsigned char 	level;
	unsigned int 	reserved_1 :6;
	unsigned int 	nal :2;
	unsigned int 	reserved_2 :3;
	unsigned int 	number_of_sps :5;
	unsigned short 	length_of_sps;
	unsigned char 	sequence_parameter_set[16];
	unsigned char 	number_of_pps;
	unsigned short 	length_of_pps;
	unsigned long 	picture_parameter_set;
}  __attribute__ ((__packed__));

#endif // !_STRUCTS_H_
