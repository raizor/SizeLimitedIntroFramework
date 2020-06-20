; Clinkster music converted from music.xrns 2013-12-01 22:49:58

%define USES_SINE 1
%define USES_SAWTOOTH 1
%define USES_SQUARE 1
%define USES_PARABOLA 1
%define USES_TRIANGLE 1
%define USES_NOISE 0
%define USES_VELOCITY 1
%define USES_LONG_NOTES 0
%define USES_DELAY 1
%define USES_PANNING 0
%define USES_INDEXDECAY 1
%define USES_GAIN 1

%define SUBSAMPLES_PER_TICK 8216
%define MAX_INSTRUMENT_SUBSAMPLES 786432
%define MAX_TOTAL_INSTRUMENT_SAMPLES 2359296
%define MAX_RELEASE_SUBSAMPLES 196608
%define TOTAL_SAMPLES 5177344
%define MAX_TRACK_INSTRUMENT_RENDERS 53

%define MAX_DELAY_LENGTH 32854
%define LEFT_DELAY_LENGTH 32854
%define RIGHT_DELAY_LENGTH 24872
%define DELAY_STRENGTH 0.60081957

%define NUMTRACKS 11
%define LOGNUMTICKS 12
%define MUSIC_LENGTH 2496
%define TICKS_PER_SECOND 21.46666667


	section instdata data align=1

_InstrumentData:
	; 00:  Chord / 00|Chord
	db	4,1,10,13,47,19,32,39,-26,21,-2,2,-84,-90,-60,16,-28,0,4
	db	65,127,4,0,0,127,2,0,1,127,4,0,0,127,2,0,0,127,4,0,0,127,4,0,1,127,4,0,-1
	; 01:  Chord / 00|Chord
	db	4,1,10,13,47,19,32,39,-26,21,-2,2,-84,-90,-60,16,-28,0,4
	db	54,127,2,0,2,127,2,0,9,127,4,0,1,127,4,0,0,127,4,0,1,127,4,0,0,127,4,0,0,127,4,0,0,127,4,0,0,127,4,0,0,127,4,0,-1
	; 02:  Chord / 00|Chord
	db	4,1,10,13,47,19,32,39,-26,21,-2,2,-84,-90,-60,16,-28,0,4
	db	72,127,4,0,2,127,4,0,1,127,4,0,0,127,4,0,1,127,4,0,2,127,4,0,-1
	; 03:  Lead / 03|Lead
	db	1,2,2,5,10,42,12,13,18,38,0,0,0,0,-20,3,-30,2,10
	db	24,127,32,127,4,127,2,0,7,127,4,0,3,127,4,127,2,0,4,127,48,127,32,127,16,0,0,127,8,0,10,127,8,127,2,0,0,127,4,0,1,48,1,127,8,127,2,0,1,80,1,127,2,0,1,127,2,0,0,127,8,127,2,0,1,127,4,127,2,0,0,127,8,0,0,127,4,127,2,0,1,127,4,127,2,0,0,127,4,127,2,0,1,127,8,127,4,127,2,0,0,127,4,127,2,0,0,127,8,127,4,127,2,0,0,127,8,127,2,0,1,127,8,127,4,127,2,0,0,127,2,0,0,127,8,127,4,127,2,0,0,127,4,127,2,0,1,127,8,127,4,127,2,0,3,80,12,0,-1
	; 04:  Lead / 03|Lead
	db	1,2,2,5,10,42,12,13,18,38,0,0,0,0,-20,3,-30,2,10
	db	12,127,12,0,28,127,24,0,2,127,24,0,0,127,2,0,2,127,48,127,32,0,4,127,2,0,1,127,2,0,0,127,48,0,11,127,16,0,-1
	; 05:  Lead / 03|Lead
	db	1,2,2,5,10,42,12,13,18,38,0,0,0,0,-20,3,-30,2,10
	db	29,127,24,0,2,127,24,0,3,127,2,0,20,127,2,0,2,127,4,0,4,127,16,127,12,127,4,0,1,127,24,127,8,127,4,0,0,127,4,0,3,127,2,0,0,127,4,127,2,0,-1
	db	-1
	; 06:  Bass / 01|Bass
	db	2,1,2,0,7,6,4,5,16,11,12,0,-122,0,14,48,-88,-6,-8
	db	24,127,64,0,2,127,12,0,1,127,64,127,10,127,8,127,2,0,0,127,16,0,1,8,8,16,8,32,4,127,12,127,10,127,8,127,4,127,2,0,0,127,16,0,0,127,8,127,2,0,0,127,16,127,4,0,1,127,8,127,2,0,3,127,2,127,1,0,0,127,16,0,1,127,2,127,1,0,1,127,2,127,1,0,0,127,2,0,0,127,1,0,0,127,2,127,1,0,1,127,1,0,-1
	; 07:  Bass / 01|Bass
	db	2,1,2,0,7,6,4,5,16,11,12,0,-122,0,14,48,-88,-6,-8
	db	37,127,16,0,3,127,2,0,12,127,16,0,-1
	; 08:  Bassdrum / 02|Bassdrum
	db	0,3,0,55,0,15,32,64,11,31,16,-91,-58,0,36,53,-120,-34,-64
	db	36,127,20,127,16,127,8,127,6,127,4,127,2,0,-1
	; 09:  Snare / 04|Snare
	db	2,0,100,96,45,51,14,22,-6,28,-12,-10,-20,6,0,42,-120,-56,4
	db	41,127,4,127,2,0,2,127,4,0,-1
	; 10:  Zap / 05|Zap
	db	0,1,26,39,0,22,2,4,1,59,65,-55,-118,-20,-74,20,-120,-102,-94
	db	33,127,2,0,0,127,2,0,1,127,4,0,0,127,2,0,1,127,4,0,0,127,2,0,0,127,2,0,1,127,2,0,0,127,2,0,5,127,2,0,3,127,2,0,0,127,2,0,0,127,2,0,1,127,2,0,0,127,2,0,1,127,2,0,5,127,2,0,2,127,2,0,4,127,4,0,1,127,4,0,0,127,4,0,1,127,4,0,8,127,4,0,2,127,4,0,1,127,4,0,0,127,4,0,1,127,4,0,3,127,4,0,2,127,4,0,1,127,4,0,0,127,4,0,1,127,4,0,-1
	db	-1,-1

	section notepos data align=1

