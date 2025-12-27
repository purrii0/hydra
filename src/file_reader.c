#include "../include/file_reader.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void read_file(const char *file_path, char *source) {
  int fd = open(file_path, O_RDONLY);

  if (fd == -1) {
    perror("open");
    return;
  }

  ssize_t num_read;
  ssize_t total = 0;

  while ((num_read = read(fd, source + total, 65536 - total - 1)) > 0) {
    total += num_read;
  }

  if (num_read == -1) {
    perror("read");
    close(fd);
    return;
  }

  source[total] = '\0';
  close(fd);
}
