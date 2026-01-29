/*
// WARNING: centers vertically using baseline
inline Vector2
get_centered_text_coords(String_Draw_Info string_info, Vector2 dim, u32 text_align) {
	Vector2 coords = { 0, 0 };
	Vector2 text_coords = {};

	switch (text_align) {
		case ALIGN_CENTER:
			text_coords.x = coords.x + (dim.x / 2.0f) - (string_info.dim.x / 2.0f);
			text_coords.y = coords.y + (dim.y / 2.0f) - (string_info.baseline.y / 2.0f);
			break;
		case ALIGN_RIGHT:
			text_coords.x = coords.x + (dim.x) - (string_info.dim.x);
			text_coords.y = coords.y + (dim.y / 2.0f) - (string_info.baseline.y / 2.0f) + string_info.baseline.y;
			break;
		case ALIGN_LEFT:
			text_coords.x = coords.x + string_info.baseline.x;
			text_coords.y = coords.y + (dim.y / 2.0f) - (string_info.baseline.y / 2.0f) + string_info.baseline.y;
			break;
	}

	return text_coords;
}

internal Vector2
text_coords(Rect rect, String_Draw_Info info, u32 text_align) {

	Vector2 text_coords = {};

	switch (text_align) {
		case ALIGN_CENTER:
			text_coords.x = rect.coords.x + (rect.dim.x / 2.0f) - (info.dim.x / 2.0f);
			text_coords.y = rect.coords.y + (rect.dim.y / 2.0f) - (info.dim.y / 2.0f);
			break;
		case ALIGN_RIGHT:
			text_coords.x = rect.coords.x + (rect.dim.x) - (info.dim.x);
			text_coords.y = rect.coords.y + (rect.dim.y / 2.0f) - (info.baseline.y / 2.0f) + info.baseline.y;
			break;
		case ALIGN_LEFT:
			text_coords.x = rect.coords.x + info.baseline.x;
			text_coords.y = rect.coords.y + (rect.dim.y / 2.0f) - (info.baseline.y / 2.0f) + info.baseline.y;
			break;
	}

	return text_coords;
}



internal void
draw_button(GUI *gui, const Draw_Style style, const u32 state, Rect rect, const char *label, const u32 text_align) {
	float corner_radius = rect.h / 4.0f;
	float32 shift = 0.1f * rect.h;

	rect.coords += gui->backdrop_px;
	draw_rounded_rect(rect.coords, rect.dim, rect.h / 4.0f, { 0, 0, 0, alpha(0.2f) }); // back
	rect.coords -= gui->backdrop_px;

	Color_RGBA back_color = style.background_colors[state];
	draw_rounded_rect(rect.coords, rect.dim, rect.h / 4.0f, style.background_colors[GUI_PRESSED]); // back
	Rect inner = rect_percent(rect, 0.1f);
	draw_rounded_rect(inner.coords, inner.dim, inner.h / 4.0f, back_color); // back

	if (label) {
		float32 pixel_height;
		if (rect.dim.y < rect.dim.x)
			pixel_height = rect.dim.y * 0.7f;
		else
			pixel_height = rect.dim.x * 0.7f;

		String_Draw_Info info = get_string_draw_info(label, -1, pixel_height);
		Vector2 text_coords = rect.coords + get_centered_text_coords(info, rect.dim, text_align);
		Color_RGBA text_color = style.text_colors[state];

		//gfx_scissor_push(rect.coords, rect.dim);
		draw_text(label, text_coords, pixel_height, text_color); // text
		//gfx_scissor_pop();
	}
}

internal float32
draw_textbox(Draw_Textbox textbox) {
	float32 padding = textbox.dim.y * 0.05f;
	Vector2 label_coords = textbox.coords + Vector2 { padding, padding };
	float32 label_height = textbox.dim.y * 0.5f;
	Vector2 label_dim = { textbox.dim.x, label_height };
	float32 pixel_height = textbox.dim.y * 0.8f;

	Color_RGBA back_color = textbox.style.background_colors[textbox.state];
	Color_RGBA text_color = textbox.style.text_colors[textbox.state];

	String_Draw_Info string_info = get_string_draw_info(textbox.text, -1, pixel_height);
	Vector2 text_coords = textbox.coords + get_centered_text_coords(string_info, textbox.dim, textbox.text_align);

	String_Draw_Info string_info_cursor = get_string_draw_info(textbox.text, textbox.cursor_position, pixel_height);
	Vector2 cursor_coords = text_coords;
	cursor_coords.x += string_info_cursor.dim.x;
	cursor_coords.y = textbox.coords.y;

	float32 shift = textbox.text_shift;
	if (textbox.state == GUI_ACTIVE) {
		if (cursor_coords.x - shift + textbox.cursor_width >= textbox.coords.x + textbox.dim.x) {
			shift = (cursor_coords.x + textbox.cursor_width) - (textbox.coords.x + textbox.dim.x);
		} else if (cursor_coords.x - shift <= textbox.coords.x) {
			shift = cursor_coords.x - textbox.coords.x;
		}

		text_coords.x -= shift;
		cursor_coords.x -= shift;
	}

	if (string_info.dim.x < textbox.dim.x)
		shift = 0.0f;

	// Back
	draw_rect(textbox.coords, textbox.dim, back_color);

	// Label
	if (textbox.label != 0) {
		Color_RGBA label_text_color = text_color;
		label_text_color.a = alpha(0.8f);
		String_Draw_Info label_info = get_string_draw_info(textbox.label, -1, label_height);
		label_coords.y += label_info.baseline.y;
		draw_text(textbox.label, label_coords, label_height, label_text_color);
	}

	//gfx.draw_string_draw_info(&string_info, text_coords);

	// Text
	//gfx_scissor_push(textbox.coords, textbox.dim);
	draw_text(textbox.text, text_coords, pixel_height, text_color);
	//gfx_scissor_pop();

	// Cursor
	if (textbox.state == GUI_ACTIVE) // clicked on
		draw_rect(cursor_coords, { textbox.cursor_width, textbox.dim.y }, textbox.cursor_color); // cursor

	return shift;
}

internal void
gui_start(GUI *gui) {
	//gfx_bind_pipeline(PIPELINE_NOISE);

	//draw_rect({0, 0}, cv2(vk_ctx.window_dim), gui->background_color);
	//vulkan_bind_pipeline(PIPELINE_2D);
	//draw_rect({0, 0}, cv2(vk_ctx.window_dim), &menu_noise);


	Vector2 gui_dim_pixels = gui->dim * cv2(sdl_ctx.window_dim);
	gui->padding_px = gui_dim_pixels * gui->padding;

	gui->segment_dim = (gui_dim_pixels / cv2(gui->segments));
	gui->segment_dim -= (gui->padding_px - (gui->padding_px / cv2(gui->segments)));

	gui->backdrop_px = gui->backdrop * cv2(sdl_ctx.window_dim);

	gui->index = 1;
}

internal void
gui_end(GUI *gui) {
	bool8 active_gui = false;
	if (!gui_manager.indices.empty()) {
		u32 top_index = gui_manager.indices.top();
		if (gui == &gui_manager.guis[top_index])
			active_gui = true;
	}

	
	if (active_gui && gui->hover != 0) {
		SDL_SetCursor(sdl_ctx.pointer_cursor);
	} else {
		SDL_SetCursor(sdl_ctx.default_cursor);
	}
}

internal bool8
coords_in_rect(Vector2 in, Vector2 coords, Vector2 dim) {
	if (in.x >= coords.x && in.x <= coords.x + dim.x &&
		in.y >= coords.y && in.y <= coords.y + dim.y)
		return true;
	return false;
}

internal void
gui_do_mouse_input(GUI *gui, Vector2 coords, Vector2 dim) {
	if (coords_in_rect(app_input.mouse.coords, coords, dim)) {
		gui->hover = gui->index;
		// don't hover anyways
		//if (gui->active != 0 && !gui->enabled)

		// don't hover other components when one is pressed
		if (gui->pressed && gui->pressed != gui->index)
			gui->hover = 0;
	} else if (gui->hover == gui->index) {
		gui->hover = 0;
	}
}

// does mouse by default but for keyboard input to work
// gui->hover has to be set before. It does the logic of when keyboard is clicked
// but it does not know which one is being hovered.
internal u32
gui_update(GUI *gui, Vector2 coords, Vector2 dim) {
	u32 state = GUI_DEFAULT; // the state of the calling gui component

	Button gui_select = {};
	switch (app_input.last_input_type) {
		//case CONTROLLER_INPUT:
		//case KEYBOARD_INPUT: gui_select = *gui->input.select;     break;
		case IN_MOUSE: gui_select = app_input.mouse.left; break;
	}

	if (app_input.last_input_type == IN_MOUSE)
		gui_do_mouse_input(gui, coords, dim);

	if (gui->hover == gui->index) {
		state = GUI_HOVER;

		if (on_down(gui_select)) {
			gui->pressed = gui->hover;
		}
	}

	if (gui->pressed == gui->index) {
		state = GUI_PRESSED;

		if (on_up(gui_select)) {
			if (gui->hover == gui->index) {
				gui->active = gui->pressed;
			} else {
				gui->pressed = 0;
				gui->active = 0;
			}
		}
	}

	if (gui->active == gui->index) {
		state = GUI_ACTIVE;
	}

	return state;
}

internal bool8
gui_button(GUI *gui, const char *label, Vector2_s32 segment_coords) {
	Rect rect = {};
	rect.coords = gui->coords + (cv2(segment_coords) * gui->segment_dim) + (gui->padding_px * float32(gui->index - 1));
	rect.dim = gui->segment_dim;
	u32 state = gui_update(gui, rect.coords, rect.dim);
	draw_button(gui, gui->style, state, rect, label, ALIGN_CENTER);

	gui->index++;

	bool8 button_pressed = false;
	if (state == GUI_ACTIVE) {
		button_pressed = true;
		gui->pressed = 0;
		gui->active = 0;
	}
	return button_pressed;
}

internal bool8
gui_textbox(GUI *gui, float32 *value, Vector2_s32 segment_coords) {
	Rect rect = {};
	rect.coords = gui->coords + (cv2(segment_coords) * gui->segment_dim) + (gui->padding_px * float32(gui->index - 1));
	rect.dim = gui->segment_dim;

	gui->edit.index = gui->index++;
	gui->edit.value = (void *)value;
	gui->edit.value_type = TYPE_FLOAT32;

	u32 state = gui_update(gui, rect.coords, rect.dim);

	Draw_Textbox draw = {};
	draw.style = gui->style;
	draw.state = state;
	draw.cursor_position = gui->edit.cursor_position;
	draw_textbox(draw);

	return false;
}

inline s32
draw_top_gui() {
	// Drawing no menu... if esc is pressed add pause to the stack
	if (gui_manager.indices.empty()) {
		if (on_down(IN_BACK)) {
			//sdl_set_relative_mouse_mode(false);
			gui_manager.indices.push(GUI_PAUSE);
		}

		return SUCCESS;
	}

	u32 index = gui_manager.indices.top();
	GUI *gui = &gui_manager.guis[index];
	#ifdef DEBUG
	if (!gui->draw) {
		app_log_error("draw_top_gui(): draw function not set for index (%d)\n", index);
		return FAILURE;
	}
	#endif // DEBUG
	return gui->draw(gui);
}

internal void
ui_box(UI_Box *parent, UI_Box *child, float32 margin_percent, float32 width_percent) {
	float32 shorter_side = parent->dim.height;
	if (parent->dim.width < parent->dim.height)
		shorter_side = parent->dim.width;

	float32 margin = shorter_side * margin_percent;

	child->coords = parent->coords + margin;
	for (u32 i = 0; i < parent->children_count; i++) {
		UI_Box *other = parent->children[i];
		child->coords.x += other->dim.x;
	}

	child->dim = parent->dim - (margin * 2.0f);
	child->dim.x *= width_percent;

	parent->children[parent->children_count++] = child;
	child->parent = parent;
};

internal void
ui_update_boxes(UI_Box *box) {
	if (box->children_count == 0) {
		return;
	}

	float32 new_width = box->dim.width / float32(box->children_count);

	for (u32 i = 0; i < box->children_count; i++) {
		UI_Box *child = box->children[i];
		ui_update_boxes(child);

		child->dim.x = new_width;
		child->coords.x += (i * new_width);
	}
}

 internal Rect
 get_centered_rect(Rect og, float32 margin_percent) {
 	float32 shorter_side = og.dim.height;
	if (og.dim.width < og.dim.height)
		shorter_side = og.dim.width;

	float32 margin = shorter_side * margin_percent;

	Rect rect = {};
	rect.coords = og.coords + margin;
	rect.dim = og.dim - (margin * 2.0f);
	return rect;
 }
 */


