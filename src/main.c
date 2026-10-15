#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_CAP 4096
#define PATH_CAP 32768

static void print_help(void) {
    puts("ArchNT - native Windows terminal workflow");
    puts("");
    puts("Built-ins:");
    puts("  help       Show this help");
    puts("  clear      Clear the console");
    puts("  pwd        Print the current directory");
    puts("  cd [path]  Change the current directory");
    puts("  exit       Exit ArchNT");
    puts("");
    puts("Other commands are passed to cmd.exe.");
    puts("ArchNT does not provide a Linux kernel or Linux ABI.");
}

static int starts_with_command(const char *line, const char *name) {
    size_t n = strlen(name);
    return _strnicmp(line, name, n) == 0 &&
           (line[n] == '\0' || line[n] == ' ' || line[n] == '\t');
}

int main(void) {
    char line[INPUT_CAP];
    char cwd[PATH_CAP];

    SetConsoleTitleW(L"ArchNT");
    puts("ArchNT 0.1.0 - native Windows shell prototype");
    puts("Type 'help' for built-ins. This is a workflow shell, not a Linux kernel.");
    puts("");

    for (;;) {
        if (_getcwd(cwd, (int)sizeof(cwd)) != NULL) {
            printf("\033[36m%s\033[0m > ", cwd);
        } else {
            printf("ArchNT > ");
        }
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            putchar('\n');
            break;
        }

        line[strcspn(line, "\r\n")] = '\0';
        char *cmd = line;
        while (*cmd == ' ' || *cmd == '\t') ++cmd;
        if (*cmd == '\0') continue;

        if (_stricmp(cmd, "exit") == 0) {
            break;
        } else if (_stricmp(cmd, "help") == 0) {
            print_help();
        } else if (_stricmp(cmd, "clear") == 0 || _stricmp(cmd, "cls") == 0) {
            system("cls");
        } else if (_stricmp(cmd, "pwd") == 0) {
            if (_getcwd(cwd, (int)sizeof(cwd)) != NULL) puts(cwd);
            else perror("pwd");
        } else if (starts_with_command(cmd, "cd")) {
            char *path = cmd + 2;
            while (*path == ' ' || *path == '\t') ++path;
            if (*path == '\0') {
                if (_getcwd(cwd, (int)sizeof(cwd)) != NULL) puts(cwd);
            } else if (_chdir(path) != 0) {
                perror("cd");
            }
        } else {
            /* Intentionally delegates external commands to Windows cmd.exe. */
            size_t len = strlen(cmd);
            if (len > INPUT_CAP - 16) {
                fputs("Command is too long.\n", stderr);
                continue;
            }
            char command[INPUT_CAP + 16];
            snprintf(command, sizeof(command), "cmd.exe /D /S /C \"%s\"", cmd);
            (void)system(command);
        }
    }

    puts("ArchNT session ended.");
    return 0;
}
