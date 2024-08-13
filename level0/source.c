#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int inputValue;
    char *shellPath;
    uid_t effectiveUserId;
    gid_t effectiveGroupId;

    // Convert the second command-line argument to an integer
    inputValue = atoi(argv[1]);

    // Check if the integer is equal to 0x1a7 (423 in decimal)
    if (inputValue == 423) { // 0x1a7
        // Duplicate the string "/bin/sh" and store the pointer in shellPath
        shellPath = strdup("/bin/sh");

        // Get the effective group ID and user ID
        effectiveGroupId = getegid();
        effectiveUserId = geteuid();

        // Set the real, effective, and saved group IDs to the effective group ID
        setresgid(effectiveGroupId, effectiveGroupId, effectiveGroupId);

        // Set the real, effective, and saved user IDs to the effective user ID
        setresuid(effectiveUserId, effectiveUserId, effectiveUserId);

        // Execute the shell "/bin/sh" with shellPath as the argument
        execv("/bin/sh", &shellPath);
    } else {
        // If the integer is not 423, print "No !\n" to the standard error
        fwrite("No !\n", 1, 5, stderr);
    }

    // Return 0
    return 0;
}