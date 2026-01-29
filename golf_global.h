SDL_Context sdl_ctx = {};
Draw_Context draw_ctx = {};
App_Input app_input = {};

Color_RGBA color = { 100, 60, 0, 255 };
GUI_Manager gui_manager;

Buffer global_buffer = blank_buffer(20);

global const Color_RGBA golf_green        = HexToRGBA(0x005F19FF);
global const Color_RGBA golf_dark_green   = {  2,  75,   0,  255 };
global const Color_RGBA golf_yellow       = HexToRGBA(0xFFD600FF);
global const Color_RGBA golf_light_yellow = { 240, 229, 118, 255 };
global const Color_RGBA golf_dark_yellow  = { 197, 180,  22, 255 };

Draw_Style button_style = {
	.background_color = golf_yellow,
	.background_color_hover = golf_light_yellow,
	.background_color_pressed = golf_dark_yellow,
};