internal bool8
	coords_in_rect(Vector2 in, Vector2 coords, Vector2 dim) {
	if (in.x >= coords.x && in.x <= coords.x + dim.x &&
		in.y >= coords.y && in.y <= coords.y + dim.y)
		return true;
	return false;
}

internal void
gui_do_mouse_input(UI *gui, Vector2 coords, Vector2 dim) {
	if (coords_in_rect(app_input.mouse.coords, coords, dim)) {
		gui->hover = gui->index;
		// don't hover anyways
		//if (gui->active != 0 && !gui->enabled)

		// don't hover other components when one is pressed
		if (gui->pressed && gui->pressed != gui->index)
			gui->hover = 0;
	} else if (gui->hover == gui->index) {
		gui->hover = 0;
	}
}

// does mouse by default but for keyboard input to work
// gui->hover has to be set before. It does the logic of when keyboard is clicked
// but it does not know which one is being hovered.
internal u32
ui_update(UI_Box *box, Vector2 coords, Vector2 dim) {
	u32 state = GUI_DEFAULT; // the state of the calling gui component

	UI *gui = box->ui;

	Button gui_select = {};
	switch (app_input.last_input_type) {
		//case CONTROLLER_INPUT:
		//case KEYBOARD_INPUT: gui_select = *gui->input.select;     break;
		case IN_MOUSE: gui_select = app_input.mouse.left; break;
	}

	if (app_input.last_input_type == IN_MOUSE)
		gui_do_mouse_input(gui, coords, dim);

	if (gui->hover == gui->index) {
		state = GUI_HOVER;

		if (on_down(gui_select)) {
			gui->pressed = gui->hover;
		}
	}

	if (gui->pressed == gui->index) {
		state = GUI_PRESSED;

		if (on_up(gui_select)) {
			if (gui->hover == gui->index) {
				gui->active = gui->pressed;
			} else {
				gui->pressed = 0;
				gui->active = 0;
			}
		}
	}

	if (gui->active == gui->index) {
		state = GUI_ACTIVE;
		gui->pressed = 0;
		gui->active = 0;
	}

	gui->index++;

	return state;
}


