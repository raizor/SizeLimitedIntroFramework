; Clinkster music converted from music.xrns 2013-10-21 20:36:27

%define USES_SINE 1
%define USES_SAWTOOTH 1
%define USES_SQUARE 1
%define USES_PARABOLA 1
%define USES_TRIANGLE 0
%define USES_NOISE 0
%define USES_VELOCITY 0
%define USES_LONG_NOTES 0
%define USES_DELAY 1
%define USES_PANNING 0
%define USES_INDEXDECAY 1
%define USES_GAIN 1

%define SUBSAMPLES_PER_TICK 18900
%define MAX_INSTRUMENT_SUBSAMPLES 917504
%define MAX_TOTAL_INSTRUMENT_SAMPLES 1572864
%define MAX_RELEASE_SUBSAMPLES 131072
%define TOTAL_SAMPLES 9109504
%define MAX_TRACK_INSTRUMENT_RENDERS 16

%define MAX_DELAY_LENGTH 28312
%define LEFT_DELAY_LENGTH 14156
%define RIGHT_DELAY_LENGTH 28312
%define DELAY_STRENGTH 0.39000005

%define NUMTRACKS 10
%define LOGNUMTICKS 11
%define MUSIC_LENGTH 1920
%define TICKS_PER_SECOND 9.33333333


	section instdata data align=1

_InstrumentData:
	; 00:  2-Chords / 01|Clinkster (Basic)
	db	1,3,6,0,17,11,22,20,13,10,0,0,0,0,-39,14,-23,-6,3
	db	64,8,1,0,4,1,0,-1
	; 01:  2-Chords / 01|Clinkster (Basic)
	db	1,3,6,0,17,11,22,20,13,10,0,0,0,0,-39,14,-23,-6,3
	db	57,8,0,10,4,0,0,16,1,0,4,2,1,0,-1
	; 02:  2-Chords / 01|Clinkster (Basic)
	db	1,3,6,0,17,11,22,20,13,10,0,0,0,0,-39,14,-23,-6,3
	db	69,8,0,1,1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,0,-1
	; 03:  2-Chords / 01|Clinkster (Basic)
	db	1,3,6,0,17,11,22,20,13,10,0,0,0,0,-39,14,-23,-6,3
	db	57,1,0,23,1,0,11,1,0,-1
	; 04:  3-Lead / 05|Clinkster (Basic)
	db	1,2,9,9,69,98,16,111,16,25,0,0,52,33,0,16,-29,-9,-3
	db	64,2,0,3,4,0,0,4,2,1,0,1,1,0,0,2,1,0,1,4,2,1,0,1,4,2,0,-1
	; 05:  4-Hihat / 02|hihat
	db	0,0,13,23,0,5,12,37,11,11,55,22,0,0,0,22,-59,-14,1
	db	22,4,1,0,11,4,0,11,4,0,11,4,1,0,11,1,0,-1
	; 06:  5-Snare / 03|snare1
	db	2,0,100,96,79,73,19,3,-5,26,-12,-10,-20,6,-4,38,-120,-58,6
	db	12,1,0,-1
	; 07:  6-Bassdrum / 04|basicbassdrum
	db	0,0,0,0,0,0,1,0,4,14,48,-120,-70,-70,-70,85,-59,-27,-3
	db	22,1,0,-1
	db	-1
	; 08:  1-Bass / 00|Clinkster (Basic)
	db	0,3,5,5,0,9,1,12,8,25,-72,0,0,-70,0,0,-57,-27,-14
	db	16,16,0,4,41,32,16,0,4,32,27,16,8,0,1,28,8,0,2,16,0,1,16,8,4,0,4,8,0,1,8,0,-1
	; 09:  1-Bass / 00|Clinkster (Basic)
	db	0,3,5,5,0,9,1,12,8,25,-72,0,0,-70,0,0,-57,-27,-14
	db	28,16,0,4,41,32,16,0,4,27,16,8,2,0,1,28,8,2,0,2,16,0,1,8,4,2,0,4,8,0,-1
	db	-1,-1

	section notepos data align=1