_NotePositions:
	; 00:  Chord / 00|Chord
	; position 6 - pattern 4
	db	-4,72,16,16,16,16,16,16,16
	; position 7 - pattern 5
	db	16,16,16,16,16,16,16,16
	; position 8 - pattern 4
	db	16,16,16,16,16,16,16,16
	; position 9 - pattern 6
	db	16,16,16,16,16,16,16,16
	; position 10 - pattern 7
	db	16,16
	; position 11 - pattern 8
	db	-1,152
	; position 12 - pattern 9
	db	-1,128
	; position 13 - pattern 10
	db	88,16,16,16,16,16,16,16
	; position 14 - pattern 11
	db	16,16,16,16,16,16,16,16
	; position 15 - pattern 10
	db	16,16,16,16,16,16,16,16
	; position 16 - pattern 12
	db	16,16,16,8,24,16

	; 01:  Chord / 00|Chord
	; position 6 - pattern 4
	db	-4,72,16,16,16,16,16,16,16
	; position 7 - pattern 5
	db	16,16,16,16,16,16,16,16
	; position 8 - pattern 4
	db	16,16,16,16,16,16,16,16
	; position 9 - pattern 6
	db	16,16,16,16,16,16,16,16
	; position 10 - pattern 7
	db	16,16
	; position 11 - pattern 8
	db	-1,152
	; position 12 - pattern 9
	db	-1,128
	; position 13 - pattern 10
	db	88,16,16,16,16,16,16,16
	; position 14 - pattern 11
	db	16,16,16,16,16,16,16,16
	; position 15 - pattern 10
	db	16,16,16,16,16,16,16,16
	; position 16 - pattern 12
	db	16,16,16,8,24,16

	; 02:  Chord / 00|Chord
	; position 6 - pattern 4
	db	-4,72,16,16,16,16,16,16,16
	; position 7 - pattern 5
	db	16,16,16,16,16,16,16,16
	; position 8 - pattern 4
	db	16,16,16,16,16,16,16,16
	; position 9 - pattern 6
	db	16,16,16,16,16,16,16,16
	; position 10 - pattern 7
	db	16,16
	; position 13 - pattern 10
	db	-2,112,16,16,16,16,16,16,16
	; position 14 - pattern 11
	db	16,16,16,16,16,16,16,16
	; position 15 - pattern 10
	db	16,16,16,16,16,16,16,16
	; position 16 - pattern 12
	db	16,16,16,32,16

	; 03:  Lead / 03|Lead
	; position 0 - pattern 0
	db	0,32,32,32,24,4
	; position 1 - pattern 1
	db	4,32,32,32
	; position 2 - pattern 2
	db	32,32,32,48
	; position 3 - pattern 1
	db	16,32,32,32
	; position 4 - pattern 2
	db	32,32,32,48
	; position 5 - pattern 3
	db	16,32,32
	; position 6 - pattern 4
	db	-1,128,8,8,40,4,4,24,8,12,8
	; position 7 - pattern 5
	db	12,8,4,12,28,4,4,4,12,4,12,8,12,4,4,4
	; position 8 - pattern 4
	db	4,8,8,40,4,4,24,8,12,8
	; position 9 - pattern 6
	db	12,24,32,4,4,24,4,4
	; position 10 - pattern 7
	db	32,24,40,24
	; position 11 - pattern 8
	db	40,48,16,4,8,12,32
	; position 12 - pattern 9
	db	8,48,16,4,8,12,32,4
	; position 13 - pattern 10
	db	4,8,8,40,4,4,24,8,12,8
	; position 14 - pattern 11
	db	12,8,4,12,28,4,4,4,12,4,12,8,12,4,4,4
	; position 15 - pattern 10
	db	4,8,8,40,4,4,24,8,12,8
	; position 16 - pattern 12
	db	12,24,40,24,4,4
	; position 17 - pattern 13
	db	32

	; 04:  Lead / 03|Lead
	; position 1 - pattern 1
	db	-1,128,48,8,8
	; position 2 - pattern 2
	db	64,48,8,8,48
	; position 3 - pattern 1
	db	16,48,8,8
	; position 4 - pattern 2
	db	64,48,8,8,48
	; position 5 - pattern 3
	db	16,96
	; position 6 - pattern 4
	db	120
	; position 7 - pattern 5
	db	-1,128
	; position 8 - pattern 4
	db	-1,128
	; position 9 - pattern 6
	db	-1,128
	; position 13 - pattern 10
	db	-3,0
	; position 14 - pattern 11
	db	-1,128
	; position 15 - pattern 10
	db	-1,128
	; position 16 - pattern 12
	db	-1,128,24

	; 05:  Lead / 03|Lead
	; position 1 - pattern 1
	db	-1,128,16,40,4,4,12,4,12,4
	; position 2 - pattern 2
	db	32,16,40,4,4,12,4,32
	; position 3 - pattern 1
	db	16,16,40,4,4,12,4,12,4
	; position 4 - pattern 2
	db	32,16,40,4,4,12,4,32
	; position 5 - pattern 3
	db	16,16,8,40
	; position 6 - pattern 4
	db	-1,152
	; position 7 - pattern 5
	db	-1,128
	; position 8 - pattern 4
	db	-1,128
	; position 9 - pattern 6
	db	-1,128
	; position 13 - pattern 10
	db	-3,0
	; position 14 - pattern 11
	db	-1,128
	; position 15 - pattern 10
	db	-1,128
	; position 16 - pattern 12
	db	-1,128,24

	; 06:  Bass / 01|Bass
	; position 1 - pattern 1
	db	-1,128,64
	; position 2 - pattern 2
	db	64,64
	; position 3 - pattern 1
	db	64,64
	; position 4 - pattern 2
	db	64,64
	; position 5 - pattern 3
	db	64
	; position 6 - pattern 4
	db	-1,192,4,4,20,4,8,20,4,8,20,4,4,4,8,4,4,4
	; position 7 - pattern 5
	db	4,4,4,20,4,8,20,4,8,20,4,4,4,8,4,4,4
	; position 8 - pattern 4
	db	4,4,4,20,4,8,20,4,8,20,4,4,4,8,4,4,4
	; position 9 - pattern 6
	db	4,4,4,20,4,8,20,4,4,4,20,4,8,12
	; position 10 - pattern 7
	db	12,4,4,20,4,8,20,4,4,4,20,4,8,12
	; position 11 - pattern 8
	db	12,4,4,20,4,6,6,4,16,4,4,20,4,8,12
	; position 12 - pattern 9
	db	12,4,4,20,4,6,6,4,16,4,4,20,4
	; position 13 - pattern 10
	db	32,4,4,20,4,8,20,4,8,20,4,4,4,8,4,4,4
	; position 14 - pattern 11
	db	4,4,4,20,4,8,20,4,8,20,4,4,4,8,4,4,4
	; position 15 - pattern 10
	db	4,4,4,20,4,8,20,4,8,20,4,4,4,8,4,4,4
	; position 16 - pattern 12
	db	4,4,4,20,4,4,12,16,4,4,20,4,8,8
	; position 17 - pattern 13
	db	16,12,12,16

	; 07:  Bass / 01|Bass
	; position 2 - pattern 2
	db	-2,112
	; position 4 - pattern 2
	db	-2,0
	; position 11 - pattern 8
	db	-4,112,6,6,4
	; position 12 - pattern 9
	db	112,6,6,4

	; 08:  Bassdrum / 02|Bassdrum
	; position 5 - pattern 3
	db	-4,48
	; position 6 - pattern 4
	db	16,8,24,8,24,8,20,4,8,16
	; position 7 - pattern 5
	db	8,8,24,8,24,8,20,4,8
	; position 8 - pattern 4
	db	24,8,24,8,24,8,20,4,8,16
	; position 9 - pattern 6
	db	8,8,24,8,24,8,20,4,8
	; position 10 - pattern 7
	db	24,8,24,8,24,8,20,4,8,16
	; position 11 - pattern 8
	db	8,8,24,6,6,4,20,4,20,4,8
	; position 12 - pattern 9
	db	24,8,24,6,6,4,20,4,20,4
	; position 13 - pattern 10
	db	32,8,24,8,24,8,20,4,8,16
	; position 14 - pattern 11
	db	8,8,24,8,24,8,20,4,8,16
	; position 15 - pattern 10
	db	8,8,24,8,24,8,20,4,8,16
	; position 16 - pattern 12
	db	8,8,24,8,24,8,20,4,12
	; position 17 - pattern 13
	db	20

	; 09:  Snare / 04|Snare
	; position 6 - pattern 4
	db	-4,80,32,32,20,12
	; position 7 - pattern 5
	db	32,32,32,20,12
	; position 8 - pattern 4
	db	32,32,32,20,12
	; position 9 - pattern 6
	db	32,32,32,20,12
	; position 10 - pattern 7
	db	32,32,32,20,12
	; position 11 - pattern 8
	db	32,32,16,16,20,12,8,4
	; position 12 - pattern 9
	db	20,32,16,16
	; position 13 - pattern 10
	db	64,32,32,20,12
	; position 14 - pattern 11
	db	32,32,32,20,12
	; position 15 - pattern 10
	db	32,32,32,20,12
	; position 16 - pattern 12
	db	32,32,32
	; position 17 - pattern 13
	db	48

	; 10:  Zap / 05|Zap
	; position 5 - pattern 3
	db	-4,32,4,4,4,4,4,4,4
	; position 6 - pattern 4
	db	4,4,8,12,12,8,12,8,4,8,12,8,4,4,4,8,4,4
	; position 7 - pattern 5
	db	4,4,8,12,12,8,12,8,4,8,12,8,4,4,4,8,4,4
	; position 8 - pattern 4
	db	4,4,8,12,12,8,12,8,4,8,12,8,4,4,4,8,4,4
	; position 9 - pattern 6
	db	4,4,8,12,12,8,12,8,4,8,12,12,8,12
	; position 10 - pattern 7
	db	8,4,8,12,12,8,12,8,4,8,12,12,8,12
	; position 11 - pattern 8
	db	8,4,8,12,8,4,2,6,4,4,4,4,4,4,8,12,12,8,12
	; position 12 - pattern 9
	db	8,4,8,12,8,4,2,6,4,4,4,4,4,4,8,12,12
	; position 13 - pattern 10
	db	28,4,8,12,12,8,12,8,4,8,12,8,4,4,4,8,4,4
	; position 14 - pattern 11
	db	4,4,8,12,12,8,12,8,4,8,12,8,4,4,4,8,4,4
	; position 15 - pattern 10
	db	4,4,8,12,12,8,12,8,4,8,12,8,4,4,4,8,4,4
	; position 16 - pattern 12
	db	4,4,8,12,12,8,4,4,4,4,4,4,8,12,12
	; position 17 - pattern 13
	db	28


	section notesamp data align=1

