#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "http_log.h"

#ifndef DEFAULT_ALLOWED_DOMAINS
#define DEFAULT_ALLOWED_DOMAINS NULL
#endif

module AP_MODULE_DECLARE_DATA hostaccess_module;

typedef struct _hostaccess_dir_config {
    apr_table_t        *allowed_domains;
} hostaccess_dir_config;

static int hostaccess_check_domain(request_rec *r)
{
    hostaccess_dir_config *dir = (hostaccess_dir_config *) 
        ap_get_module_config(r->per_dir_config, &hostaccess_module);
    if (!dir || apr_is_empty_table(dir->allowed_domains)) {
        return DECLINED;
    }
    const char *host = r->hostname;
    if (!host || apr_table_get(dir->allowed_domains, host)) {
        return DECLINED;
    }

    char *p = strstr(host, ".");
    while (p && p++) {
        if (apr_table_get(dir->allowed_domains, p)) {
            return DECLINED;
        }
        p = strstr((const char *)p, ".");
    }
    /*
     * close the connection
     */
    r->connection->aborted = 1;
    return OK;
}

static void hostaccess_register_hooks(apr_pool_t *p)
{
    ap_hook_fixups(hostaccess_check_domain, 
                                NULL, NULL, APR_HOOK_MIDDLE);
}


static void *hostaccess_create_dir_config(apr_pool_t *p,  char *d)
{
    hostaccess_dir_config *dir = (hostaccess_dir_config *)
        apr_palloc(p,  sizeof(hostaccess_dir_config));
    dir->allowed_domains = apr_table_make(p, 0);
    return dir;
}

static void *hostaccess_merge_dir_config(apr_pool_t *p, void *basev, void *overridesv)
{
    hostaccess_dir_config *dir  = apr_palloc(p,  sizeof(hostaccess_dir_config));
    hostaccess_dir_config *base = basev;
    hostaccess_dir_config *add  = overridesv;

    if (apr_is_empty_table(add->allowed_domains)) {
        dir->allowed_domains = base->allowed_domains;
    } else {
        dir->allowed_domains = add->allowed_domains;
    }
    return dir;
}

static const char *add_domains(cmd_parms *cmd,  void *mconfig, 
        const char *arg)
{
    if (!arg) {
        return NULL;
    }
    const char *p = arg;
    /* 
     * support *.example.com, .example.com, example.com
     */
    if (*p == '*') {
        p++;
    }
    if (*p == '.') {
        p++;
    }
    hostaccess_dir_config *conf= (hostaccess_dir_config *)mconfig;
    apr_table_set(conf->allowed_domains, p, "1");
    return NULL;
}

static const command_rec hostaccess_module_cmds[] =
{
    AP_INIT_ITERATE("AllowedDomains", 
    add_domains, 
    NULL, 
	OR_ALL, 
	"Parse AllowedDomains list failed"), 
    { NULL }
};

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA hostaccess_module = {
    STANDARD20_MODULE_STUFF, 
    hostaccess_create_dir_config, /* create per-dir    config structures */
    hostaccess_merge_dir_config, /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    hostaccess_module_cmds,                  /* table of config file commands       */
    hostaccess_register_hooks  /* register hooks                      */
};

