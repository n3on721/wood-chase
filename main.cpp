#include "raylib.h"

#define screenWidth 1280
#define screenHeight 720

typedef enum GameScreen { CENTRE, LEFT, RIGHT, MENU, DIALOGUE, WINDOW, CAVE, HOUSE, BEDROOM, DEAD, TRYAGAIN, ENDING1, ENDING2, WIN } gameScreen;
gameScreen currentScreen = MENU;

Vector2 MousePOS;

Texture2D forest, forest2, forest3, grave1, grave2, house, window1, window2, ghost, ghost2, cave, cave2, InsideHouse, Bedroom, LeftArrow, RightArrow, DownArrow, RScene, CScene, LScene, WScene, InsideCave, Temp1, Temp2;
Texture2D DresserScene, houseCracked, BedTook, DeadScene, EscapeScene;
Texture2D end1, end2, end3;

bool scene1 = true, scene2 = false, scene3 = false, scene4 = false;
int screamTimer = 0;

bool right1 = true, right2 = false, right3 = false;
Sound BPressed, Scream, GlassBreak;

bool exitWindowRequested = false;
bool exitWindow = false;

const char dialog[] = "\tYou wake up in a wild forest with no idea how you got here.\n\t\t\t\t\t\tYou need to get out of here... But how?";

Rectangle ArrowLeft = { 0, 290, 160, 160 };
Rectangle ArrowRight = { 1120, 290, 160, 160 };
Rectangle ArrowDown = { screenWidth / 2 - 50, screenHeight - 120, 100, 100 };

Rectangle Chest = { 1080, 450, 200, 140 };
Rectangle Grave = { 560,  350, 180, 90 };
Rectangle Window = { 1080, 320, 100, 80 };
Rectangle Plate = { 580, 530, 160, 80 };
Rectangle CaveEnterance = { 140, 300, 350, 300 };
Rectangle DroppedKey = { 580, 400, 120, 60 };
Rectangle Door = { 910, 330, 70, 160 };
Rectangle BedroomDoor = { 0, 50, 180, 600 };
Rectangle TinCan = { 560, 300, 50, 90 };

Rectangle Hide = { 0, 0, 320, 720 };
Rectangle Run = { 960, 0 ,320, 720 };
Color ChoisesColor = { 76, 63, 47, 128 };
Rectangle TimerBar = { 340, 90, 602, 50 };

Rectangle Shovel = { 25, 25, 100, 100 }, Bone = { 150,  25, 100, 100 }, Key = { 275, 25, 100, 100 }, Tin = { 25, 25, 100, 100 };
bool HasShovel = false, HasBone = false, HasKey = false, HasTin = false;
Texture2D ItemShovel, ItemBone, ItemKey, ItemTin;

//			MENU
Rectangle Start = { screenWidth / 2 - 150, 250, 300, 150 };
Color StartColor = RED, QuitColor = RED;
Rectangle Quit = { screenWidth / 2 - 150, 450, 300, 150 };
//			MENU

//				MESSAGE BOOLS
bool GraveMes = false;
bool CaveEnterMes = false;
bool PlateMes = false;
bool ChestMes = false;
bool DoorMes = false;
//				MESSAGE BOOLS

//temporary variables
int framesCounter = 0;
bool WasDug = false;
bool Jumpscare = false;
bool DoOnce = true;
bool CanEnter = false;
bool WindowMes = false;
int ChooseTime = 0;
int FrameTimer = 0;
Color Fading = { 255, 255, 255, 255 };
//temporary variables

bool IsButtonChecked(Rectangle button) {
	return CheckCollisionPointRec(GetMousePosition(), button);
}

bool ButtonPressed(Rectangle button) {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsButtonChecked(button)) {
		return true;
	}
	else
		return false;
}

