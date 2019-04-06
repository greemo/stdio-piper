# stdio-piper
An application that takes two named pipes and forwards the data to and from stdin and stdout
Outputs the characters read from the inpipe to stdout, and forwards the characters read from stdin to the outpipe.
Also echoes characters read from inpipe and written to outpipe to stderr in Hex format

Usage: piper inpipe outpipe

Probably an easier way to do this, bit this worked for me...

Useful for programs you want to feed data to and read data from via pipes. I used it for testing applications requiring a qiodevice.

Helpful commands to read to and write from a pipe:

Write to pipe:
```echo -n -e \\x11 > /tmp/myfifoin```

Follow a pipe:
```cat /tmp/myfifoout | hexdump -v -e '/1 "%02X\n"'```
