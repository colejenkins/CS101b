/* 
 * File:   main.c
 * Author: Joseph Torres
 * Author: Lucia Ahn
 * Author: Lawrence Jenkins
 *
 * Created on January 10, 2013, 2:57 AM
 */

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_NUM_TOKENS 64 //

/*
 * The main control loop for the program.
 */
int control_loop() {
    char *user = getlogin();
    char *cwd = getcwd();
    int user_len = strlen(user);
    int cwd_len = strlen(cwd);
    // update this length for changes in prompt structure!
    char prompt_str[user_len + 3 + cwd_len + 1 + 1];
    // write $user + ' : ' + $cwd to prompt string
    strncpy(prompt_str, user, user_len + 1); // we don't want null termination here!
    prompt_str[user_len] = ' ';
    strncpy(prompt_str + user_len + 1, cwd, cwd_len + 1);
}

/*
 * 
 */
int main(int argc, char** argv) {
    return (EXIT_SUCCESS);
}