void UpdateGame() {

	switch (currentScreen)
	{
	case MENU:
		if (IsButtonChecked(Start))
			StartColor = BROWN;
		else
			StartColor = BEIGE;

		if (IsButtonChecked(Quit))
			QuitColor = BROWN;
		else
			QuitColor = BEIGE;

		if (ButtonPressed(Start)) {
			currentScreen = DIALOGUE;
			PlaySound(BPressed);
		}
		if (ButtonPressed(Quit)) {
			exitWindowRequested = true;
			PlaySound(BPressed);
		}
		break;
	case DIALOGUE:
		framesCounter += 2;
		if (IsKeyPressed(KEY_ENTER)) {
			currentScreen = CENTRE;
			framesCounter = 0;
		}
		break;
	case CENTRE:
		if (ButtonPressed(ArrowLeft)) currentScreen = LEFT;
		if (ButtonPressed(ArrowRight)) currentScreen = RIGHT;

		if (!WasDug) {
			if (CheckCollisionPointRec(GetMousePosition(), Grave)) {
				GraveMes = true;
				MousePOS = GetMousePosition();
			}
			else GraveMes = false;
		}
		if (ButtonPressed(Grave) && WasDug) {
			CScene = grave2;
			HasBone = true;
			GraveMes = false;
		}

		if (HasShovel && ButtonPressed(Grave) && !WasDug) {
			CScene = grave1;
			WasDug = true;
			GraveMes = false;
		}

		break;
	case LEFT:
		if (ButtonPressed(ArrowRight)) currentScreen = CENTRE;
		if (CanEnter && ButtonPressed(CaveEnterance))
			currentScreen = CAVE;

		if (!CanEnter) {
			if (CheckCollisionPointRec(GetMousePosition(), CaveEnterance)) {
				CaveEnterMes = true;
				MousePOS = GetMousePosition();
			}
			else CaveEnterMes = false;
		}

		if (!HasBone) {
			if (CheckCollisionPointRec(GetMousePosition(), Plate)) {
				PlateMes = true;
				MousePOS = GetMousePosition();
			}
			else PlateMes = false;
		}

		if (HasBone && ButtonPressed(Plate)) {
			LScene = cave2;
			CanEnter = true;
			HasBone = false;
			PlateMes = false;
			CaveEnterMes = false;
		}
		break;
	case CAVE:
		if (ButtonPressed(ArrowDown))
			currentScreen = LEFT;
		if (ButtonPressed(DroppedKey)) {
			InsideCave = Temp2;
			HasKey = true;
		}

		break;
	case RIGHT:
		if (ButtonPressed(ArrowLeft)) currentScreen = CENTRE;

		if (right1) {
			if (CheckCollisionPointRec(GetMousePosition(), Chest)) {
				ChestMes = true;
				MousePOS = GetMousePosition();
			}
			else ChestMes = false;
		}

		if (CheckCollisionPointRec(GetMousePosition(), Window)) {
			WindowMes = true;
			MousePOS = GetMousePosition();
		}
		else WindowMes = false;

		if (!HasKey) {
			if (CheckCollisionPointRec(GetMousePosition(), Door)) {
				DoorMes = true;
				MousePOS = GetMousePosition();
			}
			else DoorMes = false;
		}

		if (ButtonPressed(Door) && HasKey) {
			currentScreen = HOUSE;
			HasShovel = false;
			HasBone = false;
			HasKey = false;
		}

		if (ButtonPressed(Window) && !Jumpscare) {
			currentScreen = WINDOW;
			if(DoOnce)
				Jumpscare = true;
		}

		if (ButtonPressed(Chest) && right2) {
			RScene = forest3;
			HasShovel = true;
			right2 = false;
			right3 = true;
		}
		if (ButtonPressed(Chest) && right1) {
			RScene = forest2;
			right1 = false;
			ChestMes = false;
			right2 = true;
		}

		break;
	case WINDOW:
		if (framesCounter == 120) {
			Jumpscare = false;
			WScene = window2;
		}
		if (ButtonPressed(ArrowDown))
			currentScreen = RIGHT;
		break;
	case HOUSE:
		if (!HasTin) {
			if (ButtonPressed(BedroomDoor)) {
				currentScreen = BEDROOM;
				DoOnce = true;
			}
		}
		else {
			if (ChooseTime == 600) {
				ChooseTime++;
				currentScreen = DEAD;
				DoOnce = true;
				HasTin = false;
			}
			if (ButtonPressed(Hide)) {
				currentScreen = ENDING1;
				DoOnce = true;
			}
			if (ButtonPressed(Run)) {
				currentScreen = ENDING2;
				DoOnce = true;
			}
		}
		if (HasTin) {
			ChooseTime += 2;
		}
		break;
	case BEDROOM:
		if (ButtonPressed(ArrowDown))
			currentScreen = HOUSE;

		if (ButtonPressed(TinCan) && !HasTin) {
			Bedroom = BedTook;
			HasTin = true;
			if (DoOnce) {
				PlaySound(GlassBreak);
				DoOnce = false;
			}
		}

		break;
	case DEAD:
		if (screamTimer == 125) {
			currentScreen = TRYAGAIN;
		}
		screamTimer++;
		break;
	case TRYAGAIN:
		if (IsButtonChecked(Quit))
			QuitColor = BROWN;
		else
			QuitColor = BEIGE;

		if (ButtonPressed(Quit)) {
			exitWindowRequested = true;
			PlaySound(BPressed);
		}
		break;
	case ENDING1:
		HasTin = false;
		FrameTimer++;
		if (scene1 && FrameTimer == 60) {
			scene2 = true;
			scene1 = false;
			DresserScene = end1;
			FrameTimer = 0;
		}
		if (scene2 && FrameTimer == 60) {
			scene3 = true;
			scene2 = false;
			DresserScene = end2;
			FrameTimer = 0;
		}
		if (scene3 && FrameTimer == 60) {
			scene4 = true;
			scene3 = false;
			DresserScene = end3;
			FrameTimer = 0;
			if (DoOnce) {
				PlaySound(Scream);
				DoOnce = false;
			}
		}
		if (scene4 && FrameTimer == 140) {
			scene4 = false;
			scene1 = true;
			currentScreen = TRYAGAIN;
		}
		break;
	case ENDING2:
		FrameTimer++;
		if (scene1 && FrameTimer == 60) {
			scene2 = true;
			scene1 = false;
			HasTin = false;
			EscapeScene = houseCracked;
			FrameTimer = 0;
			if (DoOnce) {
				PlaySound(GlassBreak);
				DoOnce = false;
			}
		}
		if (scene2 && FrameTimer == 60) {
			scene2 = false;
			currentScreen = WIN;
		}

		break;
	case WIN:
		if (Fading.a == 0)
			Fading.a = 0;
		else
			Fading.a--;
		if (IsButtonChecked(Quit))
			QuitColor = BROWN;
		else
			QuitColor = BEIGE;

		if (ButtonPressed(Quit)) {
			exitWindowRequested = true;
			PlaySound(BPressed);
		}
		break;
	}

	if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) exitWindowRequested = true;

	if (exitWindowRequested)
	{
		if (IsKeyPressed(KEY_Y)) exitWindow = true;
		else if (IsKeyPressed(KEY_N)) exitWindowRequested = false;
	}

}

