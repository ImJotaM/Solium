#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>

#define MAX_FILE_BUFFER 1024

int main() {

	int width = 800;
	int height = 600;
	const char* title = "Solium";

	InitWindow(width, height, title);

	FILE* file = NULL;
	const char* filepath = "File.txt";
	file = fopen(filepath, "r");

	if (!file) {
		TraceLog(LOG_ERROR, "Can't open file '%s': %s", filepath, strerror(errno));
		return -1;
	}

	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* fcontent = NULL;

	if (fsize > 0) {

		fcontent = (char*) malloc(fsize + 1);

		if (fcontent != NULL) {
			fread(fcontent, fsize, 1, file);
			fcontent[fsize] = '\0';
		}

	} else {
		TraceLog(LOG_INFO, "File '%s' is empty", filepath);
		return 0;
	}

	while (!WindowShouldClose()) {

		BeginDrawing();

		ClearBackground(BLACK);

		DrawText(fcontent, 0, 0, 16, WHITE);
		
		EndDrawing();

	}

	free(fcontent);
	fclose(file);

	CloseWindow();

	return 0;
}