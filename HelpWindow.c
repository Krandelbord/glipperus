#include "HelpWindow.h"
#include "config.h"

void help_window_add_tags_to_buffer(GtkTextBuffer *text_buffer);

void help_window_append_paragraph(GtkTextBuffer *text_buf, gchar *txt, gchar *tag_name);

GtkWidget *help_window_append_general_help(void);
GtkWidget *help_window_new_titlebar(void);
GtkWidget *help_window_append_common(void);
GtkWidget *help_window_append_configuration_help(void);

GtkWidget *help_window_new(GtkWindow *parent_win) {
	GtkWidget *help_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW(help_window), parent_win);
	gtk_window_set_destroy_with_parent(GTK_WINDOW(help_window), TRUE);
	gtk_window_set_title(GTK_WINDOW(help_window), _("glipper help"));
	
	GdkScreen *gdk_scr = gdk_screen_get_default();
	gint width = gdk_screen_get_width(gdk_scr);
	gint height = gdk_screen_get_height(gdk_scr);
	g_print("stara szerokość = %d, wysokość = %d\n", width, height);
	
	gtk_window_set_default_size(GTK_WINDOW(help_window), width/2.1, height/2.5);
	
	GtkWidget *main_help_box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(help_window), main_help_box);
	
	//gtk_box_pack_start(GTK_BOX(main_help_box), help_window_new_titlebar(), FALSE, FALSE, 5);
	
	GtkWidget *help_notebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(help_notebook), GTK_POS_LEFT);
	gtk_box_pack_start(GTK_BOX(main_help_box), help_notebook, TRUE, TRUE, 0);
	
	GtkWidget *conf_help_tab = help_window_append_configuration_help();
	gtk_notebook_append_page(GTK_NOTEBOOK(help_notebook), conf_help_tab, gtk_label_new(_("Configuration help")));
	GtkWidget *general_help = help_window_append_general_help();
	gtk_notebook_append_page(GTK_NOTEBOOK(help_notebook), general_help, gtk_label_new(_("General help")));
	
	
	/*
	GtkWidget *title_label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(title_label),
		// Translators: pozdro 600a
		_("<b>Configuration options:</b>")
	);
	*/
	
	/* This block is only temporray */
	//GtkWidget *button = gtk_button_new_with_label("Bla bla ");
	//gtk_box_pack_end(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show_all(help_window);
	gtk_window_set_position(GTK_WINDOW(help_window), GTK_WIN_POS_CENTER_ALWAYS);
	g_signal_connect(G_OBJECT(help_window), "destroy", gtk_main_quit, NULL);
	/* END of tomerpoary block*/
	return help_window;
}

GtkWidget *help_window_append_common(void) {
	GtkWidget *scrl_win = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrl_win), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	
	GtkWidget *text_view = gtk_text_view_new();
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(text_view), 5);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
	gtk_container_add(GTK_CONTAINER(scrl_win), text_view);
	
	GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
	
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(text_view), text_buffer);
	
	gtk_text_buffer_create_tag(text_buffer, "blue_tag", "foreground", "blue", NULL);
	gtk_text_buffer_create_tag(text_buffer, "red_tag", "foreground", "red", NULL);
	gtk_text_buffer_create_tag(text_buffer, "bold_tag", "weight", PANGO_WEIGHT_BOLD, NULL);
	
	gtk_text_buffer_create_tag(text_buffer, "title_tag", 
		"weight", PANGO_WEIGHT_BOLD, 
		"justification", GTK_JUSTIFY_CENTER,
		"foreground", "blue",
		"size-points", 12.0,
		NULL);
	
	return scrl_win;	
}

void help_window_append_paragraph(GtkTextBuffer *text_buffer, gchar *txt, gchar *tag_name) {
	GtkTextIter buf_start_iter, buf_end_iter;
	gtk_text_buffer_get_end_iter(text_buffer, &buf_start_iter);
	GtkTextMark *start_mark = gtk_text_buffer_create_mark(text_buffer, NULL, &buf_start_iter, TRUE);
	
	gtk_text_buffer_insert_at_cursor(text_buffer, txt, -1);
	
	gtk_text_buffer_get_end_iter(text_buffer, &buf_end_iter);
	
	gtk_text_buffer_get_iter_at_mark(text_buffer, &buf_start_iter, start_mark);

	if (tag_name)
		gtk_text_buffer_apply_tag_by_name(text_buffer, tag_name, &buf_start_iter, &buf_end_iter);
}

GtkWidget *help_window_append_configuration_help(void) {
	GtkWidget *scrl_win = help_window_append_common();
	GtkTextBuffer *text_buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW(gtk_bin_get_child(GTK_BIN(scrl_win))) );
	
	help_window_append_paragraph(text_buffer, _("Configuration Help\n\n"), "title_tag");
	
	help_window_append_paragraph(text_buffer, _("Override primary selection: "), "bold_tag");
	help_window_append_paragraph(text_buffer, 
		// Translators: This is description of "Override primary selection" option from configuration  
		_("When this option is enabled choosing entry from gliper " 
		  "menu will put this in normal Clipboard and in Selection "
		  "clipboard as well.\n\n"), NULL);
	
	help_window_append_paragraph(text_buffer, _("Overwrite similar entries: "), "bold_tag");
	help_window_append_paragraph(text_buffer, 
		_("When this option is enabled entries which has same prefix as "
		"current selection will be overwriten\n\n"), NULL);
	
	help_window_append_paragraph(text_buffer, _("Transparent tray: "), "bold_tag");
	help_window_append_paragraph(text_buffer, _("This option needs restart to take effect\n\n"), NULL);
	
	help_window_append_paragraph(text_buffer, _("Transparent tray: "), "bold_tag");
	help_window_append_paragraph(text_buffer, _("This option needs restart to take effect\n\n"), NULL);
	
	return scrl_win;
}

GtkWidget *help_window_append_general_help(void) {
	GtkWidget *scrl_win = help_window_append_common();
	GtkTextBuffer *text_buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW(gtk_bin_get_child(GTK_BIN(scrl_win))) );
	help_window_append_paragraph(text_buffer, _("General Help\n\n"), "title_tag");
	help_window_append_paragraph(text_buffer, 
		_("Xserver has two clipboards:\n"
		  "1. To store selection\n"
		  "2. For normal copy+paste\n"), NULL);
	help_window_append_paragraph(text_buffer, 
		_("Some applications save information only in selection. Even if you select\n"
		  "\"Copy\" operation, or press Ctrl+C or Ctrl+insert.\n"
		  "As the result you cannot paste it to some other application normally.\n\n"), NULL);
	help_window_append_paragraph(text_buffer, 
		_("Here comes one of the glipper features: It saves every selection in it's menu,\n"
		  "and when You need it in normal clipboard you can select it from menu.\n\n"), NULL);
	
	help_window_append_paragraph(text_buffer, _("Current clipboard entry is marked with tick mark\n"), NULL);
	
	return scrl_win;
}

GtkWidget *help_window_new_titlebar(void) {
	GtkWidget *lb = gtk_label_new("<b>Glipper help</b>");
	gtk_label_set_use_markup(GTK_LABEL(lb), TRUE);
	return lb;
}
