inline u8 
alpha(float32 in) {
	return u8(255 * in);
}

/*
(0, 0)
 -> ###############################
    #   #                         #
    # r #            r            #
    # b #                         #
    #   #                         #
    ###############################
    # r #            r            #
    ###############################
*/
struct String_Draw_Info {
    Vector2 dim;      // dim of the text
    Vector2 baseline;

    Vector2 font_dim; // biggest char in font
    Vector2 font_baseline; // baseline for biggest char
};

struct Draw_Context {
  Mesh square;
  Mesh square_3D;
  Mesh rounded_rect;
  Mesh cube;
  Mesh sphere;

  u32 font_id;
};

