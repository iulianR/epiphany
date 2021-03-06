/* vim: set foldmethod=marker sw=2 ts=2 sts=2 et: */
/* -*- Mode: C; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * ephy-download.c
 * This file is part of Epiphany
 *
 * Copyright © 2011, 2015 - Igalia S.L.
 *
 * Epiphany is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Epiphany is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Epiphany.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _EPHY_DOWNLOAD_WIDGET_H
#define _EPHY_DOWNLOAD_WIDGET_H

#include <glib-object.h>
#include "ephy-download.h"

G_BEGIN_DECLS

#define EPHY_TYPE_DOWNLOAD_WIDGET ephy_download_widget_get_type()

G_DECLARE_FINAL_TYPE (EphyDownloadWidget, ephy_download_widget, EPHY, DOWNLOAD_WIDGET, GtkGrid)

GtkWidget     *ephy_download_widget_new          (EphyDownload *ephy_download);

EphyDownload  *ephy_download_widget_get_download (EphyDownloadWidget *widget);

G_END_DECLS

#endif /* _EPHY_DOWNLOAD_WIDGET_H */
