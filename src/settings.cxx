#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Menu_Bar.H>

#include "tinyxml2.cpp"
#include "xhk.xpm"

using namespace std;
string orig_file = "/data/user/0/searemind.xhaskell/app_HOME/.lib/xserver/share/conf/jwm/jwmrc";

Fl_Input_Choice* choice;

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
     Cambridge, MA 02139, USA\n\
\n\
 Setting the Wallpaper:\n\
     Use the 'Type:' drop-down to set the type of background.\n\
     Use the 'Select Wallpaper' button to select and apply\n\
     a new wallpaper.\n";

const char* bold_keywords[] = {
	"Developed by:",
	"Web:",
	"Bug reports:",
	"License:",
	"Setting the Wallpaper:",
};

#define BOLD_KEYWORDS_LEN 5

static void close_cb(Fl_Widget*, void*) {
	win->hide();
}

static void more_cb(Fl_Widget*, void*) {
	popen("/data/user/0/searemind.xhaskell/lib/libb.so /data/user/0/searemind.xhaskell/app_HOME/.lib/bin/msgabout", "r");
}

int change_xml(string new_file) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(orig_file.c_str());
	const char* new_bg = new_file.c_str();
	tinyxml2::XMLElement *bg_file = doc.FirstChildElement("JWM")->FirstChildElement("Desktops")->FirstChildElement("Background");
	bg_file->SetText(new_bg);
	bg_file->SetAttribute("type", choice->input()->value());
	if (doc.SaveFile(orig_file.c_str()) == 0) {
		return 0;
	} else return 1;
}

static void apply_wall_cb(Fl_Widget*, void*) {
	const char* i = choice->input()->value();
	printf("Value = %s\n", i);
	string directory = (string(getenv("HOME")) + "/");
	const char* result = \
		fl_file_chooser("Select Wallpaper", "All Files (*)", directory.c_str());
	if (result != nullptr) {
		srand((unsigned) time(NULL));
		ifstream src(orig_file);
		string backup_file = ("/data/user/0/searemind.xhaskell/app_HOME/.temp/jwmrc_" + to_string(rand() % 10000 + 9000));
		printf("%s\n", backup_file.c_str());
		ofstream dst(backup_file);
		dst << src.rdbuf();

		change_xml(string(result));
		popen("libpkill.so libmoxfm.so && libjwm.so -restart && icons", "r");
	}
}

static void menubar_cb(Fl_Widget *w, void*) {
	Fl_Menu_Bar *bar = (Fl_Menu_Bar*)w;
	const Fl_Menu_Item *item = bar->mvalue();
	if (strcmp(item->label(), "Edit Config  ") == 0) {
		popen("libfxite.so /data/user/0/searemind.xhaskell/app_HOME/.lib/xserver/share/conf/jwm/jwmrc", "r");
	}
	if (strcmp(item->label(), "&Quit  ") == 0) {
		win->hide();
	}
	if (strcmp(item->label(), "Web  ") == 0) {
		popen("fifth https://chiselapp.com/user/searemind/repository/xhaskell", "r");
	}
	if (strcmp(item->label(), "Report a Bug  ") == 0) {
		popen("fifth \"https://chiselapp.com/user/searemind/repository/xhaskell/login?g=tktnew&anon\"", "r");
	}
}

void window_center_on_screen(Fl_Window *win) {
	int X, Y, W, H;
	Fl::screen_xywh(X, Y, W, H);
	win->position(W/2 - win->w()/2, H/2 - win->h()/2);
}

static char* prepare_style(char* txt, int len) {
	char* style = new char[len + 1];
	memset(style, 'A', len - 1);
	style[len] = '\0';
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
const char* xhk_icon_title = "Settings";
int main(int argc, char **argv) {

	win = new Fl_Window(440, 335, "Settings");
	Fl::scheme("gtk+");
	win->begin();
		Fl_Group* title_group = new Fl_Group(0, 25, 440, 65);
		title_group->box(FL_ENGRAVED_BOX);
		title_group->color(FL_BACKGROUND2_COLOR);
		title_group->begin();
			Fl_Box* image_box = new Fl_Box(5, 30, 60, 55);
			image_box->image(image_xhk);

			Fl_Box* title_label_box = new Fl_Box(70, 35, 300, 25, "XHaskell");
			title_label_box->labelfont(1);
			title_label_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

			Fl_Box* xhk_copy_box = new Fl_Box(70, 60, 300, 25, "Copyright (c) 2021-2023 searemind");
			xhk_copy_box->labelsize(11);
			xhk_copy_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_TOP);

			Fl_Box* rbox1 = new Fl_Box(385, 10, 40, 50);
        	Fl_Group::current()->resizable(rbox1);
		title_group->end();

		Fl_Text_Display* txt_display = new Fl_Text_Display(10, 103, 420, 187);
		txt_display->textsize(12);

		Fl_Text_Buffer* buff = new Fl_Text_Buffer();
		buff->append(content);

		stylebuff = new Fl_Text_Buffer(buff->length());
		char* style = prepare_style(buff->text(), buff->length());

		stylebuff->text(style);
		delete [] style;

		txt_display->buffer(buff);
		txt_display->highlight_data(stylebuff, style_table, 
				sizeof(style_table) / sizeof(style_table[0]), 'A', 0, 0);

		Fl_Box* rbox2 = new Fl_Box(270, 238, 50, 37);

		Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,440,25);
		menu->add("&File/&Quit  ", "^w", menubar_cb);
		menu->add("&Edit/Edit Config  ", "^o", menubar_cb);
		menu->add("&Help/Web  ", 0, menubar_cb);
		menu->add("&Help/Report a Bug  ", 0, menubar_cb);

		Fl_Button* close_button = new Fl_Button(380, 300, 50, 25, "Close");
		close_button->callback(close_cb);
		
		Fl_Button* more_button = new Fl_Button(10, 300, 50, 25, "More");
		more_button->callback(more_cb);
		
		choice = new Fl_Input_Choice(110, 300, 80, 25, "Type:");
		
		choice->add("tile");
		choice->add("scale");
		choice->add("image");
		choice->value("image");

		Fl_Button* wallpaper_button = new Fl_Button(210, 300, 150, 25, "Select Wallpaper");
		wallpaper_button->callback(apply_wall_cb);
		
		Fl_Group::current()->resizable(rbox2);
	win->end();
	window_center_on_screen(win);
	win->iconlabel(xhk_icon_title);
	
	Fl_RGB_Image myapp_icon (&image_xhk, Fl_Color(0));
	win->icon(&myapp_icon);
	win->xclass("Settings");
	win->show(argc, argv);
	return Fl::run();
}
