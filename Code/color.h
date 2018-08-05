#ifndef _COLOR_H_
#define _COLOR_H_

/* FOREGROUND */

#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define BOLD "\x1B[1m"
#define UNDERLINE "\x1B[4m"

#define ERROR_MSG BOLD << KRED << "ERROR: " << RST
#define WARNING_MSG BOLD << KYEL << "WARNING: " << RST

#endif  /* _COLOR_ */
