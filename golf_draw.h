global const Color_RGBA golf_green        = {  2,  97,   0,  255 };
global const Color_RGBA golf_dark_green   = {  2,  75,   0,  255 };
global const Color_RGBA golf_yellow       = { 238, 255,  0,  255 };
global const Color_RGBA golf_light_yellow = { 240, 229, 118, 255 };
global const Color_RGBA golf_dark_yellow  = { 197, 180,  22, 255 };

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
