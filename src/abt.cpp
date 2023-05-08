#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Pixmap.H>
#include "xhk.xpm"

static Fl_Pixmap image_xhk((const char**)xhk_xpm);
Fl_Window* win;
Fl_Text_Buffer* stylebuff;

Fl_Text_Display::Style_Table_Entry style_table[] = {
	{ FL_BLACK, FL_HELVETICA, 12, }, // A - plain
	{ FL_BLACK, FL_HELVETICA_BOLD, 12} // B - bold text
};

const char* content = " Developed by:\n\
     searemind (searemind@gmail.com)\n\
\n\
 Web:\n\
     https://chiselapp.com/user/searemind/repository/xhaskell\n\
\n\
 Bug reports:\n\
     https://chiselapp.com/user/searemind/repository/xhaskell/login?g=tktnew&anon\n\
\n\
 License: \n\
     This program is free software, you can redistribute\n\
     it and/or modify it under the terms of GNU General\n\
     Public License as published by the Free Software\n\
     Foundation, either version 3 of the License, or\n\
     (at your option) any later version.\n\n\
     This program is distributed in the hope that it will\n\
     be useful, but WITHOUT ANY WARRANTY;\n\
     without even the implied\n\
     warranty of MERCHANTABILITY or FITNESS\n\
     FOR A PARTICULAR PURPOSE.\n\n\
     See the GNU General Public License for more details.\n\
     You should have received a copy of the GNU General\n\
     Public Licence along with this program; if not, write\n\
     to the Free Software Foundation, Inc., 675 Mass Ave,\n\
     Cambridge, MA 02139, USA";

const char* bold_keywords[] = {
	"Developed by:",
	"Web:",
	"Bug reports:",
	"License:"
};

#define BOLD_KEYWORDS_LEN 4

static void close_cb(Fl_Widget*, void*) {
	win->hide();
}

static void more_cb(Fl_Widget*, void*) {
	popen("/data/user/0/searemind.xhaskell/lib/libb.so /data/user/0/searemind.xhaskell/app_HOME/.lib/bin/msgabout", "r");
}

void window_center_on_screen(Fl_Window *win) {
	int X, Y, W, H;
	Fl::screen_xywh(X, Y, W, H);
	win->position(W/2 - win->w()/2, H/2 - win->h()/2);
}

static char* prepare_style(char* txt, int len) {
	// paint all with 'A' style at startup
	char* style = new char[len + 1];
	memset(style, 'A', len - 1);
	style[len] = '\0';

	// find bold keywords and paint them
	char* p = 0;
	unsigned int ln;
	for(int i = 0; i < BOLD_KEYWORDS_LEN; i++) {
		p = strstr(txt, bold_keywords[i]);
		if(!p) continue;

		ln = strlen(bold_keywords[i]);
		memset(&style[p - txt], 'B', ln);
	}

	return style;
}
const char* xhk_icon_title = "About XHaskell";
int main(int argc, char **argv) {

	win = new Fl_Window(440, 335, "About XHaskell");
	win->begin();
		Fl_Group* title_group = new Fl_Group(0, 0, 440, 65);
		title_group->box(FL_ENGRAVED_BOX);
		title_group->color(FL_BACKGROUND2_COLOR);
		title_group->begin();
			Fl_Box* image_box = new Fl_Box(5, 5, 60, 55);
			image_box->image(image_xhk);

			Fl_Box* title_label_box = new Fl_Box(70, 10, 300, 25, "XHaskell ");
			title_label_box->labelfont(1);
			title_label_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

			Fl_Box* xhk_copy_box = new Fl_Box(70, 35, 300, 25, "Copyright (c) 2021-2023 searemind");
			xhk_copy_box->labelsize(11);
			xhk_copy_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_TOP);

			// resizable box
			Fl_Box* rbox1 = new Fl_Box(385, 10, 40, 50);
        	Fl_Group::current()->resizable(rbox1);
		title_group->end();

		Fl_Text_Display* txt_display = new Fl_Text_Display(10, 78, 420, 212);
		txt_display->textsize(12);

		// remove bottom scrollbar, since text is already nicely formatted
		txt_display->scrollbar_align(FL_ALIGN_RIGHT);

		Fl_Text_Buffer* buff = new Fl_Text_Buffer();
		buff->append(content);

		// load style buff
		stylebuff = new Fl_Text_Buffer(buff->length());
		char* style = prepare_style(buff->text(), buff->length());

		stylebuff->text(style);
		delete [] style;

		txt_display->buffer(buff);
		txt_display->highlight_data(stylebuff, style_table, 
				sizeof(style_table) / sizeof(style_table[0]), 'A', 0, 0);

		// resizable box
		Fl_Box* rbox2 = new Fl_Box(270, 238, 50, 37);

		Fl_Button* close_button = new Fl_Button(340, 300, 90, 25, "Close");
		close_button->callback(close_cb);
		
		Fl_Button* more_button = new Fl_Button(10, 300, 90, 25, "More");
		more_button->callback(more_cb);
		
		Fl_Group::current()->resizable(rbox2);
	win->end();
	window_center_on_screen(win);
	win->iconlabel(xhk_icon_title);
	
	Fl_RGB_Image myapp_icon (&image_xhk, Fl_Color(0));
	win->icon(&myapp_icon);
	win->xclass("About");
	win->show(argc, argv);
	return Fl::run();
}
