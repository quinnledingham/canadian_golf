internal s32
draw_main_menu(GUI *gui) {
	Vector2 wdim = cv2(sdl_ctx.window_dim);

	float32 s_side = wdim.height;
	if (wdim.width < wdim.height)
		s_side = wdim.width;
	draw_ctx.font_id = FONT_CHELA;

	draw_rect({0, 0}, cv2(sdl_ctx.window_dim), golf_green);

	float32 window_padding = s_side * 0.05f;
	float32 window_radius = s_side * 0.01f;

	Rect player_rect = {};
	player_rect.coords = Vector2{0, 0} + window_padding;
	player_rect.dim = wdim - (window_padding * 2.0f);
	player_rect.dim.x *= (2.0f/3.0f);

	draw_rounded_rect(player_rect.coords, player_rect.dim, window_radius, golf_dark_green);

	Rect title_rect = {};
	title_rect.coords = player_rect.coords;
	title_rect.coords.x += player_rect.dim.x;
	title_rect.dim = wdim - (window_padding * 2.0f);
	title_rect.dim.x *= (1.0f/3.0f);

	draw_rounded_rect(title_rect.coords, title_rect.dim, window_radius, golf_dark_green);

	//const char *title_p1 = "Canadian";
	//float32 title_pxh = title_rect.dim.y * 0.1f;
	//Vector2 title_coords = text_coords(title_rect, title_p1, title_pxh, ALIGN_CENTER);
	//draw_text("Canadian", title_coords, title_pxh, golf_yellow);

	draw_text("Canadian", title_rect, 0.1f, ALIGN_CENTER, golf_yellow);

	draw_text("CUM", { 100, 100 }, 52.0f, golf_yellow);
	draw_text("CUM", { 200, 100 }, 32.0f, golf_yellow);
	draw_text("CUM", { 300, 100 }, 22.0f, golf_yellow);
	draw_text("CUM", { 400, 100 }, 12.0f, golf_yellow);

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