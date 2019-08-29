#pragma once
#include <cstdint>

typedef struct gs_byte gs_byte_t;
struct gs_byte {
	uint32_t addr;
	uint8_t val;
	gs_byte_t * next;
};
class gs_parser
{
public:
	gs_parser(char* filename);
	~gs_parser();

	int get_byte(uint32_t* ram_addr, uint8_t* value);

private:
	gs_byte_t* byte_list;
};