_NotePositions:
	; 00:  2-Chords / 01|Clinkster (Basic)
	; position 3 - pattern 2
	db	-1,192,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 4 - pattern 3
	db	4,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 5 - pattern 4
	db	4,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 6 - pattern 5
	db	4,6,4,4,4,2,4,8,6,4,6,4,4
	; position 7 - pattern 6
	db	8,4,2,4,4,2,4,2,4,4,2,4,2,4,4,2,4,2,4,4
	; position 8 - pattern 7
	db	2,4,2,4,4,2,4,2,4,4,2,4,2,4,4,2,4,4
	; position 9 - pattern 8
	db	8,4,2,4,4,2,4,2,4,4,2,4,2,4,4,2,4,2,4,4
	; position 10 - pattern 9
	db	2,4,2,4,4,2,4,2,4,4,2
	; position 11 - pattern 4
	db	32,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 12 - pattern 5
	db	4,6,4,4,4,2,4,8,6,4,6,4,4
	; position 13 - pattern 10
	db	8,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 14 - pattern 11
	db	4,6,4,4,4,2,4,8,6,4,4,4,2,4
	; position 15 - pattern 12
	db	8,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 16 - pattern 13
	db	4,6,4,4,4,2,4,8,8,19
	; position 17 - pattern 14
	db	9,8,11
	; position 20 - pattern 16
	db	-1,169,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 21 - pattern 17
	db	4,6,4,4,4,2,4,8,6,4,4,2,4,4
	; position 22 - pattern 16
	db	8,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 23 - pattern 18
	db	4,6,4,4,4,2,4,8,6,4,4,2,4,4
	; position 24 - pattern 19
	db	8,3,3,4,3,3,3,3,4,3,3,3,3,4,3,3,3,3,2,3,3
	; position 25 - pattern 20
	db	2,3,3,4,3,3,3,3,4,3,3,3,3,4,3,3,3,3,2,3,3
	; position 26 - pattern 19
	db	2,3,3,4,3,3,3,3,4,3,3,3,3,4,3,3,3,3,2,3,3
	; position 27 - pattern 21
	db	2,3,3,4,3,3,3,3,4,3,3,8,19
	; position 28 - pattern 22
	db	9,8,11,15

	; 01:  2-Chords / 01|Clinkster (Basic)
	; position 3 - pattern 2
	db	-1,192,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 4 - pattern 3
	db	4,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 5 - pattern 4
	db	4,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 6 - pattern 5
	db	4,6,4,4,4,2,4,8,6,4,6,4,4
	; position 7 - pattern 6
	db	8,4,2,4,4,2,4,2,4,4,2,4,2,4,4,2,4,2,4,4
	; position 8 - pattern 7
	db	2,4,2,4,4,2,4,2,4,4,2,4,2,4,4,2,4,4
	; position 9 - pattern 8
	db	8,4,2,4,4,2,4,2,4,4,2,4,2,4,4,2,4,2,4,4
	; position 10 - pattern 9
	db	2,4,2,4,4,2,4,2,4,4,2
	; position 11 - pattern 4
	db	32,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 12 - pattern 5
	db	4,6,4,4,4,2,4,8,6,4,6,4,4
	; position 13 - pattern 10
	db	8,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 14 - pattern 11
	db	4,6,4,4,4,2,4,8,6,4,4,4,2,4
	; position 15 - pattern 12
	db	8,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 16 - pattern 13
	db	4,6,4,4,4,2,4,8,29
	; position 17 - pattern 14
	db	7,19
	; position 20 - pattern 16
	db	-1,169,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 21 - pattern 17
	db	4,6,4,4,4,2,4,8,6,4,4,2,4,4
	; position 22 - pattern 16
	db	8,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 23 - pattern 18
	db	4,6,4,4,4,2,4,8,6,4,4,2,4,4
	; position 24 - pattern 19
	db	8,3,3,4,3,3,3,3,4,3,3,3,3,4,3,3,3,3,2,3,3
	; position 25 - pattern 20
	db	2,3,3,4,3,3,3,3,4,3,3,3,3,4,3,3,3,3,2,3,3
	; position 26 - pattern 19
	db	2,3,3,4,3,3,3,3,4,3,3,3,3,4,3,3,3,3,2,3,3
	; position 27 - pattern 21
	db	2,3,3,4,3,3,3,3,4,3,3,29
	; position 28 - pattern 22
	db	7,19,21

	; 02:  2-Chords / 01|Clinkster (Basic)
	; position 3 - pattern 2
	db	-1,192,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 4 - pattern 3
	db	4,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 5 - pattern 4
	db	4,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 6 - pattern 5
	db	4,6,4,4,4,2,4,8,6,4,6,4,4
	; position 7 - pattern 6
	db	8,4,2,4,4,2,4,2,4,4,2,4,2,4,4,2,4,2,4,4
	; position 8 - pattern 7
	db	2,4,2,4,4,2,4,2,4,4,2,4,2,4,4,2,4,4
	; position 9 - pattern 8
	db	8,4,2,4,4,2,4,2,4,4,2,4,2,4,4,2,4,2,4,4
	; position 10 - pattern 9
	db	2,4,2,4,4,2,4,2,4,4,2
	; position 11 - pattern 4
	db	32,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 12 - pattern 5
	db	4,6,4,4,4,2,4,8,6,4,6,4,4
	; position 13 - pattern 10
	db	8,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 14 - pattern 11
	db	4,6,4,4,4,2,4,8,6,4,4,4,2,4
	; position 15 - pattern 12
	db	8,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 16 - pattern 13
	db	4,6,4,4,4,2,4,8,12,15,4
	; position 17 - pattern 14
	db	2,3,4,4,5,6
	; position 20 - pattern 16
	db	-1,169,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 21 - pattern 17
	db	4,6,4,4,4,2,4,8,6,4,4,2,4,4
	; position 22 - pattern 16
	db	8,6,4,4,4,2,4,8,6,4,4,4,2,4,4
	; position 23 - pattern 18
	db	4,6,4,4,4,2,4,8,6,4,4,2,4,4
	; position 24 - pattern 19
	db	8,3,3,4,3,3,3,3,4,3,3,3,3,4,3,3,3,3,2,3,3
	; position 25 - pattern 20
	db	2,3,3,4,3,3,3,3,4,3,3,3,3,4,3,3,3,3,2,3,3
	; position 26 - pattern 19
	db	2,3,3,4,3,3,3,3,4,3,3,3,3,4,3,3,3,3,2,3,3
	; position 27 - pattern 21
	db	2,3,3,4,3,3,3,3,4,3,3,12,15,4
	; position 28 - pattern 22
	db	2,3,4,4,5,6,8,3

	; 03:  2-Chords / 01|Clinkster (Basic)
	; position 7 - pattern 6
	db	-2,192,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2
	; position 8 - pattern 7
	db	4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2,4,3,5
	; position 9 - pattern 8
	db	8,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2
	; position 10 - pattern 9
	db	4,3,3,2,2,2,4,3,3,2,2,2,4
	; position 20 - pattern 16
	db	-3,96,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2
	; position 21 - pattern 17
	db	4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2
	; position 22 - pattern 16
	db	20,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2
	; position 23 - pattern 18
	db	4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2
	; position 24 - pattern 19
	db	20,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2
	; position 25 - pattern 20
	db	4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2
	; position 26 - pattern 19
	db	4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2,4,3,3,2,2,2
	; position 27 - pattern 21
	db	4,3,3,2,2,2,4,3,3,2,2,2,12,4,8,9
	; position 28 - pattern 22
	db	26,4

	; 04:  3-Lead / 05|Clinkster (Basic)
	; position 13 - pattern 10
	db	-4,64,2,2,2,6,2,2,4,4,4,4,4,2,4,4,2,4,4,4
	; position 14 - pattern 11
	db	4,2,2,2,4,2,2,2,2,1,1,2,2,4,4,2,2,2,4,2,2,2,4,4
	; position 15 - pattern 12
	db	8,2,2,2,6,2,2,4,4,4,4,4,2,4,4,2,4,4,4
	; position 16 - pattern 13
	db	4,2,2,2,4,2,2,2,2,1,1,2,2,4,4,3,3,3,6,3,4,5,4
	; position 17 - pattern 14
	db	2,3,4,4,4,7
	; position 20 - pattern 16
	db	-1,169,2,2,2,6,2,2,4,4,4,4,4,2,4,4,2,4,4,4
	; position 21 - pattern 17
	db	4,2,2,2,4,2,2,2,2,1,1,2,2,4,4,2,2,2,4,2,2,2,4,4
	; position 22 - pattern 16
	db	8,2,2,2,6,2,2,4,4,4,4,4,2,4,4,2,4,4,4
	; position 23 - pattern 18
	db	4,2,2,2,4,2,2,2,2,1,1,2,2,4,4,2,2,2,4,2,2,2,4,4
	; position 24 - pattern 19
	db	8,2,2,2,6,2,2,4,4,4,4,4,2,4,4,2,4,4,4
	; position 25 - pattern 20
	db	4,2,2,2,4,2,2,2,2,1,1,2,2,4,4,2,2,2,4,2,2,2,4,4,4
	; position 26 - pattern 19
	db	4,2,2,2,6,2,2,4,4,4,4,4,2,4,4,2,4,4,4
	; position 27 - pattern 21
	db	4,2,2,2,4,2,2,2,2,1,1,2,2,4,4,3,3,3,6,3,4,5,4
	; position 28 - pattern 22
	db	2,3,4,4,4,7,4,4,4

	; 05:  4-Hihat / 02|hihat
	; position 0 - pattern 0
	db	0,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 1 - pattern 0
	db	4,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 2 - pattern 1
	db	4,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4
	; position 5 - pattern 4
	db	-1,144,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 6 - pattern 5
	db	4,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4
	; position 8 - pattern 7
	db	72,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 9 - pattern 8
	db	4,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 10 - pattern 9
	db	4,2,1,1,4,4,4,4,4,4,4
	; position 11 - pattern 4
	db	32,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 12 - pattern 5
	db	4,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4
	; position 15 - pattern 12
	db	-1,136,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 16 - pattern 13
	db	4,2,1,1,4,4,4,4,4,4,4
	; position 18 - pattern 0
	db	96,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 19 - pattern 15
	db	4,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4
	; position 20 - pattern 16
	db	8,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 21 - pattern 17
	db	4,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 22 - pattern 16
	db	4,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4,4
	; position 23 - pattern 18
	db	4,2,1,1,4,4,4,4,4,4,4,2,1,1,4,4,4,4,4
	; position 24 - pattern 19
	db	8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
	; position 25 - pattern 20
	db	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
	; position 26 - pattern 19
	db	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
	; position 27 - pattern 21
	db	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2

	; 06:  5-Snare / 03|snare1
	; position 2 - pattern 1
	db	-1,136,16,16,8
	; position 5 - pattern 4
	db	-1,152,16,16,8,8
	; position 6 - pattern 5
	db	16,16,16,8,4,4
	; position 9 - pattern 8
	db	-1,144,16,16,8,8
	; position 10 - pattern 9
	db	16,16,8
	; position 13 - pattern 10
	db	-1,160
	; position 15 - pattern 12
	db	-1,136,16,16,8,8
	; position 16 - pattern 13
	db	16,16,17,18
	; position 17 - pattern 14
	db	9,19
	; position 22 - pattern 16
	db	-2,49,16,16,8,8
	; position 23 - pattern 18
	db	16,16,16,8,4,4
	; position 24 - pattern 19
	db	12,8,8,8,8,8,8,8,3
	; position 25 - pattern 20
	db	5,8,8,8,8,8,8,8,3
	; position 26 - pattern 19
	db	5,8,8,8,8,8,8,8,3
	; position 27 - pattern 21
	db	5,8,8,8,4,12,15
	; position 28 - pattern 22
	db	9,2,2,2,2,5,6

	; 07:  6-Bassdrum / 04|basicbassdrum
	; position 2 - pattern 1
	db	-1,128,20,12,6,10
	; position 5 - pattern 4
	db	-1,144,20,12,6,14
	; position 6 - pattern 5
	db	12,20,12,6,10,4,4
	; position 9 - pattern 8
	db	-1,136,20,12,6,14
	; position 10 - pattern 9
	db	12,20,12
	; position 13 - pattern 10
	db	-1,160
	; position 15 - pattern 12
	db	-1,128,20,12,6,14
	; position 16 - pattern 13
	db	12,20,12,6,21
	; position 17 - pattern 14
	db	9,19
	; position 22 - pattern 16
	db	-2,41,20,12,6,14
	; position 23 - pattern 18
	db	12,20,12,6,10,4,4
	; position 24 - pattern 19
	db	8,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
	; position 25 - pattern 20
	db	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
	; position 26 - pattern 19
	db	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
	; position 27 - pattern 21
	db	4,4,4,4,4,4,4,4,4,12,15
	; position 28 - pattern 22
	db	9,19

	; 08:  1-Bass / 00|Clinkster (Basic)
	; position 0 - pattern 0
	db	0,16,16,16
	; position 1 - pattern 0
	db	16,16,16,16
	; position 2 - pattern 1
	db	16,16,16
	; position 3 - pattern 2
	db	32,16,8,8,16,8
	; position 4 - pattern 3
	db	8,16,8,8,16,8
	; position 5 - pattern 4
	db	8,16,8,8,16,8
	; position 6 - pattern 5
	db	8,16,8,8,16,8
	; position 7 - pattern 6
	db	8,16,8,8,16,8
	; position 8 - pattern 7
	db	8,16,8,8,16,8
	; position 9 - pattern 8
	db	8,16,8,8,16,8
	; position 10 - pattern 9
	db	8,16,8,8
	; position 11 - pattern 4
	db	32,16,8,8,16,8
	; position 12 - pattern 5
	db	8,16,8,8,16,8
	; position 13 - pattern 10
	db	8,16,8,8,16,8
	; position 14 - pattern 11
	db	8,16,8,8,16,8
	; position 15 - pattern 12
	db	8,16,8,8,16,8
	; position 16 - pattern 13
	db	8,16,8,8,27
	; position 17 - pattern 14
	db	28
	; position 18 - pattern 0
	db	41,16,16,16
	; position 19 - pattern 15
	db	16,16,16,16
	; position 20 - pattern 16
	db	16,16,8,8,16,8
	; position 21 - pattern 17
	db	8,16,8,8,16,8
	; position 22 - pattern 16
	db	8,16,8,8,16,8
	; position 23 - pattern 18
	db	8,16,8,8,16,8
	; position 24 - pattern 19
	db	8,16,32,8
	; position 25 - pattern 20
	db	8,16,32,8
	; position 26 - pattern 19
	db	8,16,32,8
	; position 27 - pattern 21
	db	8,16,16,27
	; position 28 - pattern 22
	db	28

	; 09:  1-Bass / 00|Clinkster (Basic)
	; position 0 - pattern 0
	db	0,16,16,16
	; position 1 - pattern 0
	db	16,16,16,16
	; position 2 - pattern 1
	db	16,16,16
	; position 3 - pattern 2
	db	32,16,8,8,16,8
	; position 4 - pattern 3
	db	8,16,8,8,16,8
	; position 5 - pattern 4
	db	8,16,8,8,16,8
	; position 6 - pattern 5
	db	8,16,8,8,16,8
	; position 7 - pattern 6
	db	8,16,8,8,16,8
	; position 8 - pattern 7
	db	8,16,8,8,16,8
	; position 9 - pattern 8
	db	8,16,8,8,16,8
	; position 10 - pattern 9
	db	8,16,8,8
	; position 11 - pattern 4
	db	32,16,8,8,16,8
	; position 12 - pattern 5
	db	8,16,8,8,16,8
	; position 13 - pattern 10
	db	8,16,8,8,16,8
	; position 14 - pattern 11
	db	8,16,8,8,16,8
	; position 15 - pattern 12
	db	8,16,8,8,16,8
	; position 16 - pattern 13
	db	8,16,8,8,27
	; position 17 - pattern 14
	db	28
	; position 18 - pattern 0
	db	41,16,16,16
	; position 19 - pattern 15
	db	16,16,16,16
	; position 20 - pattern 16
	db	16,16,8,8,16,8
	; position 21 - pattern 17
	db	8,16,8,8,16,8
	; position 22 - pattern 16
	db	8,16,8,8,16,8
	; position 23 - pattern 18
	db	8,16,8,8,16,8
	; position 24 - pattern 19
	db	10,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
	; position 25 - pattern 20
	db	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
	; position 26 - pattern 19
	db	4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
	; position 27 - pattern 21
	db	4,4,4,4,4,4,4,4,2,27
	; position 28 - pattern 22
	db	28


	section notesamp data align=1

