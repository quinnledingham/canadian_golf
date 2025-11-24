internal s32
draw_main_menu(GUI *gui) {

	gui->start();
	
	Rect base;
	base.coords = {0, 0};
	base.dim = cv2(sdl_ctx.window_dim);
	
	Rect menu_box = get_centered_rect(base, 0.05f);
	Rect player_box = {};
	player_box.coords = menu_box.coords;
	player_box.dim = menu_box.dim;
	player_box.dim.x *= (2.0f/3.0f);

	Rect title_box = {};
	title_box.coords = menu_box.coords;
	title_box.coords.x += player_box.dim.x;
	title_box.dim = menu_box.dim;
	title_box.dim.x *= (1.0f/3.0f);

	Vector2 wdim = cv2(sdl_ctx.window_dim);
	float32 s_side = wdim.height;
	if (wdim.width < wdim.height)
		s_side = wdim.width;

	float32 window_radius = s_side * 0.01f;

	draw_ctx.font_id = FONT_CHELA;

	draw_rect({0, 0}, cv2(sdl_ctx.window_dim), golf_green);

	draw_rounded_rect(player_box.coords, player_box.dim, window_radius, golf_dark_green);
	draw_rounded_rect(title_box.coords, title_box.dim, window_radius, golf_dark_green);

	float32 title_pxh = title_box.dim.y * 0.1f;

	/*
	const char *title_p1 = "Canadian";
	const char *title_p2 = "Golf";
	String_Draw_Info p1_info = get_string_draw_info(title_p1, title_pxh);
	Vector2 title_coords = text_coords(title_box, p1_info, ALIGN_CENTER);
	draw_text("Canadian", title_coords, title_pxh, golf_yellow);
	*/

	gui_button(gui, "Play");

	draw_text("CUM", { 100, 100 }, 52.0f, golf_yellow);
	draw_text("CUM", { 200, 100 }, 32.0f, golf_yellow);
	draw_text("CUM", { 300, 100 }, 22.0f, golf_yellow);
	draw_text("CUM", { 400, 100 }, 12.0f, golf_yellow);

	gui->end();

	return 0;
}

internal void 
draw_fps() {
	u32 ret = SDL_snprintf((char *)global_buffer.memory, global_buffer.size, "%.0f", (float32)sdl_ctx.frames_per_s);
	draw_ctx.font_id = FONT_ROBOTO_MONO;
	draw_text(global_buffer.str(), {0, 0}, 20, {50, 255, 50, 255});
}

internal u32
golf_init() {
	gui_manager.indices.push(GUI_MAIN_MENU);
	gui_manager.guis[GUI_MAIN_MENU].draw = draw_main_menu;

	return 0;
}

internal u32
golf_update() {
	if (on_down(app_input.mouse.left)) {
		SDL_Log("Mouse Coords: (%f, %f)\n", app_input.mouse.coords.x, app_input.mouse.coords.y);
	}

	SDL_SetRenderDrawColor(sdl_ctx.renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl_ctx.renderer);

	draw_top_gui();
	draw_fps();

	SDL_RenderPresent(sdl_ctx.renderer);

	return 0;
}