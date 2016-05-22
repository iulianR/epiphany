#include "ephy-sync-window.h"
#include "ephy-gui.h"

#include <gtk/gtk.h>

struct _EphySyncWindow {
  GtkDialog parent_instance;

  GCancellable *cancellable;

  GtkWidget *entry_email;
  GtkWidget *entry_password;
  GtkButton *btn_submit;

  GActionGroup *action_group;

  // TODO: Add sync service instance
};

G_DEFINE_TYPE (EphySyncWindow, ephy_sync_window, GTK_TYPE_DIALOG)

static void
quickstretch (GSimpleAction *action,
              GVariant      *parameter,
              gpointer       user_data)
{
  EphySyncWindow *self = EPHY_SYNC_WINDOW (user_data);
  printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  printf("email:%s\n", gtk_entry_get_text (GTK_ENTRY (self->entry_email)));
  printf("password:%s\n", gtk_entry_get_text (GTK_ENTRY (self->entry_password)));
}

static GActionGroup *
create_action_group (EphySyncWindow *self)
{
  GSimpleActionGroup *group;

  const GActionEntry entries[] = {
    { "quickstretch", quickstretch }
  };

  group = g_simple_action_group_new ();
  g_action_map_add_action_entries (G_ACTION_MAP (group), entries, G_N_ELEMENTS (entries), self);

  return G_ACTION_GROUP (group);
}

static void
ephy_sync_window_class_init (EphySyncWindowClass *klass)
{
  // GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  gtk_widget_class_set_template_from_resource (widget_class,
                                               "/org/gnome/epiphany/sync-dialog.ui");

  gtk_widget_class_bind_template_child (widget_class, EphySyncWindow, entry_email);
  gtk_widget_class_bind_template_child (widget_class, EphySyncWindow, entry_password);
  gtk_widget_class_bind_template_child (widget_class, EphySyncWindow, btn_submit);
}

static void
ephy_sync_window_init (EphySyncWindow *self)
{
  printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  gtk_widget_init_template (GTK_WIDGET (self));

  self->cancellable = g_cancellable_new ();

  ephy_gui_ensure_window_group (GTK_WINDOW (self));

  self->action_group = create_action_group (self);
  gtk_widget_insert_action_group (GTK_WIDGET (self), "sync", self->action_group);
}

GtkWidget *
ephy_sync_window_new (void)
{
  EphySyncWindow *self;

  printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  self = g_object_new (EPHY_TYPE_SYNC_WINDOW,
                       "use-header-bar", TRUE,
                       NULL);

  return GTK_WIDGET (self);
}
