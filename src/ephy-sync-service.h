#ifndef EPHY_SYNC_SERVICE_H
#define EPHY_SYNC_SERVICE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define EPHY_TYPE_SYNC_SERVICE          (ephy_sync_service_get_type ())
#define EPHY_SYNC_SERVICE_TOKEN_LENGTH  32

G_DECLARE_FINAL_TYPE (EphySyncService, ephy_sync_service, EPHY, SYNC_SERVICE, GObject)

EphySyncService *ephy_sync_service_new          (void);
void             ephy_sync_service_stretch      (EphySyncService *self,
                                                 const gchar *emailUTF8,
                                                 const gchar *passwordUTF8,
                                                 guint8 *authPW,
                                                 guint8 *unwrapBKey);
void             ephy_sync_service_try_login    (EphySyncService *self,
                                                 gboolean login_with_keys,
                                                 const gchar *emailUTF8,
                                                 guint8 *authPW,
                                                 guint8 *sessionToken,
                                                 guint8 *keyFetchToken);

G_END_DECLS

#endif