internal UI_Box
draw_ui_box(UI_Box *parent, Vector2 coords_percent, Vector2 dim_percent, Color_RGBA color) {

	UI *ui = NULL;
	Vector2 coords = {};
	Vector2 dim = {};
	if (parent == NULL) {
		coords = { 0, 0 };
		dim = (Vector2)sdl_ctx.window_dim;
	} else {
		coords = parent->coords;
		dim = parent->dim;
		ui = parent->ui;
	}

	Vector2 coords_delta = dim * coords_percent;
	coords += coords_delta;

	dim = dim * dim_percent;

	draw_rect(coords, dim, color);

	UI_Box ret = {
		.ui = ui,
		.parent = parent,
		.coords_percent = coords_percent,
		.dim_percent = dim_percent,
		.coords = coords,
		.dim = dim,
	};

	return ret;

}

internal UI_Box
draw_centered_ui_box(UI_Box *parent, float32 padding_percent, Color_RGBA color) {
	ASSERT(parent != NULL);

	Vector2 coords_percent = {};

	if (parent->dim.x >= parent->dim.y) {
		coords_percent.y = padding_percent;
		float32 coords_delta = parent->dim.y * padding_percent;
		float32 coords_x = parent->coords.x + coords_delta;
		coords_percent.x = (coords_x - parent->coords.x) / parent->dim.x;
	} else {
		coords_percent.x = padding_percent;
		float32 coords_delta = parent->dim.x * padding_percent;
		float32 coords_y = parent->coords.y + coords_delta;
		coords_percent.y = (coords_y - parent->coords.x) / parent->dim.y;
	}

	Vector2 dim_percent = (parent->dim - (parent->dim * coords_percent * 2.0f))/parent->dim;
	return draw_ui_box(parent, coords_percent, dim_percent, color);
}

