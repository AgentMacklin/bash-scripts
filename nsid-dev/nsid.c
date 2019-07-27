
#include "nsid.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *languages[NUMLANGS] = {"python2", "python3", "ruby", "bash",
                                          "perl"};

static char *strlwr(char *str) {
    unsigned char *p = (unsigned char *)str;

    while (*p) {
        *p = tolower((unsigned char)*p);
        p++;
    }

    return str;
}

void usage(int argc, char **argv) {
    char *default_lang = get_default_lang();
    printf(GREEN "\nnsid " RESET VERSION " Default: %s\n", default_lang);
    printf(
        CYAN
        "Usage:" RESET " nsid <options> [filename | default]\n\n"
        "Create a new script for a language. If no options are passed nsid\n"
        "will default to Bash, but you have the option to edit the default\n"
        "language. nsid always requires a file name unless you are editing\n"
        "the defaults.\n\n" CYAN "Options:\n" RESET
        "       --help                 Print this help message\n"
        "  -r,  --ruby <name>          Create a new Ruby script\n"
        "  -b,  --bash <name>          Create a new Bash script\n"
        "  -p2, --python2 <name>       Create a new Python2 script\n"
        "  -p3, --python3 <name>       Create a new Python3 script\n"
        "  -p,  --perl <name>          Create a new Python3 script\n"
        "  -d,  --default <language>   Edit the default scripting language\n"
        "  -sd, --show-default         Show your current default language\n\n");
}

void nsid_error(const char *msg) {
    printf(WHITE "[" RED "nsid" WHITE "]" RESET " %s\n", msg);
    exit(1);
}

void nsid_success(const char *msg) {
    printf(WHITE "[" GREEN "nsid" WHITE "]" RESET " %s\n", msg);
}

char *construct_shebang(int lang) {
    char *shebang = calloc(64, sizeof(char));
    switch (lang) {
    case PYTHON2:
        sprintf(shebang, BASE_SHEBANG "%s", "python2");
        break;

    case PYTHON3:
        sprintf(shebang, BASE_SHEBANG "%s", "python3");
        break;

    case RUBY:
        sprintf(shebang, BASE_SHEBANG "%s", "ruby");
        break;

    case BASH:
        sprintf(shebang, BASE_SHEBANG "%s", "bash");
        break;

    default:
        return "There was an error";
        break;
    }
    return shebang;
}

void create_file(const char *filename, int lang) {
    char *shebang = construct_shebang(lang);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        nsid_error("Could not create file for script");
        exit(1);
    }
    fprintf(file, "%s\n", shebang);
    chmod(filename, S_IRWXU); // change file permissions so only owner can rwx
    nsid_success(
        "Created script with owner read, write, and execute permissions");
    fclose(file);
}

int option_is_valid(const char *option, const char *short_flag,
                    const char *long_flag) {
    return strcmp(option, short_flag) == 0 || strcmp(option, long_flag) == 0;
}

// most likely will need to be "smarter than this"
int arg_is_flag(const char *arg) { return arg[0] == '-'; }

int nsid_conf_exists(const char *path) {
    return access(path, F_OK) == 0 ? 1 : 0;
}

void create_nsid_conf(const char *file_path) {
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        nsid_error("Could not create a configuration file");
    }
    fprintf(file, "%s\n", "bash");
    nsid_success("Created nsid configuration file, default set to bash");
    fclose(file);
}

FILE *open_nsid_conf() {
    char *user_path = getenv("HOME");
    char *file_path = strcat(user_path, NSID_CONF);
    if (!nsid_conf_exists(file_path)) {
        create_nsid_conf(file_path);
    }
    return fopen(file_path, "r+");
}

char *get_default_lang() {
    char buffer[32];
    FILE *file = open_nsid_conf();
    if (file == NULL) {
        nsid_error("Could not open nsid configuration file");
    }
    char *default_lang = fgets(buffer, 32, file);
    // eat the newline character
    default_lang[strlen(default_lang) - 1] = '\0';
    fclose(file);
    return default_lang;
}

void set_default_lang(int lang) {
    int status = 0;
    char *lang_set_to = calloc(32, sizeof(char));
    FILE *conf = open_nsid_conf();
    status = fprintf(conf, "%s\n", languages[lang]);
    sprintf(lang_set_to, "%s", languages[lang]);
    if (status < 1) {
        nsid_error("There was an error setting the default langauge");
    } else {
        char *msg = calloc(128, sizeof(char));
        sprintf(msg, "Default language set to %s", lang_set_to);
        nsid_success(msg);
        free(msg);
    }
    free(lang_set_to);
}

int parse_default_lang_arg(char *arg) {
    char *formatted_arg = strlwr(arg);
    // for some reason if strcpy is not here this function does not work
    for (int i = 0; i < NUMLANGS; i++) {
        // if match then break and return arg
        const char *lang = languages[i];
        if (strcmp(formatted_arg, lang) == 0) {
            return i;
        }
    }
    // If there's no match we get here and exit(1)
    nsid_error("Unknown language argument");
    return -1;
}