#if !defined(NSID_H)
#define NSID_H

#include <stdio.h>

#define VERSION "0.1"

#define BOLD "\e[1m"
#define WHITE "\e[97m"
#define GREEN "\e[32m"
#define CYAN "\e[36m"
#define RED "\e[31m"
#define RESET "\e[0m"

#define MSGLEN 64
#define NSID_CONF "/.nsid"

#define BASE_SHEBANG "#!/usr/bin/env "
#define NUMLANGS 5
#define PYTHON2 0
#define PYTHON3 1
#define RUBY 2
#define BASH 3
#define PERL 4

// print help message
void usage();

// Verify there is the right number of arguments
int check_num_args(int argc);

// User messages
void nsid_error(const char *msg);
void nsid_success(const char *msg);

int parse_default_lang_arg(char *arg);

char *construct_shebang(int lang);

void create_file(const char *filename, int lang);

// Is an option defined?
int option_is_valid(const char *option, const char *short_flag,
                    const char *long_flag);

int arg_is_flag(const char *arg);

// Check environment vaiables for default language variables, will
// create one if there's not
char *get_default_lang();

void set_default_lang(int lang);

int nsid_conf_exists(const char *path);
void create_nsid_conf(const char *file_path);
FILE *open_nsid_conf();

#endif // NSID_H
