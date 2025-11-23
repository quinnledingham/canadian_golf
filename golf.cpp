internal u32
golf_init() {
	

	return 0;
}

internal u32
golf_update() {
	if (on_down(app_input.mouse.left)) {
		SDL_Log("Mouse Coords: (%f, %f)\n", app_input.mouse.coords.x, app_input.mouse.coords.y);
	}

	SDL_SetRenderDrawColor(sdl_ctx.renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl_ctx.renderer);

	draw_ctx.font_id = FONT_CHELA;
	draw_rect({0, 0}, cv2(sdl_ctx.window_dim), golf_green);
	draw_rounded_rect({100, 100}, cv2(sdl_ctx.window_dim), 10.0f, golf_dark_green);
	draw_text("CUM", { 100, 100 }, 52.0f, golf_yellow);
	draw_text("CUM", { 200, 100 }, 32.0f, golf_yellow);
	draw_text("CUM", { 300, 100 }, 22.0f, golf_yellow);
	draw_text("CUM", { 400, 100 }, 12.0f, golf_yellow);

	SDL_RenderPresent(sdl_ctx.renderer);

	return 0;
}