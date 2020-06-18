; Clinkster music converted from Maytz-Michigan.xrns 2013-09-17 10:33:00

%define USES_SINE 1
%define USES_SAWTOOTH 1
%define USES_SQUARE 0
%define USES_PARABOLA 1
%define USES_TRIANGLE 0
%define USES_NOISE 0
%define USES_VELOCITY 0
%define USES_LONG_NOTES 0
%define USES_DELAY 1
%define USES_PANNING 0
%define USES_INDEXDECAY 0
%define USES_GAIN 1

%define SUBSAMPLES_PER_TICK 10584
%define MAX_INSTRUMENT_SUBSAMPLES 2162688
%define MAX_TOTAL_INSTRUMENT_SAMPLES 1703936
%define MAX_RELEASE_SUBSAMPLES 720896
%define TOTAL_SAMPLES 9371648
%define MAX_TRACK_INSTRUMENT_RENDERS 9

%define MAX_DELAY_LENGTH 15346
%define LEFT_DELAY_LENGTH 15346
%define RIGHT_DELAY_LENGTH 15346
%define DELAY_STRENGTH 0.10000000

%define NUMTRACKS 10
%define LOGNUMTICKS 12
%define MUSIC_LENGTH 3456
%define TICKS_PER_SECOND 16.66666667


	section instdata data align=1

_InstrumentData:
	; 00:  Clap / 03|Clap
	db	1,0,21,14,0,10,20,0,0,5,-4,42,50,-39,72,-59,3,19
	db	33,1,0,-1
	; 01:  Clap / 03|Clap
	db	1,0,21,14,0,10,20,0,0,5,-4,42,50,-39,72,-59,3,19
	db	21,1,0,11,1,0,11,1,0,-1
	; 02:  bell / 06|snare
	db	0,2,26,0,0,3,20,0,0,4,-2,0,-46,0,45,-59,-9,-27
	db	93,1,0,11,1,0,-1
	; 03:  hihat / 08|hihat
	db	0,0,26,22,0,26,20,0,0,2,72,34,51,-66,45,-59,-11,-27
	db	40,1,0,11,1,0,11,1,0,-1
	db	-1
	; 04:  string high / 09|stringsHigh
	db	0,2,0,8,8,23,11,0,6,7,-120,0,0,0,0,-3,25,29
	db	64,8,0,0,8,0,3,8,0,2,8,0,1,8,0,1,32,0,0,32,0,3,8,0,2,8,0,-1
	; 05:  string melo / 0E|stringsHighshort
	db	0,2,0,8,8,24,20,0,-5,7,-110,0,0,0,0,-26,10,18
	db	77,4,0,1,4,0,1,4,0,1,4,0,0,4,0,1,4,0,-1
	; 06:  string bass / 0C|stringsLow
	db	0,2,26,2,11,8,17,0,9,13,-64,0,0,0,45,-3,25,26
	db	33,96,32,0,4,32,0,1,32,0,0,32,0,-1
	; 07:  stringLead / 0D|stringsHigh
	db	0,2,0,8,8,23,11,0,6,13,-120,0,0,0,0,-3,25,29
	db	45,96,32,0,4,32,0,1,32,0,0,32,0,-1
	; 08:  Bassdrum / 00|bassdrum
	db	2,0,0,0,0,0,1,0,-14,18,26,0,-70,0,15,-67,-3,-3
	db	33,20,12,10,8,7,4,0,11,4,0,6,4,0,-1
	; 09:  bassDrum / 00|bassdrum
	db	2,0,0,0,0,0,1,0,-14,18,26,0,-70,0,15,-67,-3,-3
	db	33,20,10,8,6,4,0,11,4,0,11,8,6,4,0,-1
	db	-1,-1

	section notepos data align=1

