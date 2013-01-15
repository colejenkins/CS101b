/* 
 * File:   exec.h
 * Author: Jose
 *
 * Created on January 15, 2013, 4:45 AM
 */

#ifndef EXEC_H
#define	EXEC_H

#ifdef	__cplusplus
extern "C" {
#endif

int exec_program(char *program, int argc, char **argv,
    int istream, int ostream, int errstream, int* to_close);

int exec_command(char **tokens);

void error_handler(int err_code, char *program);



#ifdef	__cplusplus
}
#endif

#endif	/* EXEC_H */

