#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <termios.h>
using namespace std;

void* out_writer(void* args) {
    int fd1;
    const char* outfifo = static_cast<const char*>(args);
    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(outfifo, 0666);

    // open in read only and read
    fd1 = open(outfifo,O_WRONLY);
    char c;
    while (1) {
        read(0, &c, 1); // 0 is stdin filehandle
        fprintf(stderr, "Wrote: %02X\n", c);
        // Write c to the out pipe.
        write(fd1, &c, 1);
        fsync(fd1);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s inpipe outpipe\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    struct termios old_tio, new_tio;
    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO, &old_tio);
    /* we want to keep the old setting to restore them a the end */
    new_tio = old_tio;
    /* disable canonical mode (buffered i/o) and local echo */
    new_tio.c_lflag &= unsigned(~ICANON & ~ECHO);
    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
    setvbuf(stdin, nullptr, _IONBF, 0); //turn off buffering
    cerr << "Welcome to the binary piper! Using pipes /tmp/myfifoin /tmp/myfifoout" << endl;
    int fd1;

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(argv[1], 0666);

    pthread_t inc_x_thread;

    if(pthread_create(&inc_x_thread, nullptr, out_writer, argv[2])) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // open in read only and read
    fd1 = open(argv[1],O_RDONLY);

    char str1[1];
    while (1)
    {
        if (read(fd1, str1, 1) > 0) {
            // Print the read string and close
            fprintf(stderr, "Read: %02X\n", *str1);
            cout << str1 << flush;
        };
    }
}
