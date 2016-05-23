#include "ephy-sync-service.h"

#include <stdio.h>

struct _EphySyncService {
  GObject parent_instance;
};

G_DEFINE_TYPE (EphySyncService, ephy_sync_service, G_TYPE_OBJECT);

static void
ephy_sync_service_class_init (EphySyncServiceClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class = object_class; // suppress warnings

  printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  // TODO: Set finalize, dispose, set/get property methods
}

static void
ephy_sync_service_init (EphySyncService *self)
{
  printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);
}

EphySyncService *
ephy_sync_service_new (void)
{
  printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  return EPHY_SYNC_SERVICE (g_object_new (EPHY_TYPE_SYNC_SERVICE,
                                          NULL));
}

void
ephy_sync_service_stretch (void)
{
  printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);
}