_NoteSamples:
	; 00:  2-Chords / 01|Clinkster (Basic)
	; position 3 - pattern 2
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 4 - pattern 3
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 5 - pattern 4
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 6 - pattern 5
	db	1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 7 - pattern 6
	db	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
	; position 8 - pattern 7
	db	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
	; position 9 - pattern 8
	db	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2
	; position 10 - pattern 9
	db	2,2,2,2,2,2,2,2,2,2,1
	; position 11 - pattern 4
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 12 - pattern 5
	db	1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 13 - pattern 10
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 14 - pattern 11
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 15 - pattern 12
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 16 - pattern 13
	db	1,1,1,1,1,1,1,0,0,0
	; position 17 - pattern 14
	db	0,0,0
	; position 20 - pattern 16
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 21 - pattern 17
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 22 - pattern 16
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 23 - pattern 18
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 24 - pattern 19
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 25 - pattern 20
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 26 - pattern 19
	db	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	; position 27 - pattern 21
	db	1,1,1,1,1,1,1,1,1,1,0,0,0
	; position 28 - pattern 22
	db	0,0,0,0
	db	-1

	; 01:  2-Chords / 01|Clinkster (Basic)
	; position 3 - pattern 2
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3,1
	; position 4 - pattern 3
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3,1
	; position 5 - pattern 4
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3,1
	; position 6 - pattern 5
	db	3,3,3,3,3,1,3,3,3,3,3,1,3
	; position 7 - pattern 6
	db	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,5,5,5
	; position 8 - pattern 7
	db	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
	; position 9 - pattern 8
	db	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,5,5,5
	; position 10 - pattern 9
	db	5,5,5,5,5,5,4,5,5,5,3
	; position 11 - pattern 4
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3,1
	; position 12 - pattern 5
	db	3,3,3,3,3,1,3,3,3,3,3,1,3
	; position 13 - pattern 10
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3,1
	; position 14 - pattern 11
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3
	; position 15 - pattern 12
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3,1
	; position 16 - pattern 13
	db	3,3,3,3,3,1,3,2,3
	; position 17 - pattern 14
	db	2,0
	; position 20 - pattern 16
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3,1
	; position 21 - pattern 17
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3
	; position 22 - pattern 16
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3,1
	; position 23 - pattern 18
	db	3,3,3,3,3,1,3,3,3,3,3,3,1,3
	; position 24 - pattern 19
	db	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
	; position 25 - pattern 20
	db	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
	; position 26 - pattern 19
	db	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3
	; position 27 - pattern 21
	db	3,3,3,3,3,3,3,3,3,3,2,3
	; position 28 - pattern 22
	db	2,0,0
	db	-1

	; 02:  2-Chords / 01|Clinkster (Basic)
	; position 3 - pattern 2
	db	2,2,2,2,3,1,2,3,3,3,3,1,4,2,1
	; position 4 - pattern 3
	db	2,2,2,2,3,1,2,3,3,3,3,1,4,2,1
	; position 5 - pattern 4
	db	2,2,2,2,3,1,2,3,3,3,3,1,4,2,1
	; position 6 - pattern 5
	db	2,2,2,2,3,1,2,3,3,3,3,4,2
	; position 7 - pattern 6
	db	5,5,6,6,6,5,5,6,6,6,5,5,6,6,6,5,5,6,6,6
	; position 8 - pattern 7
	db	5,5,6,6,6,5,5,6,6,6,5,5,6,6,6,5,4,5
	; position 9 - pattern 8
	db	5,5,6,6,6,5,5,6,6,6,5,5,6,6,6,5,5,6,6,6
	; position 10 - pattern 9
	db	5,5,6,6,6,5,5,6,6,6,2
	; position 11 - pattern 4
	db	2,2,2,2,3,1,2,3,3,3,3,1,4,2,1
	; position 12 - pattern 5
	db	2,2,2,2,3,1,2,3,3,3,3,4,2
	; position 13 - pattern 10
	db	2,2,2,2,3,1,2,3,3,3,3,1,4,2,1
	; position 14 - pattern 11
	db	2,2,2,2,3,1,2,3,3,3,3,1,4,2
	; position 15 - pattern 12
	db	2,2,2,2,3,1,2,3,3,3,3,1,4,2,1
	; position 16 - pattern 13
	db	2,2,2,2,3,1,2,3,0,1,1
	; position 17 - pattern 14
	db	2,3,4,2,1,0
	; position 20 - pattern 16
	db	2,2,2,2,3,1,2,3,3,3,3,1,4,2,1
	; position 21 - pattern 17
	db	2,2,2,2,3,1,2,3,3,3,3,3,4,2
	; position 22 - pattern 16
	db	2,2,2,2,3,1,2,3,3,3,3,1,4,2,1
	; position 23 - pattern 18
	db	2,2,2,2,3,1,2,3,3,3,3,3,4,2
	; position 24 - pattern 19
	db	2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,1,1,1,2,2,2
	; position 25 - pattern 20
	db	2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,1,1,1,2,2,2
	; position 26 - pattern 19
	db	2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,1,1,1,2,2,2
	; position 27 - pattern 21
	db	2,2,2,2,2,3,3,3,3,3,3,0,1,1
	; position 28 - pattern 22
	db	2,3,4,2,1,0,4,0
	db	-1

	; 03:  2-Chords / 01|Clinkster (Basic)
	; position 7 - pattern 6
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2
	; position 8 - pattern 7
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2,0,2,0
	; position 9 - pattern 8
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2
	; position 10 - pattern 9
	db	0,2,0,2,1,2,0,2,0,2,1,2,0
	; position 20 - pattern 16
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2
	; position 21 - pattern 17
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2
	; position 22 - pattern 16
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2
	; position 23 - pattern 18
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2
	; position 24 - pattern 19
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2
	; position 25 - pattern 20
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2
	; position 26 - pattern 19
	db	0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2,0,2,0,2,1,2
	; position 27 - pattern 21
	db	0,2,0,2,1,2,0,2,0,2,1,2,1,1,1,1
	; position 28 - pattern 22
	db	1,1
	db	-1

	; 04:  3-Lead / 05|Clinkster (Basic)
	; position 13 - pattern 10
	db	3,5,6,2,4,7,8,12,6,3,8,7,8,7,3,9,6,3,0
	; position 14 - pattern 11
	db	3,5,6,2,3,5,7,9,4,5,6,9,11,6,12,6,10,11,10,6,5,2,1,3
	; position 15 - pattern 12
	db	3,5,6,2,4,7,8,12,6,3,8,7,8,7,3,9,6,3,0
	; position 16 - pattern 13
	db	3,5,6,2,3,5,7,9,4,5,6,9,11,6,12,6,10,11,10,6,5,2,5
	; position 17 - pattern 14
	db	6,1,1,1,1,2
	; position 20 - pattern 16
	db	3,5,6,2,4,7,8,12,6,3,8,7,8,7,3,9,6,3,0
	; position 21 - pattern 17
	db	3,5,6,2,3,5,7,9,4,5,6,9,11,6,12,6,10,11,10,6,5,2,1,3
	; position 22 - pattern 16
	db	3,5,6,2,4,7,8,12,6,3,8,7,8,7,3,9,6,3,0
	; position 23 - pattern 18
	db	3,5,6,2,3,5,7,9,4,5,6,9,11,6,12,6,10,11,10,6,5,2,1,3
	; position 24 - pattern 19
	db	3,5,6,2,4,7,8,12,6,3,8,7,8,7,3,9,6,3,0
	; position 25 - pattern 20
	db	3,5,6,2,3,5,7,9,4,5,6,9,11,6,12,6,10,11,10,6,5,2,1,2,0
	; position 26 - pattern 19
	db	3,5,6,2,4,7,8,12,6,3,8,7,8,7,3,9,6,3,0
	; position 27 - pattern 21
	db	3,5,6,2,3,5,7,9,4,5,6,9,11,6,12,6,10,11,10,6,5,2,5
	; position 28 - pattern 22
	db	6,1,1,1,1,2,2,2,2
	db	-1

	; 05:  4-Hihat / 02|hihat
	; position 0 - pattern 0
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 1 - pattern 0
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 2 - pattern 1
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6
	; position 5 - pattern 4
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 6 - pattern 5
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6
	; position 8 - pattern 7
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 9 - pattern 8
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 10 - pattern 9
	db	6,5,1,3,6,1,6,2,6,4,6
	; position 11 - pattern 4
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 12 - pattern 5
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6
	; position 15 - pattern 12
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 16 - pattern 13
	db	6,5,1,3,6,1,6,2,6,4,6
	; position 18 - pattern 0
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 19 - pattern 15
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6
	; position 20 - pattern 16
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 21 - pattern 17
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 22 - pattern 16
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6,5
	; position 23 - pattern 18
	db	6,5,1,3,6,1,6,2,6,4,6,5,1,3,6,1,6,0,6
	; position 24 - pattern 19
	db	5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6
	; position 25 - pattern 20
	db	5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6
	; position 26 - pattern 19
	db	5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6
	; position 27 - pattern 21
	db	5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,6
	db	-1

	; 06:  5-Snare / 03|snare1
	; position 2 - pattern 1
	db	0,0,0,0
	; position 5 - pattern 4
	db	0,0,0,0,0
	; position 6 - pattern 5
	db	0,0,0,0,0,0
	; position 9 - pattern 8
	db	0,0,0,0,0
	; position 10 - pattern 9
	db	0,0,0
	; position 13 - pattern 10
	db	0
	; position 15 - pattern 12
	db	0,0,0,0,0
	; position 16 - pattern 13
	db	0,0,0,0
	; position 17 - pattern 14
	db	0,0
	; position 22 - pattern 16
	db	0,0,0,0,0
	; position 23 - pattern 18
	db	0,0,0,0,0,0
	; position 24 - pattern 19
	db	0,0,0,0,0,0,0,0,0
	; position 25 - pattern 20
	db	0,0,0,0,0,0,0,0,0
	; position 26 - pattern 19
	db	0,0,0,0,0,0,0,0,0
	; position 27 - pattern 21
	db	0,0,0,0,0,0,0
	; position 28 - pattern 22
	db	0,0,0,0,0,0,0
	db	-1

	; 07:  6-Bassdrum / 04|basicbassdrum
	; position 2 - pattern 1
	db	0,0,0,0,0
	; position 5 - pattern 4
	db	0,0,0,0,0
	; position 6 - pattern 5
	db	0,0,0,0,0,0,0
	; position 9 - pattern 8
	db	0,0,0,0,0
	; position 10 - pattern 9
	db	0,0,0
	; position 13 - pattern 10
	db	0
	; position 15 - pattern 12
	db	0,0,0,0,0
	; position 16 - pattern 13
	db	0,0,0,0,0
	; position 17 - pattern 14
	db	0,0
	; position 22 - pattern 16
	db	0,0,0,0,0
	; position 23 - pattern 18
	db	0,0,0,0,0,0,0
	; position 24 - pattern 19
	db	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	; position 25 - pattern 20
	db	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	; position 26 - pattern 19
	db	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	; position 27 - pattern 21
	db	0,0,0,0,0,0,0,0,0,0,0
	; position 28 - pattern 22
	db	0,0
	db	-1

	; 08:  1-Bass / 00|Clinkster (Basic)
	; position 0 - pattern 0
	db	0,0,0,0
	; position 1 - pattern 0
	db	0,0,0,0
	; position 2 - pattern 1
	db	0,0,0
	; position 3 - pattern 2
	db	3,9,12,6,9,12
	; position 4 - pattern 3
	db	3,9,12,6,9,12
	; position 5 - pattern 4
	db	3,9,12,6,9,12
	; position 6 - pattern 5
	db	3,9,12,6,9,13
	; position 7 - pattern 6
	db	6,7,14,10,12,9
	; position 8 - pattern 7
	db	6,7,14,10,12,7
	; position 9 - pattern 8
	db	6,7,14,10,12,9
	; position 10 - pattern 9
	db	6,7,14,2
	; position 11 - pattern 4
	db	3,9,12,6,9,12
	; position 12 - pattern 5
	db	3,9,12,6,9,13
	; position 13 - pattern 10
	db	3,9,12,6,9,12
	; position 14 - pattern 11
	db	3,9,12,6,9,12
	; position 15 - pattern 12
	db	3,9,12,6,9,12
	; position 16 - pattern 13
	db	3,9,12,5,8
	; position 17 - pattern 14
	db	1
	; position 18 - pattern 0
	db	0,0,0,0
	; position 19 - pattern 15
	db	0,0,0,0
	; position 20 - pattern 16
	db	3,9,12,6,9,12
	; position 21 - pattern 17
	db	3,9,12,6,9,13
	; position 22 - pattern 16
	db	3,9,12,6,9,12
	; position 23 - pattern 18
	db	3,9,12,6,9,13
	; position 24 - pattern 19
	db	11,4,15,12
	; position 25 - pattern 20
	db	11,4,15,12
	; position 26 - pattern 19
	db	11,4,15,12
	; position 27 - pattern 21
	db	11,6,5,8
	; position 28 - pattern 22
	db	1
	db	-1

	; 09:  1-Bass / 00|Clinkster (Basic)
	; position 0 - pattern 0
	db	0,0,0,0
	; position 1 - pattern 0
	db	0,0,0,0
	; position 2 - pattern 1
	db	0,0,0
	; position 3 - pattern 2
	db	3,9,12,5,9,12
	; position 4 - pattern 3
	db	3,9,12,5,9,12
	; position 5 - pattern 4
	db	3,9,12,5,9,12
	; position 6 - pattern 5
	db	3,9,12,5,9,13
	; position 7 - pattern 6
	db	5,6,15,11,12,9
	; position 8 - pattern 7
	db	5,6,15,11,12,6
	; position 9 - pattern 8
	db	5,6,15,11,12,9
	; position 10 - pattern 9
	db	5,6,15,2
	; position 11 - pattern 4
	db	3,9,12,5,9,12
	; position 12 - pattern 5
	db	3,9,12,5,9,13
	; position 13 - pattern 10
	db	3,9,12,5,9,12
	; position 14 - pattern 11
	db	3,9,12,5,9,12
	; position 15 - pattern 12
	db	3,9,12,5,9,12
	; position 16 - pattern 13
	db	3,9,12,4,8
	; position 17 - pattern 14
	db	1
	; position 18 - pattern 0
	db	0,0,0,0
	; position 19 - pattern 15
	db	0,0,0,0
	; position 20 - pattern 16
	db	3,9,12,5,9,12
	; position 21 - pattern 17
	db	3,9,12,5,9,13
	; position 22 - pattern 16
	db	3,9,12,5,9,12
	; position 23 - pattern 18
	db	3,9,12,5,9,13
	; position 24 - pattern 19
	db	14,14,14,14,7,7,7,7,7,7,7,7,10,10,14,14
	; position 25 - pattern 20
	db	14,14,14,14,7,7,7,7,7,7,7,7,10,10,14,14
	; position 26 - pattern 19
	db	14,14,14,14,7,7,7,7,7,7,7,7,10,10,14,14
	; position 27 - pattern 21
	db	14,14,14,14,7,7,7,7,4,8
	; position 28 - pattern 22
	db	1
	db	-1