internal Vector2
text_coords(UI_Box box, String_Draw_Info info, u32 text_align) {

	Vector2 text_coords = {};

	switch (text_align) {
		case ALIGN_CENTER:
			text_coords.x = box.coords.x + (box.dim.x / 2.0f) - (info.dim.x / 2.0f);
			text_coords.y = box.coords.y + (box.dim.y / 2.0f) - (info.dim.y / 2.0f);
			break;
		case ALIGN_RIGHT:
			text_coords.x = box.coords.x + (box.dim.x) - (info.dim.x);
			text_coords.y = box.coords.y + (box.dim.y / 2.0f) - (info.baseline.y / 2.0f) + info.baseline.y;
			break;
		case ALIGN_LEFT:
			text_coords.x = box.coords.x + info.baseline.x;
			text_coords.y = box.coords.y + (box.dim.y / 2.0f) - (info.baseline.y / 2.0f) + info.baseline.y;
			break;
	}

	return text_coords;
}

internal void
draw_ui_text(UI_Box *parent, Vector2 coords_percent, Vector2 dim_percent, const char *text, Color_RGBA color) {
	UI_Box box = draw_ui_box(parent, coords_percent, dim_percent, HexToRGBA(0x0));

	float32 pixel_height;
	if (box.dim.y < box.dim.x)
		pixel_height = box.dim.y * 0.7f;
	else
		pixel_height = box.dim.x * 0.7f;

	String_Draw_Info info = get_string_draw_info(text, get_length(text), pixel_height);
	Vector2 coords = text_coords(box, info, ALIGN_CENTER);
	draw_text(text, coords, pixel_height, color);
}

