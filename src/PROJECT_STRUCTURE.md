# The project structure

The overal flow of the project goes like this:
1. inputs are parsed into a context
2. all information is retrieved
3. information is sorted and parsed
4. the information is printed

## directories
 - `/cli` - Everything about parsing inputs
 - `/printing` - Everything about outputing/printing
 - `/io` - Everything about I/O, like retrieving file info
 - `/utils` - Extra utilities
 - `/includes` - Header files

## input parsing
```
IF args.len() == 0
  args[0] = "."

IF args.len() == 1 AND args[0].is_dir()
  print_dir(args[0])
ELSE
  print_multiple(args)
```
