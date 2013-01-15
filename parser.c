/*
 * Decomposes a command into tokens. Returns an array of strings containing the
 * tokens. num_tokens contains the number of tokens.
 */
char ** tokenize(char *command, int *num_tokens) {
    int i;
    int start = 0; // beginning of the current token
    int token_length;
    int command_length = strlen(command);
    
    char *tokens[DEFAULT_NUM_TOKENS];
    int curr_token_num = 0;
    
    for(i = 0; i < command_length; i++) {
        if(command[i] == ' ' || command[i] ==) {
            token_length = i - start;
            char token[token_length];
            strncpy(token, command + start, token_length);
            tokens[curr_token_num] = token;
            curr_token_num++;
        }
        if(command[i] == ' ') {
            i++;
        } else if(command[i] == '<') {
            char token[1];
            token[0] = '<';
            curr_token_num++;
            i++;
        } else if(command[i] == '>') {
            char token[1];
            token[0] = '>';
            curr_token_num++;
            i++;
        } else if(command[i] == '|') {
            char token[1];
            token[0] = '|';
            curr_token_num++;
            i++;
        }
    }
    
    *num_tokens = curr_token_num;
}
