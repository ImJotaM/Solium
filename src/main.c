#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <raylib.h>

int main(int argc, char* argv[]) {

	if (argc < 2 || argc > 3) {

		printf("Wrong number of arguments.\n");
		printf("Usage: solium <image-path>\n");

		return -1;
	}
	
	const char* fpath = argv[1];
	const char* filename = GetFileName(fpath);

	int padding = 0;

	if (argc == 3) {
		padding = atoi(argv[2]);
	}

	Image image = LoadImage(fpath);

	int width = image.width + padding;
	int height = image.height + padding;

	InitWindow(width, height, filename);
	SetTargetFPS(60);

	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);

	while (!WindowShouldClose()) {

		BeginDrawing();

		ClearBackground(RED);

		DrawTexture(texture, padding / 2, padding / 2, WHITE);
		
		EndDrawing();

	}

	UnloadTexture(texture);

	CloseWindow();

	return 0;
}