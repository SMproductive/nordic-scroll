/*
 * Define ESC sequences to use for scroll events.
 * Use "cat -v" to figure out favorite key combination.
 *
 * lines is the number of lines scrolled up or down.
 * If lines is negative, it's the fraction of the terminal size.
 */
struct rule rules[] = {
	/* sequence     event        lines */
	{"\ek",   SCROLL_UP,   1},       /* [Alt] + k ( "\e" = [ESC] = "\033" = "^[" ) */
	{"\ej",   SCROLL_DOWN, 1},       /* [Alt] + j ( "\e" = [ESC] = "\033" = "^[" ) */
	{"\eK",   SCROLL_UP,   -1},       /* [Alt] + K
	{"\eJ",   SCROLL_DOWN, -1},       /* [Alt] + J
	{"\031",  SCROLL_UP,    1},       /* mouse wheel up ( "^Y" = "\031" ) */
	{"\005",  SCROLL_DOWN,  1},       /* mouse wheel Down ( "^E" = "\005" ) */
};
