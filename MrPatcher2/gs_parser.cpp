#include "gs_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gs_parser::gs_parser(char* filename) {
	byte_list = nullptr;
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		return;
	}
	char line[256];
	char* codeStart;
	char tmp[8];
	uint32_t gs_addr;
	gs_byte_t* tmp_ptr;
	while (fgets(line, sizeof(line), fp)) {
		codeStart = line;
		//remove line start
		while ((*codeStart == ' ') || (*codeStart == '-')) {
			codeStart += 1;
		}

		//get code address
		if ((*codeStart == '8') || (*codeStart == 'F')) {
			codeStart++;
			if ((*codeStart == '0')) {
				codeStart++;
				strncpy(tmp, codeStart, 6);
				tmp_ptr = (gs_byte_t*)malloc(sizeof(gs_byte_t));
				tmp_ptr->addr = strtol(tmp, NULL, 16);
				codeStart += 7;
				strncpy(tmp, codeStart + 2, 2);
				tmp[2] = '\0';
				tmp_ptr->val = strtol(tmp, NULL, 16);
				tmp_ptr->next = byte_list;
				byte_list = tmp_ptr;
			}
			else if ((*codeStart == '1')) {
				codeStart++;
				strncpy(tmp, codeStart, 6);
				gs_addr = strtol(tmp, NULL, 16);
				tmp_ptr = (gs_byte_t*)malloc(sizeof(gs_byte_t));
				tmp_ptr->addr = gs_addr;
				codeStart += 7;
				strncpy(tmp, codeStart, 2);
				tmp[2] = '\0';
				tmp_ptr->val = strtol(tmp, NULL, 16);
				tmp_ptr->next = byte_list;
				byte_list = tmp_ptr;

				tmp_ptr = (gs_byte_t*)malloc(sizeof(gs_byte_t));
				tmp_ptr->addr = gs_addr+1;
				strncpy(tmp, codeStart + 2, 2);
				tmp[2] = '\0';
				tmp_ptr->val = strtol(tmp, NULL, 16);
				tmp_ptr->next = byte_list;
				byte_list = tmp_ptr;
			}
		}

	}

	
}

gs_parser::~gs_parser() {
	while (byte_list != nullptr) {
		gs_byte_t* tmp_ptr = byte_list;
		byte_list = byte_list->next;
		free(tmp_ptr);
		tmp_ptr = nullptr;
	}
}

int gs_parser::get_byte(uint32_t* ram_addr, uint8_t* value) {
	if (byte_list != nullptr) {
		*(ram_addr) = byte_list->addr;
		*(value) = byte_list->val;
		gs_byte_t* tmp_ptr = byte_list;
		byte_list = byte_list->next;
		free(tmp_ptr);
		tmp_ptr = nullptr;
		return 0;
	}
	else {
		return -1;
	}
	
}