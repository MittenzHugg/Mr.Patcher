#pragma once
#include <cstdint>

uint32_t n64_get_word(uint8_t* src) {
	return (((uint32_t)src[0]) << 0x18) | (((uint32_t)src[1]) << 0x10) | (((uint32_t)src[2]) << 0x08) | ((uint32_t)src[3]);
};

uint16_t n64_get_half(uint8_t* src) {
	return (((uint16_t)src[0]) << 0x8) | ((uint16_t)src[1]);
};

void n64_set_half(uint8_t* dst, uint16_t src) {
	*dst = (uint8_t)((src & 0xFF00) >> 8);
	*(dst + 1) = (uint8_t)(src & 0x00FF);
	return;
};