internal Rect
	rect_percent(Rect in, float32 p) {
	Vector2 thickness = { in.h * p, in.h * p }; // px

	Rect r = {};
	r.dim = in.dim - thickness;
	r.coords = in.coords + (in.dim / 2.0f) - (r.dim / 2.0f);
	return r;
}

internal bool8
draw_ui_button(UI_Box *parent, Vector2 coords_percent, Vector2 dim_percent, const char *label, Color_RGBA text_color, Color_RGBA back_color) {
	UI_Box box = draw_ui_box(parent, coords_percent, dim_percent, HexToRGBA(0x0));

	u32 state = ui_update(&box, box.coords, box.dim);

	float32 factor = 1.0f;
	if (state == GUI_HOVER) {
		factor = 1.8f;
	} else if (state == GUI_PRESSED) {
		factor = 0.7f;
	}

	//text_color = color_adjust_brightness(text_color, factor);
	back_color = color_adjust_brightness(back_color, factor);

	Rect rect = {};
	rect.coords = box.coords;
	rect.dim = box.dim;

	float corner_radius = rect.h / 4.0f;
	float32 shift = 0.1f * rect.h;

	//rect.coords += gui->backdrop_px;
	//draw_rounded_rect(rect.coords, rect.dim, rect.h / 4.0f, { 0, 0, 0, alpha(0.2f) }); // back
	//rect.coords -= gui->backdrop_px;

	//Color_RGBA back_color = style.background_colors[state];
	draw_rounded_rect(rect.coords, rect.dim, rect.h / 8.0f, color_adjust_brightness(back_color, 0.9f)); // back
	Rect inner = rect_percent(rect, 0.1f);
	draw_rounded_rect(inner.coords, inner.dim, inner.h / 8.0f, back_color); // back

	if (label) {
		float32 pixel_height;
		if (rect.dim.y < rect.dim.x)
			pixel_height = rect.dim.y * 0.7f;
		else
			pixel_height = rect.dim.x * 0.7f;
	
		pixel_height = roundf(pixel_height);

		String_Draw_Info info = get_string_draw_info(label, get_length(label), pixel_height);
		Vector2 coords = text_coords(box, info, ALIGN_CENTER);
		//Color_RGBA text_color = style.text_colors[state];

		draw_text(label, coords, pixel_height, text_color); // text
	}

	bool8 button_pressed = false;
	if (state == GUI_ACTIVE) {
		button_pressed = true;
	}
	return button_pressed;
}