_NotePositions:
	; 00:  Clap / 03|Clap
	; position 3 - pattern 0
	db	-2,144,32,32,32
	; position 4 - pattern 0
	db	32,32,32,32
	; position 5 - pattern 0
	db	32,32,32,32
	; position 6 - pattern 3
	db	32,32,32
	; position 7 - pattern 0
	db	64,32,32,32
	; position 8 - pattern 0
	db	32,32,32,32
	; position 9 - pattern 0
	db	32,32,32,32
	; position 10 - pattern 3
	db	32,32,32
	; position 13 - pattern 1
	db	-2,64,32,32,32
	; position 14 - pattern 5
	db	32,32,32,32
	; position 15 - pattern 0
	db	32,32,32,32
	; position 16 - pattern 0
	db	32,32,32,32
	; position 17 - pattern 0
	db	32,32,32,32
	; position 18 - pattern 3
	db	32,32,32
	; position 19 - pattern 0
	db	64,32,32,32
	; position 20 - pattern 0
	db	32,32,32,32
	; position 21 - pattern 0
	db	32,32,32,32
	; position 22 - pattern 3
	db	32,32,32

	; 01:  Clap / 03|Clap
	; position 3 - pattern 0
	db	-2,144,6,6,20,32,6,6,20
	; position 4 - pattern 0
	db	32,6,6,20,32,6,6,20
	; position 5 - pattern 0
	db	32,6,6,20,32,6,6,20
	; position 6 - pattern 3
	db	32,6,6,20,32,6,6
	; position 7 - pattern 0
	db	52,6,6,20,32,6,6,20
	; position 8 - pattern 0
	db	32,6,6,20,32,6,6,20
	; position 9 - pattern 0
	db	32,6,6,20,32,6,6,20
	; position 10 - pattern 3
	db	32,6,6,20,32,6,6
	; position 13 - pattern 1
	db	-2,52,6,6,20,32,6,6,20
	; position 14 - pattern 5
	db	32,6,6,20,32,6,6,20
	; position 15 - pattern 0
	db	32,6,6,20,32,6,6,20
	; position 16 - pattern 0
	db	32,6,6,20,32,6,6,20
	; position 17 - pattern 0
	db	32,6,6,20,32,6,6,20
	; position 18 - pattern 3
	db	32,6,6,20,32,6,6
	; position 19 - pattern 0
	db	52,6,6,20,32,6,6,20
	; position 20 - pattern 0
	db	32,6,6,20,32,6,6,20
	; position 21 - pattern 0
	db	32,6,6,20,32,6,6,20
	; position 22 - pattern 3
	db	32,6,6,20,32,6,6

	; 02:  bell / 06|snare
	; position 3 - pattern 0
	db	-2,128,32,36,28
	; position 4 - pattern 0
	db	32,32,36,28
	; position 5 - pattern 0
	db	32,32,36,28
	; position 6 - pattern 3
	db	32,32,36
	; position 7 - pattern 0
	db	60,32,36,28
	; position 8 - pattern 0
	db	32,32,36,28
	; position 9 - pattern 0
	db	32,32,36,28
	; position 10 - pattern 3
	db	32,32,36
	; position 14 - pattern 5
	db	-2,188,32,36,28
	; position 15 - pattern 0
	db	32,32,36,28
	; position 16 - pattern 0
	db	32,32,36,28
	; position 17 - pattern 0
	db	32,32,36,28
	; position 18 - pattern 3
	db	32,32,36
	; position 21 - pattern 0
	db	-2,60,32,36,28
	; position 22 - pattern 3
	db	32,32,36

	; 03:  hihat / 08|hihat
	; position 5 - pattern 0
	db	-3,128,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2
	; position 6 - pattern 3
	db	4,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2
	; position 7 - pattern 0
	db	32,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2
	; position 8 - pattern 0
	db	4,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2
	; position 9 - pattern 0
	db	4,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2
	; position 10 - pattern 3
	db	4,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2
	; position 15 - pattern 0
	db	-3,32,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2
	; position 16 - pattern 0
	db	4,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2
	; position 17 - pattern 0
	db	4,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2
	; position 18 - pattern 3
	db	4,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2
	; position 21 - pattern 0
	db	-2,32,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2
	; position 22 - pattern 3
	db	4,2,2,2,2,4,4,2,2,2,2,2,2,2,2,4,4,2,2,2,2,4,2,2,4,4,2,2,2,2,2,2,4,4,2,2,2,2,2,2

	; 04:  string high / 09|stringsHigh
	; position 0 - pattern 0
	db	0,8,8,8,8,32,8,8,8,8
	; position 1 - pattern 0
	db	32,8,8,8,8,32,8,8,8,8
	; position 7 - pattern 0
	db	-3,160,8,8,8,8,32,8,8,8,8
	; position 8 - pattern 0
	db	32,8,8,8,8,32,8,8,8,8
	; position 9 - pattern 0
	db	32,8,8,8,8,32,8,8,8,8
	; position 10 - pattern 3
	db	32,8,8,8,8,32,8,8,8,8
	; position 15 - pattern 0
	db	-3,32,8,8,8,8,32,8,8,8,8
	; position 16 - pattern 0
	db	32,8,8,8,8,32,8,8,8,8
	; position 17 - pattern 0
	db	32,8,8,8,8,32,8,8,8,8
	; position 18 - pattern 3
	db	32,8,8,8,8,32,8,8,8,8
	; position 21 - pattern 0
	db	-2,32,8,8,8,8,32,8,8,8,8
	; position 22 - pattern 3
	db	32,8,8,8,8,32,8,8,8,8
	; position 23 - pattern 4
	db	32,8,8,8,8,32,8,8,8,8
	; position 24 - pattern 4
	db	32,8,8,8,8,32,8,8,8,8

	; 05:  string melo / 0E|stringsHighshort
	; position 9 - pattern 0
	db	-5,164,4,4,4,4,4,4,40,4,4,4,4,4,4
	; position 10 - pattern 3
	db	40,4,4,4,4,4,4,40,4,4,4,4,4,4
	; position 15 - pattern 0
	db	-3,40,4,4,4,4,4,4,40,4,4,4,4,4,4
	; position 16 - pattern 0
	db	40,4,4,4,4,4,4,40,4,4,4,4,4,4
	; position 17 - pattern 0
	db	40,4,4,4,4,4,4,40,4,4,4,4,4,4
	; position 18 - pattern 3
	db	40,4,4,4,4,4,4,40,4,4,4,4,4,4
	; position 21 - pattern 0
	db	-2,40,4,4,4,4,4,4,40,4,4,4,4,4,4
	; position 22 - pattern 3
	db	40,4,4,4,4,4,4,40,4,4,4,4,4,4

	; 06:  string bass / 0C|stringsLow
	; position 0 - pattern 0
	db	0,32,32,32
	; position 1 - pattern 0
	db	32,32,32,32
	; position 2 - pattern 1
	db	32
	; position 3 - pattern 0
	db	-1,128,32,32,32
	; position 4 - pattern 0
	db	32,32,32,32
	; position 5 - pattern 0
	db	32,32,32,32
	; position 6 - pattern 3
	db	32,32,32,32
	; position 7 - pattern 0
	db	32,32,32,32
	; position 8 - pattern 0
	db	32,32,32,32
	; position 9 - pattern 0
	db	32,32,32,32
	; position 10 - pattern 3
	db	32,32,32,32
	; position 11 - pattern 4
	db	32
	; position 12 - pattern 1
	db	-1,128
	; position 13 - pattern 1
	db	-1,128
	; position 14 - pattern 5
	db	-1,128
	; position 15 - pattern 0
	db	-1,128,32,32,32
	; position 16 - pattern 0
	db	32,32,32,32
	; position 17 - pattern 0
	db	32,32,32,32
	; position 18 - pattern 3
	db	32,32,32,32
	; position 19 - pattern 0
	db	32,32,32,32
	; position 20 - pattern 0
	db	32,32,32,32
	; position 21 - pattern 0
	db	32,32,32,32
	; position 22 - pattern 3
	db	32,32,32,32
	; position 23 - pattern 4
	db	32
	; position 24 - pattern 4
	db	-1,128
	; position 25 - pattern 4
	db	-1,128
	; position 26 - pattern 1
	db	-1,128

	; 07:  stringLead / 0D|stringsHigh
	; position 0 - pattern 0
	db	0,32,32,32
	; position 1 - pattern 0
	db	32,32,32,32
	; position 2 - pattern 1
	db	32
	; position 3 - pattern 0
	db	-1,128,32,32,32
	; position 4 - pattern 0
	db	32,32,32,32
	; position 5 - pattern 0
	db	32,32,32,32
	; position 6 - pattern 3
	db	32,32,32,32
	; position 7 - pattern 0
	db	32,32,32,32
	; position 8 - pattern 0
	db	32,32,32,32
	; position 9 - pattern 0
	db	32,32,32,32
	; position 10 - pattern 3
	db	32,32,32,32
	; position 11 - pattern 4
	db	32
	; position 12 - pattern 1
	db	-1,128
	; position 13 - pattern 1
	db	-1,128
	; position 14 - pattern 5
	db	-1,128
	; position 15 - pattern 0
	db	-1,128,32,32,32
	; position 16 - pattern 0
	db	32,32,32,32
	; position 17 - pattern 0
	db	32,32,32,32
	; position 18 - pattern 3
	db	32,32,32,32
	; position 19 - pattern 0
	db	32,32,32,32
	; position 20 - pattern 0
	db	32,32,32,32
	; position 21 - pattern 0
	db	32,32,32,32
	; position 22 - pattern 3
	db	32,32,32,32
	; position 23 - pattern 4
	db	32
	; position 24 - pattern 4
	db	-1,128
	; position 25 - pattern 4
	db	-1,128
	; position 26 - pattern 1
	db	-1,128

	; 08:  Bassdrum / 00|bassdrum
	; position 3 - pattern 0
	db	-2,128,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 4 - pattern 0
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 5 - pattern 0
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 6 - pattern 3
	db	10,8,4,12,4,4,12,20,8,4,12,4,4
	; position 7 - pattern 0
	db	32,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 8 - pattern 0
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 9 - pattern 0
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 10 - pattern 3
	db	10,8,4,12,4,4,12,20,8,4,12,4,4
	; position 11 - pattern 4
	db	32,12,12,8,12,20,12,12,8,12
	; position 12 - pattern 1
	db	20,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 13 - pattern 1
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 14 - pattern 5
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12
	; position 15 - pattern 0
	db	20,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 16 - pattern 0
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 17 - pattern 0
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 18 - pattern 3
	db	10,8,4,12,4,4,12,20,8,4,12,4,4
	; position 19 - pattern 0
	db	32,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 20 - pattern 0
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 21 - pattern 0
	db	10,8,4,12,4,4,12,20,8,4,12,4,4,12,10
	; position 22 - pattern 3
	db	10,8,4,12,4,4,12,20,8,4,12,4,4
	; position 23 - pattern 4
	db	32,12,12,8,12,20,12,12,8,12
	; position 24 - pattern 4
	db	20,12,12,8,12,20,12,12,8,12
	; position 25 - pattern 4
	db	20,12,12,8,12,20,12,12,8,12

	; 09:  bassDrum / 00|bassdrum
	; position 3 - pattern 0
	db	-2,128,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 4 - pattern 0
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 5 - pattern 0
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 6 - pattern 3
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8
	; position 7 - pattern 0
	db	32,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 8 - pattern 0
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 9 - pattern 0
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 10 - pattern 3
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8
	; position 11 - pattern 4
	db	32,12,12,8,12,20,12,12,8,12
	; position 12 - pattern 1
	db	20,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 13 - pattern 1
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 14 - pattern 5
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6
	; position 15 - pattern 0
	db	20,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 16 - pattern 0
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 17 - pattern 0
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 18 - pattern 3
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8
	; position 19 - pattern 0
	db	32,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 20 - pattern 0
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 21 - pattern 0
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8,6,6,6,4
	; position 22 - pattern 3
	db	10,4,8,8,4,8,6,6,6,4,10,4,8,8,4,8
	; position 23 - pattern 4
	db	32,12,12,8,12,20,12,12,8,12
	; position 24 - pattern 4
	db	20,12,12,8,12,20,12,12,8,12
	; position 25 - pattern 4
	db	20,12,12,8,12,20,12,12,8,12


	section notesamp data align=1

