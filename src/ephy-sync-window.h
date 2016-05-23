#ifndef EPHY_SYNC_WINDOW_H
#define EPHY_SYNC_WINDOW_H

#include <gtk/gtk.h>

#include "ephy-sync-service.h"

G_BEGIN_DECLS

#define EPHY_TYPE_SYNC_WINDOW (ephy_sync_window_get_type ())

G_DECLARE_FINAL_TYPE (EphySyncWindow, ephy_sync_window, EPHY, SYNC_WINDOW, GtkDialog)

GtkWidget       *ephy_sync_window_new        (EphySyncService *sync_service);

G_END_DECLS

#endif
