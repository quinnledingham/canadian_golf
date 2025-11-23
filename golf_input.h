//
// Button
//

struct Button {
	s32 ids[3];
	u32 num_of_ids;

	bool8 current_state;
	bool8 previous_state;
};

//
// Setup for game
//

enum Input_IDs {
	// game
	IN_CARD_0,
	IN_CARD_1,
	IN_CARD_2,
	IN_CARD_3,

	IN_CARD_4,
	IN_CARD_5,
	IN_CARD_6,
	IN_CARD_7,

	IN_DRAW_PILE,
	IN_DISCARD_PILE,
	IN_PASS,

	IN_GAME_SIZE, // just as a test to see if the enum matches Game_Input

	// menus
	IN_BACK,

	#ifdef DEBUG

	// camera
	IN_FORWARD,
	IN_BACKWARD,
	IN_LEFT,
	IN_RIGHT,
	IN_UP,
	IN_DOWN,

	IN_REFRESH_SHADERS,
	IN_TOGGLE_CAMERA,
	IN_RESET_GAME,
	IN_TOGGLE_WIREFRAME,

	#endif // DEBUG

	IN_COUNT
};

//
// App
//

struct Mouse_Input {
	Vector2 coords;
	Vector2 relative_coords;
	Button left;
};

struct App_Input {
	Mouse_Input mouse;

	u32 last_input_type;

	Button buttons[IN_COUNT];
};

enum App_Input_Types {
	IN_KEYBOARD,
	IN_MOUSE,
	IN_CONTROLLER,

	INPUT_TYPE_COUNT
};