void DrawGame() {
	BeginDrawing();
	
	switch (currentScreen)
	{
	case MENU:
		ClearBackground(RAYWHITE);
		DrawText("WOOD-CHASE", 340, 80, 90, BLACK);
		DrawRectangleRec(Start, StartColor);
		DrawRectangleLinesEx(Start, 6.5, DARKGRAY);
		DrawText("Start", Start.x + 80, Start.y + 50, 50, BLACK);
		DrawRectangleRec(Quit, QuitColor);
		DrawRectangleLinesEx(Quit, 6.5, DARKGRAY);
		DrawText("Quit", Quit.x + 100, Quit.y + 50, 50, BLACK);
		DrawText("Made by Savu Matei & Rusu Teodora", 10, 10, 25, LIGHTGRAY);
		break;
	case DIALOGUE:
		ClearBackground(BEIGE);
		DrawText(TextSubtext(dialog, 0, framesCounter / 10), 60, 300, 35, DARKGRAY);
		DrawText("Press [ENTER] to Continue!", 10, screenHeight - 40, 20, LIGHTGRAY);
		break;
	case CENTRE:
		DrawTexture(CScene, 0, 0, WHITE);
		DrawTexture(LeftArrow, 0, 290, WHITE);
		DrawTexture(RightArrow, 1170, 290, WHITE);

		if (GraveMes) {
			DrawText("I need something to dig..", MousePOS.x + 10, MousePOS.y - 10, 18, ORANGE);
		}

		break;
	case LEFT:
		DrawTexture(LScene, 0, 0, WHITE);
		DrawTexture(RightArrow, 1170, 290, WHITE);
		if (CaveEnterMes) {
			DrawText("I can't enter right now..", MousePOS.x + 10, MousePOS.y - 10, 18, ORANGE);
		}
		if (PlateMes) {
			DrawText("Plate.. for food?", MousePOS.x + 10, MousePOS.y - 10, 18, ORANGE);
		}
		break;
	case CAVE:
		DrawTexture(InsideCave, 0, 0, WHITE);
		DrawTexture(DownArrow, ArrowDown.x, ArrowDown.y, WHITE);
		break;
	case RIGHT:
		DrawTexture(RScene, 0, 0, WHITE);
		DrawTexture(LeftArrow, 0, 290, WHITE);
		if (ChestMes) {
			DrawText("A coffin", MousePOS.x + 10, MousePOS.y - 10, 18, ORANGE);
		}
		if (WindowMes) {
			DrawText("Check window?", MousePOS.x + 10, MousePOS.y - 10, 18, ORANGE);
		}
		if (DoorMes) {
			DrawText("I need a key..", MousePOS.x + 10, MousePOS.y - 10, 18, ORANGE);
		}
		break;
	case WINDOW:
		DrawTexture(WScene, 0, 0, WHITE);
		if (DoOnce) {
			PlaySound(Scream);
			DoOnce = false;
		}
		if (Jumpscare) {
			DrawTexture(ghost, 0, 0, WHITE);
			framesCounter++;
		}
		DrawTexture(DownArrow, ArrowDown.x, ArrowDown.y, WHITE);
		break;
	case HOUSE:
		DrawTexture(InsideHouse, 0, 0, WHITE);
		if (HasTin) {
			DrawRectangleRec(Hide, ChoisesColor);
			DrawText("HIDE", 65, 338, 75, BLACK);
			DrawRectangleRec(Run, ChoisesColor);
			DrawText("RUN", 1050, 338, 75, BLACK);
			DrawRectangle(TimerBar.x, TimerBar.y, ChooseTime, TimerBar.height, WHITE);
			DrawRectangleLinesEx(TimerBar, 3.5, LIGHTGRAY);
			DrawText("CHOOSE!", 500, 20, 70, WHITE);
			
		}
		break;
	case BEDROOM:
		DrawTexture(Bedroom, 0, 0, WHITE);
		DrawTexture(DownArrow, ArrowDown.x, ArrowDown.y, WHITE);
		break;
	case DEAD:
		DrawTexture(DeadScene, 0, 0, WHITE);
		DrawTexture(ghost2, 0, 0, WHITE);
		if (DoOnce) {
			PlaySound(Scream);
			DoOnce = false;
		}
		break;
	case TRYAGAIN:
		ClearBackground(MAROON);
		DrawText("YOU DIED!", 410, 70, 90, BLACK);
		DrawRectangleRec(Quit, QuitColor);
		DrawRectangleLinesEx(Quit, 6.5, DARKGRAY);
		DrawText("Quit", Quit.x + 100, Quit.y + 50, 50, BLACK);
		break;
	case ENDING1:
		DrawTexture(DresserScene, 0, 0, WHITE);
		break;
	case ENDING2:
		DrawTexture(EscapeScene, 0, 0, WHITE);
		break;
	case WIN:
		ClearBackground(SKYBLUE);
		DrawText("YOU SURVIVED!", 330, 70, 90, BLACK);
		DrawRectangleRec(Quit, QuitColor);
		DrawRectangleLinesEx(Quit, 6.5, DARKGRAY);
		DrawText("Quit", Quit.x + 100, Quit.y + 50, 50, BLACK);
		DrawTexture(EscapeScene, 0, 0, Fading);
		break;
	}

	if (currentScreen != MENU && currentScreen != DIALOGUE) {
		if (HasShovel)
			DrawTexture(ItemShovel, Shovel.x, Shovel.y, WHITE);
		if (HasBone)
			DrawTexture(ItemBone, Bone.x, Bone.y, WHITE);
		if (HasKey)
			DrawTexture(ItemKey, Key.x, Key.y, WHITE);
		if (HasTin)
			DrawTexture(ItemTin, Tin.x, Tin.y, WHITE);
	}

	if (exitWindowRequested)
	{
		DrawRectangle(0, screenHeight / 2 - 150, screenWidth, 300, BLACK);
		DrawText("Are you sure that you want to exit? [Y/N]", 160, screenHeight / 2 - 20, 40, WHITE);
	}

	EndDrawing();
}

