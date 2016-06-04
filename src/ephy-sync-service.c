#include "ephy-sync-service.h"

#include <string.h>
#include <glib/gstdio.h>
#include <json-glib/json-glib.h>
#include <libsoup/soup.h>
#include <nettle/hmac.h>
#include <nettle/pbkdf2.h>

struct _EphySyncService {
  GObject parent_instance;
};

G_DEFINE_TYPE (EphySyncService, ephy_sync_service, G_TYPE_OBJECT);

static const gchar hex_digits[] = "0123456789abcdef";

static gchar *
KW (const gchar *name)
{
  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);
  return g_strconcat ("identity.mozilla.com/picl/v1/",
                      name,
                      NULL);
}

static gchar *
KWE (const gchar *name,
     const gchar *emailUTF8)
{
  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);
  return g_strconcat ("identity.mozilla.com/picl/v1/",
                      name,
                      ":",
                      emailUTF8,
                      NULL);
}

static gchar *
encode_hex (guint8 *data,
            gsize   data_len)
{
  gchar *retval = g_malloc (data_len * 2 + 1);

  for (gsize i = 0; i < data_len; i++) {
    guint8 byte = data[i];

    retval[2 * i] = hex_digits[byte >> 4];
    retval[2 * i + 1] = hex_digits[byte & 0xf];
  }

  retval[data_len * 2] = 0;

  return retval;
}

/*
 * Runs 1000 PBKDF2 iterations using sha256 as hash function.
 */
static void pbkdf2_1k (gsize key_length, guint8 *key,
                       gsize salt_length, guint8 *salt,
                       gsize out_length, guint8 *out)
{
  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);
  pbkdf2_hmac_sha256 (key_length, key, 1000, salt_length, salt, out_length, out);
}

/*
 * HMAC-based Extract-and-Expand Key Derivation Function.
 * Uses sha256 as hash function.
 * https://tools.ietf.org/html/rfc5869
 */
static void hkdf (gsize in_length, guint8 *in,
                  gsize salt_length, guint8 *salt,
                  gsize info_length, guint8 *info,
                  gsize out_length, guint8 *out)
{
  struct hmac_sha256_ctx ctx;
  const gsize hash_length = 32;
  gsize i, offset = 0;
  guint8 *tmp, *prk;
  guint8 counter;

  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  if (out_length > hash_length * 255)
    return;

  /* If salt value was not provided, use an array of hash_length zeros */
  if (salt == NULL) {
    salt = g_malloc0 (hash_length);
    salt_length = hash_length;
  }

  tmp = g_malloc0 (hash_length + info_length + 1);
  prk = g_malloc0 (hash_length);

  /* Step 1: Extract */
  hmac_sha256_set_key (&ctx, salt_length, salt);
  hmac_sha256_update (&ctx, in_length, in);
  hmac_sha256_digest (&ctx, hash_length, prk);

  /* Step 2: Expand */
  hmac_sha256_set_key (&ctx, hash_length, prk);

  for (i = 0, counter = 1; i < out_length; i += hash_length, counter++) {
    memcpy (tmp + offset, info, info_length);
    tmp[offset + info_length] = counter;

    hmac_sha256_update (&ctx, offset + info_length + 1, tmp);
    hmac_sha256_digest (&ctx, hash_length, tmp);

    offset = hash_length;

    memcpy (out + i, tmp, hash_length);
  }

  g_free (salt);
  g_free (tmp);
  g_free (prk);
}

static void
ephy_sync_service_class_init (EphySyncServiceClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class = object_class; // suppress warnings

  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  // TODO: Set finalize, dispose, set/get property methods
}

static void
ephy_sync_service_init (EphySyncService *self)
{
  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);
}

static void
server_response_cb (SoupSession *session,
                    SoupMessage *message,
                    gpointer user_data)
{
  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  if (message->status_code == 200) {
    g_printf ("response body: %s\n", message->response_body->data);
    // TODO: parse response data using JsonParser
  } else {
    g_printerr ("Error response from server: [%u] %s\n", message->status_code, message->reason_phrase);
  }
}

EphySyncService *
ephy_sync_service_new (void)
{
  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  return EPHY_SYNC_SERVICE (g_object_new (EPHY_TYPE_SYNC_SERVICE,
                                          NULL));
}

void
ephy_sync_service_try_login (EphySyncService *self,
                             gboolean login_with_keys,
                             const gchar *emailUTF8,
                             guint8 *authPW,
                             guint8 *sessionToken,
                             guint8 *keyFetchToken)
{
  SoupSession *session;
  SoupMessage *message;
  char *request_body;
  char *authPW_hex;

  g_return_if_fail (EPHY_IS_SYNC_SERVICE (self));
  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  session = soup_session_new_with_options (SOUP_SESSION_USER_AGENT,
                                           "test-json",
                                           NULL);
  message = soup_message_new (SOUP_METHOD_POST,
                              "https://api.accounts.firefox.com/v1/account/login");

  authPW_hex = encode_hex (authPW, TOKEN_LENGTH);
  request_body = g_strconcat ("{\"authPW\": \"",
                              authPW_hex,
                              "\", \"email\": \"",
                              emailUTF8,
                              "\"}",
                              NULL);

  soup_message_set_request (message,
                            "application/json",
                            SOUP_MEMORY_COPY,
                            request_body,
                            strlen (request_body));

  soup_session_queue_message (session, message, server_response_cb, NULL);

  // TODO: find a way to safely free authPW_hex, request_body
  // TODO: find a way to safely destroy session, message
}

void
ephy_sync_service_stretch (EphySyncService *self,
                           const gchar *emailUTF8,
                           const gchar *passwordUTF8,
                           guint8 *authPW,
                           guint8 *unwrapBKey)
{
  gchar *salt_stretch;
  gchar *info_auth;
  gchar *info_unwrap;
  guint8 *quickStretchedPW;

  g_return_if_fail (EPHY_IS_SYNC_SERVICE (self));

  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  salt_stretch = KWE ("quickStretch", emailUTF8);
  quickStretchedPW = g_malloc (TOKEN_LENGTH);
  pbkdf2_1k (strlen (passwordUTF8), (guint8 *) passwordUTF8,
             strlen (salt_stretch), (guint8 *) salt_stretch,
             TOKEN_LENGTH, quickStretchedPW);

  ephy_sync_service_display_hex ("quickStretchedPW", TOKEN_LENGTH, quickStretchedPW);

  info_auth = KW ("authPW");
  hkdf (TOKEN_LENGTH, quickStretchedPW,
        0, NULL,
        strlen (info_auth), (guint8 *) info_auth,
        TOKEN_LENGTH, authPW);

  info_unwrap = KW ("unwrapBkey");
  hkdf (TOKEN_LENGTH, quickStretchedPW,
        0, NULL,
        strlen (info_unwrap), (guint8 *) info_unwrap,
        TOKEN_LENGTH, unwrapBKey);

  g_free (salt_stretch);
  g_free (info_unwrap);
  g_free (info_auth);
  g_free (quickStretchedPW);
}

void
ephy_sync_service_display_hex (const gchar *name,
                               gsize length,
                               guint8 *data)
{
  g_printf ("[%s:%d, %s]\n", __FILE__, __LINE__, __func__);

  g_printf ("%s:\n", name);
  for (gsize i = 0; i < length; i++) {
    g_printf ("%02x", data[i]);
    if ((i + 1) % 8 == 0)
      g_printf ("\n");
  }
  g_printf ("\n");
}
