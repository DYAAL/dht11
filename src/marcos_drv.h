#ifndef __MARCOS_DRV_H__
#define __MARCOS_DRV_H__

#define MAX_MINOR (10)

typedef enum {
    kern_emerg = 0,
    kern_alert,
    kern_crit,
    kern_err,
    kern_warning,
    kern_notice,
    kern_info,
    kern_debug,
    kern_default
} Infor_Level;

#endif // __MARCOS_DRV_H__