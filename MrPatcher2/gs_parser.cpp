/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   gs_parser.cpp
*
*   This file is part of Mr.Patcher.
*
*   Mr.Patcher is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   Mr.Patcher is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with Mr.Patcher.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/
#include "gs_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <regex>
#include <execution>

gs_parser::gs_parser(char* filename) {
	std::ifstream in(filename);
	std::string tmp_line;
	std::vector<gs_c>codes;

	//parse lines
	std::regex gs_code_regex("[8fF]([01])([A-Fa-f0-9]{6}) ([A-Fa-f0-9]{4})");
	while (std::getline(in, tmp_line)) {
		std::smatch code_matches;
		if (std::regex_search(tmp_line, code_matches, gs_code_regex)) {
			codes.push_back(
				{ (bool) std::stoi(code_matches[1])
				, (u32) std::stoul(code_matches[2],nullptr,16)
				, (u16) std::stoi(code_matches[3],nullptr,16) 
				});
		}
	}

	//sort by address
	std::sort(std::execution::par_unseq,
		codes.begin(), codes.end(), 
		[](gs_c lhs, gs_c rhs) {
			return lhs.addr < rhs.addr; 
		}
	);


	//everything after this is old code and needs to be deleted/replaced with ^
	byte_list = nullptr;
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		throw "Unable to open "+ std::string(filename);
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