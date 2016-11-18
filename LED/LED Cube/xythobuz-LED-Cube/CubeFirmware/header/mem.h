/*
 * mem.h
 *
 * Copyright 2012 Thomas Buck <xythobuz@me.com>
 *
 * This file is part of LED-Cube.
 *
 * LED-Cube is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LED-Cube is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LED-Cube.  If not, see <http://www.gnu.org/licenses/>.
 */

#define MEMTWIADDRESS 0xA0
#define MemLength 131072

// address is a number between (inclusive) zero and 131071
uint8_t memGetByte(uint32_t address);

// Free returned memory!
uint8_t *memGetBytes(uint32_t address, uint8_t length);

void memWriteByte(uint32_t address, uint8_t data);

void memWriteBytes(uint32_t address, uint8_t *data, uint8_t length);