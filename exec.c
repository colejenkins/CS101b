#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Executes the program specified.
 * argv must be null-terminated.
 * 
 * istream, ostream, and errstream give file descriptors for input, output, and 
 * error. If specified, the child will use these instead of the standard ones.
 * 
 * Returns:
 * | 0 if successful
 * | -1 if program errored (errno maintaned)
 * | -2 if child process was unable to complete execution
 * | -3 if fork failed
 * |
 * | -12 if input redirection failed
 * | -14 if output redirection failed
 * | -16 if error redirection failed
 * | 
 * | -101 if argv was not null-terminated
 */
int exec_program(char *program, int argc, char **argv,
        int istream, int ostream, int errstream, int* to_close) {
    pid_t child_pid;
    
    child_pid = fork();
    
    if(argv[argc] != 0) { // check null termination
        return -101;
    }
    
    if(strcmp(program, "cd") == 0 || strcmp(program, "chdir") == 0) {
        exec_cd(argc, argv, istream, ostream, errstream);
    }
    
    if(strcmp(program, "exit") == 0) {
        exit(0);
    }
    
    if(child_pid >= 0) {
        if(child_pid == 0) { // child
            if(to_close[1] >= 0) {
                close(to_close[1]);
            }
            
            //redirection
            if(istream != STDIN_FILENO) {
                int err = dup2(istream, 0);
                if(err < 0) {
                    exit(-11);
                }
            }
            if(ostream != STDOUT_FILENO) {
                int err = dup2(ostream, 1);
                if(err < 0) {
                    exit(-12);
                }
            }
            if(errstream != STDERR_FILENO) {
                int err = dup2(errstream, 2);
                if(err < 0) {
                    exit(-13);
                }
            }
            
            // execution
            int success = execvp(program, argv);
            if(success == -1) {
                exit(errno);
            } else {
                exit(0);
            }
        } else { // parent
            int prog_err_code;
            int err = wait(&prog_err_code);
            if(err == -1) { // child process didn't finish
                return -2;
            } else if(prog_err_code > 0) { // program failed
                errno = prog_err_code;
                return -1;
            } else if(prog_err_code < 0) { // redirection failed
                return prog_err_code;
            } else {
                return 0;
            }
        }
    } else {
        return -3;
    }
}

// -17 pipe not created
// -21 multiple in redirects
// -22 multiple out redirects
int exec_command(char **tokens) {
    char **curr_ptr = tokens;
    bool end = 0;
    
    int next_istream = STDIN_FILENO;
    
    while(!end) {
        char *program = *curr_ptr;
        curr_ptr++;
        char **arg_start = curr_ptr;
        int argc = 0;
        
        int istream = next_istream;
        int ostream = STDOUT_FILENO;
        int errstream = STDERR_FILENO;
        next_istream = STDIN_FILENO;

        bool in_change = 0; //have these streams been redirected
        bool out_change = 0;
        
        // check for and process command symbols
        while(1) {
            if(istream != 0) {
                in_change = 1;
            }
            
            if(*curr_ptr == 0) {
                argc--;
                end = 1;
                break;
            } else if(strcmp(*curr_ptr, "<") == 0) {
                if(!in_change) {
                    return -21;
                }
                int file_desc = open(*(curr_ptr + 1), O_RDONLY);
                if(file_desc == -1) {
                    return -11;
                } else {
                    istream = file_desc;
                    in_change = 1;
                }
            } else if(strcmp(*curr_ptr, ">") == 0) {
                if(!out_change) {
                    return -22;
                }
                int file_desc = open(*(curr_ptr + 1), O_WRONLY);
                if(file_desc == -1) {
                    return -13;
                } else {
                    ostream = file_desc;
                    out_change = 1;
                }
            } else if(strcmp(*curr_ptr, "|") == 0) {
                if(!out_change) {
                    return -22;
                }
                int fd[2];
                int err = pipe(fd);
                if(err == -1) {
                    return -17;
                }
                
                ostream = fd[1];
                next_istream = fd[0];
                
                out_change = 1;
                break;
            }
            
            curr_ptr++;
            argc++;
        }
        
        // now execute
        char *argv[argc + 1];
        int j;
        for(j = 0; j < argc; j++) {
            argv[j] = *(arg_start + j);
        }
        argv[argc] = 0;
        exec_program(program, argc, argv, istream, ostream, errstream);
    }
}

void error_handler(int err_code, char *program) {
    if(err_code == -1) {
        printf("%s: Program execution failed. %s", program, strerror(errno));
    } else if(err_code == -2) {
        printf("%s: Process was interrupted, or "
                "otherwise could not be completed.", program);
    } else if(err_code == -3) {
        printf("%s: Process could not be created.", program);
    } else if(err_code == -11) {
        printf("%s: Could not read from specified file.", program);
    } else if(err_code == -12) {
        printf("%s: Could not write to specified file.", program);
    } else if(err_code == -13) {
        printf("%s: Could not write to specified error file.", program);
    }
}