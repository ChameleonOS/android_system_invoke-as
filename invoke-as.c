/*
 * Copyright (C) 2012 The ChameleonOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <pwd.h>

#include <private/android_filesystem_config.h>

#include "invoke-as.h"

static void usage(int status)
{
    FILE *stream = (status == EXIT_SUCCESS) ? stdout : stderr;

    fprintf(stream,
    "Usage: invoke-as [-h] | [-v] [-u USER] COMMAND\n\n"
    "Options:\n"
    "  -h                            display this help message and exit\n"
    "  -u USER                       run command as USER\n"
    "  -v                            display version number and exit\n");
    exit(status);
}


int main(int argc, char *argv[])
{
    char *user = NULL;
    int c;
    uid_t requestor;
    struct passwd *run_as;

    struct option long_opts[] = {
        { "help",			no_argument,		NULL, 'h' },
        { "user",			required_argument,		NULL, 'u' },
        { "version",			no_argument,		NULL, 'v' },
        { NULL, 0, NULL, 0 },
    };

    while ((c = getopt_long(argc, argv, "+hu:v", long_opts, NULL)) != -1) {
        switch(c) {
        case 'h':
            usage(EXIT_SUCCESS);
            break;
        case 'u':
            user = argv[optind-1];
            break;
        case 'v':
            printf("%s\n", VERSION);
            exit(EXIT_SUCCESS);
        default:
            /* Bionic getopt_long doesn't terminate its error output by newline */
            fprintf(stderr, "\n");
            usage(2);
        }
    }

    requestor = getuid();
    if (requestor != AID_ROOT && requestor != AID_SYSTEM && requestor != AID_RADIO) {
        printf("only root, system, or radio user can use this [%d]\n", requestor);
        exit(EXIT_SUCCESS);
    }

    if (user) {
        // check if a UID was passed, if so use getpwuid
        if (isdigit(user[0]))
            run_as=getpwuid(atoi(user));
        else
            run_as = getpwnam(user);
    } else {
        run_as = getpwnam("root");
    }

    if (run_as) {
        if (seteuid(run_as->pw_uid)) {
            printf("failed to seteuid(%d)\n", run_as->pw_uid);
            exit(EXIT_FAILURE);
        }
        if (setegid(run_as->pw_gid)) {
            printf("failed to setegid(%d)\n", run_as->pw_gid);
            exit(EXIT_FAILURE);
        }
        if (optind >= argc) {
            printf("Expected COMMAND after options\n");
            usage(EXIT_FAILURE);
        }
        LOGD("executing %s as %s\n", argv[optind], run_as->pw_name);
        execvp(argv[optind], argv + optind);
    } else {
        printf("user does not exist!\n");
    }

    return EXIT_SUCCESS;
}
