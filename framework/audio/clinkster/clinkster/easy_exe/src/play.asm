
%include "src/clinkster.inc"

global _main

extern __imp__printf
extern __imp__GetAsyncKeyState@4
extern __imp__ExitProcess@4
extern __imp__Sleep@4

	section main text align=1

_main:
	push	message
	push	messageformat
	call	[__imp__printf]
	add		esp, byte 2*4

	call	Clinkster_GenerateMusic
	call	Clinkster_StartMusic

.playloop:
	mov		ebx, 60

	fild	dword [Clinkster_MusicLength]
	fdiv	dword [Clinkster_TicksPerSecond]
	push	eax
	fistp	dword [esp]
	pop		eax ; music length in seconds

	xor		edx, edx
	div		ebx
	push	edx
	push	eax

	call	Clinkster_GetPosition
	fdiv	dword [Clinkster_TicksPerSecond]
	push	eax
	fistp	dword [esp]
	pop		eax ; play position in seconds

	xor		edx, edx
	div		ebx
	push	edx
	push	eax

	push	timeformat
	call	[__imp__printf]
	add		esp, byte 5*4

	push	byte 100
	call	[__imp__Sleep@4]

	push	byte 27
	call	[__imp__GetAsyncKeyState@4]
	test	ax, ax
	je		.playloop

	push	byte 0
	call	[__imp__ExitProcess@4]

	section mformat rdata align=1

messageformat:
	db "%s",0

	section tformat rdata align=1

timeformat:
	db 13,"Playing %d:%02d / %d:%02d",0

	section message rdata align=1

message:
	incbin "music.txt"
	db 0
