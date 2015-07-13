#ifndef CWO_ERRORS_H
#define CWO_ERRORS_H

#define CWO_SUCCESS 0

#define CWOERR_BUG -1	    /* unknown error, probably a bug */
#define CWOERR_NOMEM -2	    /* out of memory */
#define CWOERR_NOENT -3	    /* entry not found */
#define CWOERR_INVARG -4    /* invalid argument */
#define CWOERR_NULLARG -5   /* null argument */
#define CWOERR_INCARG -6    /* incompatible type of argument */
#define CWOERR_NOTIMP -7    /* not implemented */
#define CWOERR_EXISTS -8    /* entry already exists */
#define CWOERR_NOMETH -9    /* no such method */

#endif

