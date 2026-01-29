enum GUI_States {
	GUI_DEFAULT,
	GUI_HOVER,
	GUI_PRESSED,
	GUI_ACTIVE
};

enum GUI_Align {
	ALIGN_CENTER,
	ALIGN_LEFT,
	ALIGN_RIGHT,
};

struct Draw_Style {
	union {
		struct {
			Color_RGBA background_color;
			Color_RGBA background_color_hover;
			Color_RGBA background_color_pressed;
			Color_RGBA background_color_active; // mostly for textboxs right now
		};
		Color_RGBA background_colors[4];
	};

	union {
		struct {
			Color_RGBA text_color;
			Color_RGBA text_color_hover;
			Color_RGBA text_color_pressed;
			Color_RGBA text_color_active;
		};
		Color_RGBA text_colors[4];
	};
};

struct Draw_Textbox {
	Draw_Style style;
	u32 state;

	Color_RGBA cursor_color;
	u32 cursor_position;
	float32 cursor_width;
	float32 text_shift; // changes depending on cursor position

	Vector2 coords;
	Vector2 dim;

	Font *font;
	const char *text;
	u32 text_align; // inside textbox

	const char *label;
	Color_RGBA label_color;
};

#define TEXTBOX_SIZE 20

struct Textbox {
	u32 index; // what GUI index is active
	void *value;
	u32 value_type;
	u32 cursor_position;
	float32 shift;
	char text[TEXTBOX_SIZE];

};

struct UI_Widget {
	Vector2 coords; // location
	Vector2 percent_dim; // % of above in hierarchy
	Vector2 dim; // px
	//Vector2_s32 segments; // splits of dim

	bool8 visible;
	bool8 interactable;

	UI_Widget *root[3]; // start a new graph for a menu in this box
	u32 root_count;
	UI_Widget *children[3]; // continue graph for current menu (add rows)
	u32 children_count;
};

struct GUI {
	u32 index;
	u32 hover;
	u32 pressed;
	u32 active;

	Textbox edit;
	Draw_Style style; // for components

	Vector4 background_color; // clears the windows to this color
	Vector4 back_color; // color of just the part of the screen covered by gui with dim

	UI_Widget base; // defines a widget that fills the whole screen

	Vector2 dim; // %
	Vector2_s32 segments; // splits of dim

	Vector2 shift; // %
	Vector2 coords; // px
	Vector2 segment_dim; // px

	Vector2 padding; // %
	Vector2 padding_px;

	Vector2 backdrop;
	Vector2 backdrop_px;

	s32 (*draw)(GUI *gui);

	void start() {
		index = 1;
	}

	void end() {
		if (hover > index - 1)
			hover = index - 1;
	}

	void close() {
		hover = 0;
		pressed = 0;
		active = 0;
		edit.index = 0;
	}
};

enum GUI_Ids {
	GUI_MAIN_MENU,
	GUI_TEST,
	GUI_PAUSE,
	GUI_DEBUG,

	GUI_COUNT
};

struct GUI_Manager {
	GUI guis[GUI_COUNT];

	Stack<u32> indices = Stack<u32>(10); // index at the top is the gui that is currently rendered
};


struct UI_Box {
	UI_Box *parent; // if this is zero it is the top gui
	Vector2 coords_percent;
	Vector2 dim_percent;

	Vector2 coords;
	Vector2 dim;
	
	u32 index;
	u32 hover;
	u32 pressed;
	u32 active;
};