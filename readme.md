# Archiver

## Usage

A console tool to archive and unarchive files using [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding).

Archiver has the following interface:

- `archiver -c archive_name file1 [file2 ...]` - archive files `file1, file2, ...` and save the result in the file `archive_name`.
- `archiver -d archive_name` - unarchive files from the archive `archive_name` and put them in the current directory. Filenames will be preserved after archiving and unarchiving.
- `archiver -h` - display help about using the program.
