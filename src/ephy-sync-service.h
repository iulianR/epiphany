#ifndef EPHY_SYNC_SERVICE_H
#define EPHY_SYNC_SERVICE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define EPHY_TYPE_SYNC_SERVICE (ephy_sync_service_get_type())

G_DECLARE_FINAL_TYPE (EphySyncService, ephy_sync_service, EPHY, SYNC_SERVICE, GObject)

EphySyncService        *ephy_sync_service_new           (void);
void                    ephy_sync_service_stretch       (void);

G_END_DECLS

#endif
