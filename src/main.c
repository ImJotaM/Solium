#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#define RETURN_RESULT(ERROR_TYPE) { result = ERROR_TYPE; return result; }

#define ARG_HELP  1950366504
#define ARG_PAD    189261108
#define ARG_COL    189247421

enum ERROR_TYPE {
	NONE = 0,
	OK,
	END_EARLY,
	ERROR,
};

static const char* validArgs[] = {
	"--help",
	"--pad",
	"--col",
};

static const char* formated_validArgs[] = {
	"--help,",
	"--pad ,",
	"--col ,",
};

static const char* argUsage[] = {
	"--help           ",
	"--pad  <integer>  ",
	"--col  <hex-color>",
};

static const char* argDescription[] = {
	"",
	"",
	"",
};

const unsigned long Hash(const char* str) {

	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

typedef struct {

	char* filepath;
	int padding;
	unsigned long paddingColor;

} ArgumentValues;

int ParseArgs(ArgumentValues* values, int argc, char* argv[]) {

	int result = NONE;

	if (argc == 1) {

		printf("Solium 1.0.0\n");
		printf("Use \"solium --help\" to get a list of valid commands.\n\n");
		
		RETURN_RESULT(END_EARLY);
	}

	bool UseAsFilepath = true;

	for (size_t i = 1; i < argc; ++i) {

		switch (Hash(argv[i])) {

		case ARG_HELP:

			printf("Usage: solium <file-path> [<args>]\n\n");

			for (size_t i = 0; i < sizeof(validArgs) / sizeof(validArgs[0]); ++i) {
				printf(" %s  %s %s\n", formated_validArgs[i], argUsage[i], argDescription[i]);
			}
			printf("\n\n");

			RETURN_RESULT(END_EARLY);

			break;

		case ARG_PAD:

			if (i + 1 < argc) {
				values->padding = atoi(argv[i + 1]);
				++i;
			} else {
				printf("\"--pad\" argument require an int value.\n");
				RETURN_RESULT(ERROR);
			}

			break;

		case ARG_COL:
			
			if (i + 1 < argc) {
				values->paddingColor = strtoul(argv[i + 1], NULL, 0);
				++i;
			} else {
				printf("\"--col\" argument require an hex-color value.\n");
				RETURN_RESULT(ERROR);
			}

			break;

		default:

			if (UseAsFilepath) {
				values->filepath = argv[i];
				UseAsFilepath = false;
			} else {
				printf("\"%s\" is not a valid argument.", argv[i]);
				RETURN_RESULT(ERROR);
			}
			
			break;
			
		}

	}

	RETURN_RESULT(OK);
}

int main(int argc, char* argv[]) {

	SetTraceLogLevel(LOG_NONE);

	ArgumentValues values = { "", 0, 0 };

	{
		int opresult = NONE;
		opresult = ParseArgs(&values, argc, argv);
		if (opresult == ERROR || opresult == END_EARLY) return ParseArgs;
	}

	const char* filename = GetFileName(values.filepath);
	Image image = LoadImage(values.filepath);
	int width = image.width + values.padding;
	int height = image.height + values.padding;

	InitWindow(width, height, filename);
	SetTargetFPS(60);

	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);

	while (!WindowShouldClose()) {

		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
			
			TakeScreenshot("out.png");

			SetTraceLogLevel(LOG_INFO);
			TraceLog(LOG_INFO, "Printed!");
			SetTraceLogLevel(LOG_NONE);

		}

		BeginDrawing();

		ClearBackground(GetColor(values.paddingColor));

		DrawTexture(texture, values.padding / 2, values.padding / 2, WHITE);
		
		EndDrawing();

	}

	UnloadTexture(texture);

	CloseWindow();

	return 0;
}