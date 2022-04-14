# ft_ls

Remaking the `ls` command from scratch.<br/>
**Made for school (42  school)**

## whats included
 - List directory and file information
 - options: `-L`, `-r`, `-a`, `-R`, `-t`, `-u`, `-f`, `-g` & `-d`
 - dynamic column sizing (through ENV or terminal)
 - colorized output (through ENV or `-G`)
 - proper handling of ACL and extended attributes
 - another option: `--hi`

## functions used from standard library
 - memory functions: `malloc`, `free`
 - dir and file functions: `opendir`, `closedir`, `readdir`, `stat`, `lstat`
 - IO functions: `write`
 - error functions: `strerror`

## Development setup
Run the following to compile, this requires build-essentials to be installed.
```sh
# dont have 9 cores? run it without `-j 9`
make -j 9
```
Then just run it with `./ft_ls`

## FAQ

#### Found a bug?
Report it in the issues tab of github

#### Want to add a new feature
Do it yourself, I accept PR's

#### What's the answer to life universe and everything?
42

#### Can I do X and Y with your code?
It's an MIT license, do whatever you want.
