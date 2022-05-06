Todo:
 - [X] no illegal functions
 - [X] make sure of no leaks
 - [X] Error handling
 - [X] Sorting
 - [X] get rid of printfs
 - [X] get rid of atoi and strchr
 - [X] ask harm about sorting lexicographical
 - [X] Printing
   - [X] Grid
   - [X] Long format
   - [X] @ if any xattr, + if ACL
   - [X] Timestamps (if older than 6 months, show year)
   - [X] Fix janky grid
 - [X] Bugs
   - [X] `-u` without sorting
   - [X] implement `-d`
   - [X] implement `-g`
   - [X] Hangs on /tmp of cmake (except if `-a`)
   - [X] breaks on plain file and one argument
   - [ ] Check leaks

Before turnin:
 - [X] Compile with MAC_OS define
 - [X] Test everything on mac (acl, xattr, symlinks, colors, sorting)
 - [X] Cleanup proper makefile
 - [X] Remove libs folder