void DrawUpdateFrame() {
	UpdateGame();
	DrawGame();
}

int main(void) {

	InitWindow(screenWidth, screenHeight, "WOOD-CHASE");
	InitAudioDevice();
	SetExitKey(KEY_NULL);
	SetTargetFPS(60);

	forest = LoadTexture("resources/CENTRE.png");
	forest2 = LoadTexture("resources/forest2.png");
	forest3 = LoadTexture("resources/forest3.png");
	house = LoadTexture("resources/RIGHT.png");
	window1 = LoadTexture("resources/Window1.png");
	window2 = LoadTexture("resources/Window2.png");
	ghost = LoadTexture("resources/GHOST.png");
	cave = LoadTexture("resources/LEFT.png");
	cave2 = LoadTexture("resources/LEFT2.png");
	LeftArrow = LoadTexture("resources/LeftArrow.png");
	RightArrow = LoadTexture("resources/RightArrow.png");
	DownArrow = LoadTexture("resources/DownArrow.png");
	grave1 = LoadTexture("resources/CENTRE2.png");
	grave2 = LoadTexture("resources/CENTRE3.png");
	Temp1 = LoadTexture("resources/CAVE1.png");
	Temp2 = LoadTexture("resources/CAVE2.png");
	InsideHouse = LoadTexture("resources/House.png");
	Bedroom = LoadTexture("resources/BedRoom.png");
	houseCracked = LoadTexture("resources/House2.png");
	BedTook = LoadTexture("resources/BedRoom2.png");
	ghost2 = LoadTexture("resources/GHOST2.png");
	end1 = LoadTexture("resources/end1.png");
	end2 = LoadTexture("resources/end2.png");
	end3 = LoadTexture("resources/end3.png");

	ItemShovel = LoadTexture("resources/ItemShovel.png");
	ItemBone = LoadTexture("resources/ItemBone.png");
	ItemKey = LoadTexture("resources/ItemKey.png");
	ItemTin = LoadTexture("resources/ItemTin.png");

	BPressed = LoadSound("resources/BPressed.mp3");
	Scream = LoadSound("resources/Scream.mp3");
	GlassBreak = LoadSound("resources/Glass_Break.mp3");
	SetSoundVolume(Scream, 1.0);

	RScene = house;
	CScene = forest;
	WScene = window1;
	LScene = cave;
	InsideCave = Temp1;
	DeadScene = InsideHouse;
	DresserScene = Bedroom;
	EscapeScene = InsideHouse;

	while (!exitWindow) {

		DrawUpdateFrame();

	}
}