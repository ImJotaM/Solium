#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>

static void CreateWindow(char* fname, int width, int height) {

	const char* prefix = "Solium - Content (";
	const char* filename = fname;
	const char* suffix = ")";

	size_t buf = strlen(prefix) + strlen(filename) + strlen(suffix) + 1;

	char* title = (char*)malloc(buf);

	if (title == NULL) {
		printf("Failed to allocate memory: %s", strerror(errno));
		return;
	}

	strcpy(title, prefix);
	strcat(title, filename);
	strcat(title, suffix);

	InitWindow(width, height, title);
	SetTargetFPS(60);

	free(title);
}

static bool FileExists(const char* filepath) {

	FILE* tf = fopen(filepath, "r");
	
	if (tf) {
		fclose(tf);
		return true;
	} else {
		return false;
	}

}

static void LoadMainFont(Font* font, const char* fontpath, int fontsize) {

	if (FileExists(fontpath)) {
		*font = LoadFontEx(fontpath, fontsize, NULL, NULL);
	} else {
		printf("Custom font not found. Using default function.");
		*font = GetFontDefault();
	}

}

int main(int argc, char* argv[]) {

	if (argc != 2) {

		printf("Wrong number of arguments.\n");
		printf("Usage: solium <file>\n");

		return -1;
	}

	int width = 800;
	int height = 600;
	
	CreateWindow(argv[1], width, height);
	
	const char* fontpath = "rsc/Consolas.ttf";
	Font* font = (Font*)malloc(sizeof(struct Font));
	if (font == NULL) {
		printf("Failed to allocate memory: %s", strerror(errno));
		return -1;
	}
	float fontsize = 16.f;

	LoadMainFont(font, fontpath, (int)fontsize);

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

		fcontent = (char*) malloc(fsize);

		if (fcontent != NULL) {
			fread(fcontent, fsize, 1, file);
			fcontent[fsize - 1] = '\0';
		}

	} else {
		
		TraceLog(LOG_INFO, "File '%s' is empty", filepath);
		return 0;
	}

	while (!WindowShouldClose()) {

		BeginDrawing();

		ClearBackground(BLACK);

		DrawTextEx(*font, fcontent, (Vector2){ 0, 0 }, fontsize, 1.f , WHITE);
		
		EndDrawing();

	}

	free(font);
	free(fcontent);
	fclose(file);

	CloseWindow();

	return 0;
}