_NoteSamples:
	; 00:  Chord / 00|Chord
	; position 6 - pattern 4
	db	0,0,0,0,2,2,4,4
	; position 7 - pattern 5
	db	0,0,0,0,2,2,4,4
	; position 8 - pattern 4
	db	0,0,0,0,2,2,4,4
	; position 9 - pattern 6
	db	0,0,0,0,0,0,0,0
	; position 10 - pattern 7
	db	0,0
	; position 11 - pattern 8
	db	1
	; position 12 - pattern 9
	db	1
	; position 13 - pattern 10
	db	2,2,2,2,5,5,6,6
	; position 14 - pattern 11
	db	2,2,2,2,5,5,6,6
	; position 15 - pattern 10
	db	2,2,2,2,5,5,6,6
	; position 16 - pattern 12
	db	2,2,2,3,2,2
	db	-1

	; 01:  Chord / 00|Chord
	; position 6 - pattern 4
	db	4,3,2,3,6,5,5,7
	; position 7 - pattern 5
	db	4,3,2,3,6,5,5,7
	; position 8 - pattern 4
	db	4,3,2,3,6,5,5,7
	; position 9 - pattern 6
	db	4,4,4,4,4,4,4,4
	; position 10 - pattern 7
	db	4,4
	; position 11 - pattern 8
	db	0
	; position 12 - pattern 9
	db	0
	; position 13 - pattern 10
	db	6,5,4,5,9,8,8,10
	; position 14 - pattern 11
	db	6,5,4,5,9,8,8,10
	; position 15 - pattern 10
	db	6,5,4,5,9,8,8,10
	; position 16 - pattern 12
	db	6,6,6,1,6,6
	db	-1

	; 02:  Chord / 00|Chord
	; position 6 - pattern 4
	db	0,0,0,0,4,1,2,2
	; position 7 - pattern 5
	db	0,0,0,0,4,1,2,2
	; position 8 - pattern 4
	db	0,0,0,0,4,1,2,2
	; position 9 - pattern 6
	db	0,0,0,0,0,0,0,0
	; position 10 - pattern 7
	db	0,0
	; position 13 - pattern 10
	db	1,1,1,1,5,3,4,4
	; position 14 - pattern 11
	db	1,1,1,1,5,3,4,4
	; position 15 - pattern 10
	db	1,1,1,1,5,3,4,4
	; position 16 - pattern 12
	db	1,1,1,1,1
	db	-1

	; 03:  Lead / 03|Lead
	; position 0 - pattern 0
	db	2,5,2,5,13,16
	; position 1 - pattern 1
	db	1,4,7,8
	; position 2 - pattern 2
	db	1,4,6,52
	; position 3 - pattern 1
	db	1,4,7,8
	; position 4 - pattern 2
	db	1,4,6,52
	; position 5 - pattern 3
	db	1,2,0
	; position 6 - pattern 4
	db	35,44,25,45,41,35,42,37,32,29
	; position 7 - pattern 5
	db	37,39,46,24,24,26,28,30,28,27,19,18,36,29,32,27
	; position 8 - pattern 4
	db	35,44,25,45,41,35,42,37,32,29
	; position 9 - pattern 6
	db	25,10,24,21,18,18,17,11
	; position 10 - pattern 7
	db	25,37,22,17
	; position 11 - pattern 8
	db	18,12,39,11,37,25,9
	; position 12 - pattern 9
	db	18,12,39,11,37,25,29,32
	; position 13 - pattern 10
	db	40,49,29,50,47,40,48,42,39,34
	; position 14 - pattern 11
	db	42,43,51,28,28,31,33,38,33,32,23,22,41,34,39,32
	; position 15 - pattern 10
	db	40,49,29,50,47,40,48,42,39,34
	; position 16 - pattern 12
	db	29,14,22,22,20,15
	; position 17 - pattern 13
	db	3
	db	-1

	; 04:  Lead / 03|Lead
	; position 1 - pattern 1
	db	4,6,7,8
	; position 2 - pattern 2
	db	4,6,7,8,9
	; position 3 - pattern 1
	db	4,6,7,8
	; position 4 - pattern 2
	db	4,6,7,8,9
	; position 5 - pattern 3
	db	5,0
	; position 6 - pattern 4
	db	1
	; position 7 - pattern 5
	db	1
	; position 8 - pattern 4
	db	1
	; position 9 - pattern 6
	db	1
	; position 13 - pattern 10
	db	2
	; position 14 - pattern 11
	db	2
	; position 15 - pattern 10
	db	2
	; position 16 - pattern 12
	db	2,3
	db	-1

	; 05:  Lead / 03|Lead
	; position 1 - pattern 1
	db	4,8,7,10,11,10,6,4,5
	; position 2 - pattern 2
	db	4,8,7,10,11,10,5,14
	; position 3 - pattern 1
	db	4,8,7,10,11,10,6,4,5
	; position 4 - pattern 2
	db	4,8,7,10,11,10,5,14
	; position 5 - pattern 3
	db	13,12,9,2
	; position 6 - pattern 4
	db	0
	; position 7 - pattern 5
	db	0
	; position 8 - pattern 4
	db	0
	; position 9 - pattern 6
	db	0
	; position 13 - pattern 10
	db	1
	; position 14 - pattern 11
	db	1
	; position 15 - pattern 10
	db	1
	; position 16 - pattern 12
	db	1,3
	db	-1

	; 06:  Bass / 01|Bass
	; position 1 - pattern 1
	db	0,2
	; position 2 - pattern 2
	db	0,2
	; position 3 - pattern 1
	db	0,2
	; position 4 - pattern 2
	db	0,2
	; position 5 - pattern 3
	db	0
	; position 6 - pattern 4
	db	5,23,3,22,5,3,22,13,11,25,17,28,16,27,17,27,30
	; position 7 - pattern 5
	db	5,23,3,22,5,3,22,13,11,25,17,28,16,27,17,27,30
	; position 8 - pattern 4
	db	5,23,3,22,5,3,22,13,11,25,17,28,16,27,17,27,30
	; position 9 - pattern 6
	db	5,23,3,22,5,3,22,5,23,3,22,5,4,1
	; position 10 - pattern 7
	db	5,23,3,22,5,3,22,5,23,3,22,5,4,1
	; position 11 - pattern 8
	db	5,23,3,22,5,5,5,24,5,23,3,22,5,4,1
	; position 12 - pattern 9
	db	5,23,3,22,5,5,5,24,5,23,3,22,4
	; position 13 - pattern 10
	db	14,26,11,25,14,11,25,19,18,29,21,32,20,31,21,31,33
	; position 14 - pattern 11
	db	14,26,11,25,14,11,25,19,18,29,21,32,20,31,21,31,33
	; position 15 - pattern 10
	db	14,26,11,25,14,11,25,19,18,29,21,32,20,31,21,31,33
	; position 16 - pattern 12
	db	14,26,11,25,14,11,15,14,26,11,25,14,12,6
	; position 17 - pattern 13
	db	10,9,8,7
	db	-1

	; 07:  Bass / 01|Bass
	; position 2 - pattern 2
	db	0
	; position 4 - pattern 2
	db	0
	; position 11 - pattern 8
	db	1,1,1,2
	; position 12 - pattern 9
	db	1,1,1,2
	db	-1

	; 08:  Bassdrum / 02|Bassdrum
	; position 5 - pattern 3
	db	1
	; position 6 - pattern 4
	db	4,2,4,4,4,4,4,4,4,4
	; position 7 - pattern 5
	db	4,2,4,4,4,4,4,4,4
	; position 8 - pattern 4
	db	4,2,4,4,4,4,4,4,4,4
	; position 9 - pattern 6
	db	4,2,4,4,4,4,4,4,0
	; position 10 - pattern 7
	db	4,4,4,4,4,4,4,4,4,4
	; position 11 - pattern 8
	db	4,2,4,3,4,0,5,4,4,4,4
	; position 12 - pattern 9
	db	4,2,4,3,4,0,5,4,4,4
	; position 13 - pattern 10
	db	4,2,4,4,4,4,4,4,4,4
	; position 14 - pattern 11
	db	4,2,4,4,4,4,4,4,4,4
	; position 15 - pattern 10
	db	4,2,4,4,4,4,4,4,4,4
	; position 16 - pattern 12
	db	4,2,4,4,4,4,4,4,0
	; position 17 - pattern 13
	db	4
	db	-1

	; 09:  Snare / 04|Snare
	; position 6 - pattern 4
	db	0,0,0,0,0
	; position 7 - pattern 5
	db	0,0,0,0,0
	; position 8 - pattern 4
	db	0,0,0,0,0
	; position 9 - pattern 6
	db	0,0,0,0,0
	; position 10 - pattern 7
	db	0,0,0,0,0
	; position 11 - pattern 8
	db	0,0,1,0,0,0,1,1
	; position 12 - pattern 9
	db	0,0,1,0
	; position 13 - pattern 10
	db	0,0,0,0,0
	; position 14 - pattern 11
	db	0,0,0,0,0
	; position 15 - pattern 10
	db	0,0,0,0,0
	; position 16 - pattern 12
	db	0,2,0
	; position 17 - pattern 13
	db	1
	db	-1

	; 10:  Zap / 05|Zap
	; position 5 - pattern 3
	db	6,6,6,6,6,6,6,6
	; position 6 - pattern 4
	db	19,6,22,27,6,22,27,16,2,23,28,18,29,1,24,13,12,11
	; position 7 - pattern 5
	db	19,6,22,27,6,22,27,16,2,23,28,18,29,1,24,13,12,11
	; position 8 - pattern 4
	db	19,6,22,27,6,22,27,16,2,23,28,18,29,1,24,13,12,11
	; position 9 - pattern 6
	db	19,6,22,27,6,22,27,19,6,22,27,6,22,27
	; position 10 - pattern 7
	db	19,6,22,27,6,22,27,19,6,22,27,6,22,27
	; position 11 - pattern 8
	db	19,6,22,27,6,6,6,6,10,9,7,5,19,6,22,27,6,22,27
	; position 12 - pattern 9
	db	19,6,22,27,6,6,6,6,10,9,7,5,19,6,22,27,6
	; position 13 - pattern 10
	db	21,8,23,28,8,23,28,17,4,25,30,20,31,3,26,15,14,13
	; position 14 - pattern 11
	db	21,8,23,28,8,23,28,17,4,25,30,20,31,3,26,15,14,13
	; position 15 - pattern 10
	db	21,8,23,28,8,23,28,17,4,25,30,20,31,3,26,15,14,13
	; position 16 - pattern 12
	db	21,8,23,28,8,23,0,0,0,0,21,8,23,28,8
	; position 17 - pattern 13
	db	21
	db	-1

