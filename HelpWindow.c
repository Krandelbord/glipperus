#include "HelpWindow.h"
#include "config.h"

void help_window_add_tags_to_buffer(GtkTextBuffer *text_buffer);

void help_window_append_paragraph(GtkTextBuffer *text_buf, gchar *txt, gchar *tag_name);

GtkWidget *help_window_append_general_help(void);
GtkWidget *help_window_new_titlebar(void);
GtkWidget *help_window_append_common(void);
GtkWidget *help_window_append_configuration_help(void);
void help_window_destroy(GtkObject *obj, gpointer);
GtkWindow *help_window_new(GtkWindow *parent_win) {
	GtkWidget *help_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW(help_window), parent_win);
	gtk_window_set_destroy_with_parent(GTK_WINDOW(help_window), TRUE);
	gtk_window_set_title(GTK_WINDOW(help_window), _("glipperus help"));
	
	GdkScreen *gdk_scr = gdk_screen_get_default();
	gint width = gdk_screen_get_width(gdk_scr)/2.9;
	gint height = gdk_screen_get_height(gdk_scr)/2.5;
	
	int reszta_10 = width % 10;
	gtk_window_set_default_size(GTK_WINDOW(help_window), width-reszta_10, height);
	
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
	
	
	GtkWidget *button = gtk_button_new_with_label("One help window is enough...");
	GtkWidget *sub = gtk_bin_get_child(GTK_BIN(button));
	gtk_label_set_text(GTK_LABEL(sub),
		_("<span color=\"red\">One help window is enough...</span>"));
	gtk_label_set_use_markup(GTK_LABEL(sub), TRUE);
	gtk_box_pack_end(GTK_BOX(main_help_box), button, FALSE, FALSE, 0);
	
	
	/* This block is only temporray */
	gtk_window_set_position(GTK_WINDOW(help_window), GTK_WIN_POS_CENTER_ALWAYS);
	g_signal_connect(G_OBJECT(help_window), "destroy", G_CALLBACK(help_window_destroy), NULL);
	/* END of tomerpoary block*/
	
	gtk_widget_show_all(help_window);
	gtk_widget_hide(button);
	return GTK_WINDOW(help_window);
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
		_("Here comes one of the glipperus features: It saves every selection in it's menu,\n"
		  "and when You need it in normal clipboard you can select it from menu.\n\n"), NULL);
	
	help_window_append_paragraph(text_buffer, 
		_("You can pop-up glipperus menu everywhere you need without touching mouse "
		  "changeing focus. By pressing keybard combination - by defatult it is "), NULL);
	help_window_append_paragraph(text_buffer, ("CTRL+ALT+V\n"), "bold_tag");
	
	help_window_append_paragraph(text_buffer, 
		_("For now chaneging default keybard shortcut is not implemented\n"), NULL);
		
	return scrl_win;
}

GtkWidget *help_window_get_notify_button(GtkWindow *help_window) {
	GtkWidget *main_box = gtk_bin_get_child(GTK_BIN(help_window));
	GList *lista_widgetow = gtk_container_get_children(GTK_CONTAINER(main_box));
	while (lista_widgetow!=NULL) {
		if (GTK_IS_BUTTON(lista_widgetow->data))
			return GTK_WIDGET(lista_widgetow->data);
		lista_widgetow = g_list_next(lista_widgetow);
	}
	return NULL;
}

gboolean help_window_hide_notify_button(gpointer gp_button) {
	if ( (gp_button!=NULL) && GTK_IS_BUTTON(gp_button))
		gtk_widget_hide(GTK_WIDGET(gp_button));
	return FALSE;
}

GtkWidget *help_window_new_titlebar(void) {
	GtkWidget *lb = gtk_label_new("<b>glipperus help</b>");
	gtk_label_set_use_markup(GTK_LABEL(lb), TRUE);
	return lb;
}

void help_window_destroy(GtkObject *obj, gpointer data) {
	gtk_object_destroy(obj);
	obj=NULL;
}
