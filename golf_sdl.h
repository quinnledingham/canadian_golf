struct SDL_Context {
	bool8 should_quit;
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_TextEngine *text_engine;

	u64 start_ticks;
	u64 now_ticks;
	u64 last_ticks;
	u64 performance_frequency;

	float64 run_time_s;
	float64 frame_time_s;
	float64 frames_per_s;

	Vector2_s32 window_dim;
};

#define sdl_log(fmt, ...) SDL_Log("(sdl) " fmt, ##__VA_ARGS__)
#define sdl_log_error(fmt, ...) SDL_LogError(SDL_LOG_CATEGORY_ERROR, "(sdl) " fmt, ##__VA_ARGS__)
