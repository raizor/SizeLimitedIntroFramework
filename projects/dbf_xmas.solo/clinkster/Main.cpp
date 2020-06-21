#include "clinkster.h"

#include <stdio.h>
#include <Windows.h>

void printTime(float ticks) {
	int sec = (int)(ticks / Clinkster_TicksPerSecond);
	printf("%d:%02d", sec / 60, sec % 60);
}

int main(int argc, char **argv)
{
	printf("Music composed using 4k synth \"Clinkster\" by Blueberry / Loonies\n\n");

	printf("Generating...\n");
	Clinkster_GenerateMusic();

	printf("Saving...\n");
	FILE *outfile = fopen("music.wav", "wb");
	fwrite(Clinkster_WavFileHeader, 1, sizeof(Clinkster_WavFileHeader), outfile);
	fwrite(Clinkster_MusicBuffer, 1, Clinkster_WavFileHeader[10], outfile);
	fclose(outfile);

	printf("Playing...\n\n");
	Clinkster_StartMusic();
	while (!GetAsyncKeyState(VK_ESCAPE)) {
		float pos = Clinkster_GetPosition();
		if (pos > Clinkster_MusicLength) break;
		printf("\r ");
		printTime(pos);
		printf(" / ");
		printTime((float)Clinkster_MusicLength);

		printf("   ");
		for (int i = 0 ; i < Clinkster_NumTracks ; i++) {
			float t = Clinkster_GetInstrumentTrigger(i, pos);
			int ti = (int)(t * 0.25);
			if (ti > 3) ti = 3;
			printf("%c",  "|:. "[ti]);
		}

		Sleep(100);
	}
	exit(0);
	return 0;
}
