#include "nsid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLAGSIZE 16
#define ARGSIZE 128

int main(int argc, char **argv) {

    char *flag = NULL;
    char *argument = NULL;

    /**
     * Parse arguments into respective strings
     */
    if (argc == 1) {
        nsid_error("Must supply at least an option and/or file name");
    } else if (argc == 2) {
        if (arg_is_flag(argv[1])) {
            flag = malloc(strlen(argv[1] + 1));
            strcpy(flag, argv[1]);
        } else {
            argument = malloc(strlen(argv[1] + 1));
            strcpy(argument, argv[1]);
        }
    } else if (argc == 3) {
        flag = malloc(strlen(argv[1] + 1));
        argument = malloc(strlen(argv[2] + 1));
        strcpy(flag, argv[1]);
        strcpy(argument, argv[2]);
    } else if (argc >= 4) {
        int num_args = argc - 1;
        char msg[MSGLEN];
        snprintf(msg, MSGLEN, "Too many arguments, expected 1 or 2, got %d",
                 num_args);
        nsid_error(msg);
    }

    /**
     * Run commands based on options
     */
    if (flag != NULL) {
        if (strcmp(flag, "--help") == 0) {
            usage();
        } else if (option_is_valid(flag, "-r", "--ruby")) {
            if (argument == NULL)
                nsid_error("Missing file name argument");
            create_file(argument, RUBY);
        } else if (option_is_valid(flag, "-p2", "--python2")) {
            if (argument == NULL)
                nsid_error("Missing file name argument");
            create_file(argument, PYTHON2);
        } else if (option_is_valid(flag, "-p3", "--python3")) {
            if (argument == NULL)
                nsid_error("Missing file name argument");
            create_file(argument, PYTHON3);
        } else if (option_is_valid(flag, "-b", "--bash")) {
            if (argument == NULL)
                nsid_error("Missing file name argument");
            create_file(argument, BASH);
        } else if (option_is_valid(flag, "-p", "--perl")) {
            if (argument == NULL)
                nsid_error("Missing file name argument");
            create_file(argument, PERL);
        } else if (option_is_valid(flag, "-sd", "--show-default")) {
            if (argument != NULL)
                nsid_error("--show-default takes no parameters");
            char *lang = get_default_lang();
            printf("%s \n", lang);
        } else if (option_is_valid(flag, "-d", "--default")) {
            if (argument == NULL)
                nsid_error("Missing default language argument");
            int lang = parse_default_lang_arg(argument);
            set_default_lang(lang);
        } else {
            char *msg = calloc(64, sizeof(char));
            sprintf(msg, "%s is not a valid flag", flag);
            nsid_error(msg);
        }
    } else if (argument != NULL) {
        // get default language and create file for it
        char *default_lang = get_default_lang();
        int lang = parse_default_lang_arg(default_lang);
        create_file(argument, lang);
    } else {
        char msg[MSGLEN];
        sprintf(msg, "%s is not a valid option", flag);
        nsid_error(msg);
    }
    return 0;
}