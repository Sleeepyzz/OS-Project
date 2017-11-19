/*
paging.s Assembly functions for paging

Copyright (C) 2017 Ryken Thompson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

.section .text
	.global read_cr2
	.type read_cr2, @function
	read_cr2:
		mov %cr2, %eax
		ret

	.global read_cr3
	.type read_cr3, @function
	read_cr3:
		mov %cr3, %eax
		ret
	
	.global write_cr3
	.type write_cr3, @function
	write_cr3:
		push %ebp
		mov %esp, %ebp
		mov 8(%esp), %eax
		mov %eax, %cr3
		mov %ebp, %esp
		pop %ebp
		ret
	
	.global readcr4
	.type readcr4, @function
	readcr4:
		mov %cr4, %eax
		ret
