$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$
prefsect renames the sections in a psy-q object file by prefixing the
names with a defined string.

usage   : prefsect [options] infile outfile prefix
options : -g create a group of the same name as the prefix and put all
             sections in this group

e.g.

prefsect mod1.obj newmod1.obj psyq

will prefix the sections in mod1.obj with psyq

e.g.   .text  will become  psyq.text




prefsect -g mod1.obj newmod1.obj psyq

will also create a group called psyq and put all the sections in this group


