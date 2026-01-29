internal s32
draw_main_menu() {

	UI_Box gui = draw_ui_box(NULL, { 0, 0 }, { 1.0f, 1.0f }, HexToRGBA(0x005F19FF));

	UI_Box menu = draw_centered_ui_box(&gui, 0.05f, HexToRGBA(0xFF0000FF));

	UI_Box player_list = draw_ui_box(&menu, { 0, 0 }, { 2.0f/3.0f, 1.0f }, HexToRGBA(0x008723FF));
	UI_Box buttons_list = draw_ui_box(&menu, { 2.0f/3.0f, 0 }, { 1.0f/3.0f, 1.0f }, HexToRGBA(0x005F19FF));

	draw_text(buttons_list, { 0, 0 }, { 1.0f, 1.0f / 3.0f }, "Canadian Golf", HexToRGBA(0xFFD600FF)) {

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
	//gui_manager.indices.push(GUI_MAIN_MENU);
	//gui_manager.guis[GUI_MAIN_MENU].draw = draw_main_menu;

	return 0;
}

internal u32
golf_update() {
	if (on_down(app_input.mouse.left)) {
		SDL_Log("Mouse Coords: (%f, %f)\n", app_input.mouse.coords.x, app_input.mouse.coords.y);
	}

	SDL_SetRenderDrawColor(sdl_ctx.renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl_ctx.renderer);

	//draw_top_gui();
	draw_main_menu();
	draw_fps();

	SDL_RenderPresent(sdl_ctx.renderer);

	return 0;
}