_NoteSamples:
	; 00:  Clap / 03|Clap
	; position 3 - pattern 0
	db	0,0,0,0
	; position 4 - pattern 0
	db	0,0,0,0
	; position 5 - pattern 0
	db	0,0,0,0
	; position 6 - pattern 3
	db	0,0,0
	; position 7 - pattern 0
	db	0,0,0,0
	; position 8 - pattern 0
	db	0,0,0,0
	; position 9 - pattern 0
	db	0,0,0,0
	; position 10 - pattern 3
	db	0,0,0
	; position 13 - pattern 1
	db	0,0,0,0
	; position 14 - pattern 5
	db	0,0,0,0
	; position 15 - pattern 0
	db	0,0,0,0
	; position 16 - pattern 0
	db	0,0,0,0
	; position 17 - pattern 0
	db	0,0,0,0
	; position 18 - pattern 3
	db	0,0,0
	; position 19 - pattern 0
	db	0,0,0,0
	; position 20 - pattern 0
	db	0,0,0,0
	; position 21 - pattern 0
	db	0,0,0,0
	; position 22 - pattern 3
	db	0,0,0
	db	-1

	; 01:  Clap / 03|Clap
	; position 3 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 4 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 5 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 6 - pattern 3
	db	2,1,0,2,2,1,0
	; position 7 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 8 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 9 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 10 - pattern 3
	db	2,1,0,2,2,1,0
	; position 13 - pattern 1
	db	2,1,0,2,2,1,0,2
	; position 14 - pattern 5
	db	2,1,0,2,2,1,0,2
	; position 15 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 16 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 17 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 18 - pattern 3
	db	2,1,0,2,2,1,0
	; position 19 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 20 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 21 - pattern 0
	db	2,1,0,2,2,1,0,2
	; position 22 - pattern 3
	db	2,1,0,2,2,1,0
	db	-1

	; 02:  bell / 06|snare
	; position 3 - pattern 0
	db	1,0,0,1
	; position 4 - pattern 0
	db	1,0,0,1
	; position 5 - pattern 0
	db	1,0,0,1
	; position 6 - pattern 3
	db	1,0,0
	; position 7 - pattern 0
	db	1,0,0,1
	; position 8 - pattern 0
	db	1,0,0,1
	; position 9 - pattern 0
	db	1,0,0,1
	; position 10 - pattern 3
	db	1,0,0
	; position 14 - pattern 5
	db	1,0,0,1
	; position 15 - pattern 0
	db	1,0,0,1
	; position 16 - pattern 0
	db	1,0,0,1
	; position 17 - pattern 0
	db	1,0,0,1
	; position 18 - pattern 3
	db	1,0,0
	; position 21 - pattern 0
	db	1,0,0,1
	; position 22 - pattern 3
	db	1,0,0
	db	-1

	; 03:  hihat / 08|hihat
	; position 5 - pattern 0
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0
	; position 6 - pattern 3
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1
	; position 7 - pattern 0
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0
	; position 8 - pattern 0
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0
	; position 9 - pattern 0
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0
	; position 10 - pattern 3
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1
	; position 15 - pattern 0
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0
	; position 16 - pattern 0
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0
	; position 17 - pattern 0
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0
	; position 18 - pattern 3
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1
	; position 21 - pattern 0
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0
	; position 22 - pattern 3
	db	0,0,1,1,0,2,0,0,2,1,0,0,1,1,0,2,0,0,2,1,0,1,1,0,2,0,0,0,0,1,1,0,2,0,0,2,1,0,0,1
	db	-1

	; 04:  string high / 09|stringsHigh
	; position 0 - pattern 0
	db	3,0,2,7,6,4,1,2,8,5
	; position 1 - pattern 0
	db	3,0,2,7,6,4,1,2,8,5
	; position 7 - pattern 0
	db	3,0,2,7,6,4,1,2,8,5
	; position 8 - pattern 0
	db	3,0,2,7,6,4,1,2,8,5
	; position 9 - pattern 0
	db	3,0,2,7,6,4,1,2,8,5
	; position 10 - pattern 3
	db	3,0,2,7,6,4,1,2,8,5
	; position 15 - pattern 0
	db	3,0,2,7,6,4,1,2,8,5
	; position 16 - pattern 0
	db	3,0,2,7,6,4,1,2,8,5
	; position 17 - pattern 0
	db	3,0,2,7,6,4,1,2,8,5
	; position 18 - pattern 3
	db	3,0,2,7,6,4,1,2,8,5
	; position 21 - pattern 0
	db	3,0,2,7,6,4,1,2,8,5
	; position 22 - pattern 3
	db	3,0,2,7,6,4,1,2,8,5
	; position 23 - pattern 4
	db	3,0,2,7,6,4,1,2,8,5
	; position 24 - pattern 4
	db	3,0,2,7,6,4,1,2,8,5
	db	-1

	; 05:  string melo / 0E|stringsHighshort
	; position 9 - pattern 0
	db	0,1,2,3,1,4,5,1,2,3,4,3,2,1
	; position 10 - pattern 3
	db	0,1,2,3,1,4,5,1,2,3,4,3,2,1
	; position 15 - pattern 0
	db	0,1,2,3,1,4,5,1,2,3,4,3,2,1
	; position 16 - pattern 0
	db	0,1,2,3,1,4,5,1,2,3,4,3,2,1
	; position 17 - pattern 0
	db	0,1,2,3,1,4,5,1,2,3,4,3,2,1
	; position 18 - pattern 3
	db	0,1,2,3,1,4,5,1,2,3,4,3,2,1
	; position 21 - pattern 0
	db	0,1,2,3,1,4,5,1,2,3,4,3,2,1
	; position 22 - pattern 3
	db	0,1,2,3,1,4,5,1,2,3,4,3,2,1
	db	-1

	; 06:  string bass / 0C|stringsLow
	; position 0 - pattern 0
	db	1,4,2,3
	; position 1 - pattern 0
	db	1,4,2,3
	; position 2 - pattern 1
	db	0
	; position 3 - pattern 0
	db	1,4,2,3
	; position 4 - pattern 0
	db	1,4,2,3
	; position 5 - pattern 0
	db	1,4,2,3
	; position 6 - pattern 3
	db	1,4,2,3
	; position 7 - pattern 0
	db	1,4,2,3
	; position 8 - pattern 0
	db	1,4,2,3
	; position 9 - pattern 0
	db	1,4,2,3
	; position 10 - pattern 3
	db	1,4,2,3
	; position 11 - pattern 4
	db	0
	; position 12 - pattern 1
	db	0
	; position 13 - pattern 1
	db	0
	; position 14 - pattern 5
	db	0
	; position 15 - pattern 0
	db	1,4,2,3
	; position 16 - pattern 0
	db	1,4,2,3
	; position 17 - pattern 0
	db	1,4,2,3
	; position 18 - pattern 3
	db	1,4,2,3
	; position 19 - pattern 0
	db	1,4,2,3
	; position 20 - pattern 0
	db	1,4,2,3
	; position 21 - pattern 0
	db	1,4,2,3
	; position 22 - pattern 3
	db	1,4,2,3
	; position 23 - pattern 4
	db	0
	; position 24 - pattern 4
	db	0
	; position 25 - pattern 4
	db	0
	; position 26 - pattern 1
	db	0
	db	-1

	; 07:  stringLead / 0D|stringsHigh
	; position 0 - pattern 0
	db	1,4,2,3
	; position 1 - pattern 0
	db	1,4,2,3
	; position 2 - pattern 1
	db	0
	; position 3 - pattern 0
	db	1,4,2,3
	; position 4 - pattern 0
	db	1,4,2,3
	; position 5 - pattern 0
	db	1,4,2,3
	; position 6 - pattern 3
	db	1,4,2,3
	; position 7 - pattern 0
	db	1,4,2,3
	; position 8 - pattern 0
	db	1,4,2,3
	; position 9 - pattern 0
	db	1,4,2,3
	; position 10 - pattern 3
	db	1,4,2,3
	; position 11 - pattern 4
	db	0
	; position 12 - pattern 1
	db	0
	; position 13 - pattern 1
	db	0
	; position 14 - pattern 5
	db	0
	; position 15 - pattern 0
	db	1,4,2,3
	; position 16 - pattern 0
	db	1,4,2,3
	; position 17 - pattern 0
	db	1,4,2,3
	; position 18 - pattern 3
	db	1,4,2,3
	; position 19 - pattern 0
	db	1,4,2,3
	; position 20 - pattern 0
	db	1,4,2,3
	; position 21 - pattern 0
	db	1,4,2,3
	; position 22 - pattern 3
	db	1,4,2,3
	; position 23 - pattern 4
	db	0
	; position 24 - pattern 4
	db	0
	; position 25 - pattern 4
	db	0
	; position 26 - pattern 1
	db	0
	db	-1

	; 08:  Bassdrum / 00|bassdrum
	; position 3 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 4 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 5 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 6 - pattern 3
	db	3,7,1,5,6,1,0,3,7,1,5,6,4
	; position 7 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 8 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 9 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 10 - pattern 3
	db	3,7,1,5,6,1,0,3,7,1,5,6,4
	; position 11 - pattern 4
	db	1,1,3,1,0,1,1,3,1,0
	; position 12 - pattern 1
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 13 - pattern 1
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 14 - pattern 5
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,0
	; position 15 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 16 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 17 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 18 - pattern 3
	db	3,7,1,5,6,1,0,3,7,1,5,6,4
	; position 19 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 20 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 21 - pattern 0
	db	3,7,1,5,6,1,0,3,7,1,5,6,1,2,2
	; position 22 - pattern 3
	db	3,7,1,5,6,1,0,3,7,1,5,6,4
	; position 23 - pattern 4
	db	1,1,3,1,0,1,1,3,1,0
	; position 24 - pattern 4
	db	1,1,3,1,0,1,1,3,1,0
	; position 25 - pattern 4
	db	1,1,3,1,0,1,1,3,1,0
	db	-1

	; 09:  bassDrum / 00|bassdrum
	; position 3 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 4 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 5 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 6 - pattern 3
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4
	; position 7 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 8 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 9 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 10 - pattern 3
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4
	; position 11 - pattern 4
	db	4,4,4,4,4,4,4,4,4,4
	; position 12 - pattern 1
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 13 - pattern 1
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 14 - pattern 5
	db	4,6,2,5,2,3,7,3,5,1,4,6,2,5,2,3,7,0
	; position 15 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 16 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 17 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 18 - pattern 3
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4
	; position 19 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 20 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 21 - pattern 0
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4,8,4,5,4
	; position 22 - pattern 3
	db	4,8,4,5,4,4,8,4,5,4,4,8,4,5,4,4
	; position 23 - pattern 4
	db	4,4,4,4,4,4,4,4,4,4
	; position 24 - pattern 4
	db	4,4,4,4,4,4,4,4,4,4
	; position 25 - pattern 4
	db	4,4,4,4,4,4,4,4,4,4
	db	-1

