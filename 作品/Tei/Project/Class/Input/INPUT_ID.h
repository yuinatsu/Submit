#pragma once

enum class INPUT_ID
{
	UP = 1,
	DOWN = 2,
	LEFT = 4,
	RIGHT = 8,
	SWT1 = 16,
	SWT2 = 32,
	SWT3 = 64,
	MAX = 128
};

INPUT_ID begin(INPUT_ID);
INPUT_ID end(INPUT_ID);
INPUT_ID operator++(INPUT_ID& key);
INPUT_ID operator*(INPUT_ID key);


enum class TRG
{
	NOW,
	OLD,
	MAX
};

enum class CMD_ID
{
	UP = 1,
	DOWN = 2,
	LEFT = 4,
	RIGHT = 8,
	LEFT_U_D = UP + LEFT,
	LEFT_D_D = DOWN + LEFT,
	RIGHT_U_D = UP + RIGHT,
	RIGHT_D_D = DOWN + RIGHT,
	SWT1 = 16,
	SWT2 = 32,
	SWT3 = 64,
	MAX,
	NEUTRAL = 0,
};

enum class InputType
{
	KEYBOARD,
	PAD,
};
